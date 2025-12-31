#include "api_endpoints.h"
#include <soci/soci.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace mb;
using json = nlohmann::json;

// Helper function to get current timestamp
std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S%z");
    return oss.str();
}

// Helper function to hash password (SHA3-256)
std::string hashPassword(const std::string& password, const std::string& salt) {
    // TODO: Implement proper password hashing with SHA3-256
    // For now, return a placeholder
    return password + ":" + salt;
}

void setupUserEndpoints(Router& router, MantisBase& app) {
    // Get all users
    router.Get("/api/v1/salamapos/users", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            soci::rowset<soci::row> rs = (sql->prepare << 
                "SELECT u.firstname, u.lastname, u.username, u.password, u.phone_no, "
                "u.date_added, p.can_add_user, p.can_remove_user, p.can_add_product, "
                "p.can_remove_product, p.can_add_stock, p.can_remove_stock, "
                "p.can_remove_sales, p.can_backup, u.to_change_password "
                "FROM users u "
                "INNER JOIN priviledges p ON u.username = p.username");
            
            json users = json::array();
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                soci::row const& row = *it;
                json user;
                user["firstname"] = row.get<std::string>(0);
                user["lastname"] = row.get<std::string>(1);
                user["username"] = row.get<std::string>(2);
                user["password"] = row.get<std::string>(3);
                user["phone_no"] = row.get<std::string>(4);
                user["date_added"] = row.get<std::string>(5);
                user["canAddUser"] = row.get<bool>(6);
                user["canRemoveUsers"] = row.get<bool>(7);
                user["canAddItems"] = row.get<bool>(8);
                user["canRemoveItems"] = row.get<bool>(9);
                user["canAddStock"] = row.get<bool>(10);
                user["canRemoveStock"] = row.get<bool>(11);
                user["canUndoSales"] = row.get<bool>(12);
                user["canBackupDb"] = row.get<bool>(13);
                user["changePassword"] = row.get<bool>(14);
                users.push_back(user);
            }
            
            json response;
            response["success"] = true;
            response["users"] = users;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Add new user
    router.Post("/api/v1/salamapos/users", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            json requestBody = json::parse(req.body());
            std::string firstname = requestBody.value("firstname", "");
            std::string lastname = requestBody.value("lastname", "");
            std::string username = requestBody.value("username", "");
            std::string password = requestBody.value("password", "");
            std::string phone_no = requestBody.value("phone_no", "");
            std::string date_added = requestBody.value("date_added", getCurrentTimestamp());
            
            if (firstname.empty() || lastname.empty() || username.empty() || password.empty()) {
                json response;
                response["success"] = false;
                response["error"] = "Missing required fields";
                res.sendJson(400, response);
                return;
            }
            
            auto sql = app.db().session();
            
            // Check if username exists
            soci::row row;
            *sql << "SELECT username FROM users WHERE username = :username",
                    soci::use(username), soci::into(row);
            
            if (sql->got_data()) {
                json response;
                response["success"] = false;
                response["error"] = "Username already exists";
                res.sendJson(409, response);
                return;
            }
            
            // Hash password
            std::string salt = std::to_string(std::time(nullptr));
            std::string hashedPassword = hashPassword(password, salt);
            
            // Insert user
            *sql << "INSERT INTO users(firstname, lastname, username, password, phone_no, date_added) "
                    "VALUES (:firstname, :lastname, :username, :password, :phone_no, :date_added)",
                    soci::use(firstname), soci::use(lastname), soci::use(username),
                    soci::use(hashedPassword), soci::use(phone_no), soci::use(date_added);
            
            // Insert privileges
            *sql << "INSERT INTO priviledges(username) VALUES (:username)",
                    soci::use(username);
            
            json response;
            response["success"] = true;
            response["message"] = "User created successfully";
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Update user
    router.Put("/api/v1/salamapos/users/:username", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string username = req.getPathParamValue("username");
            json requestBody = json::parse(req.body());
            
            std::string firstname = requestBody.value("firstname", "");
            std::string lastname = requestBody.value("lastname", "");
            std::string phone_no = requestBody.value("phone_no", "");
            std::string orig_username = requestBody.value("orig_username", username);
            
            auto sql = app.db().session();
            *sql << "UPDATE users SET firstname=:firstname, lastname=:lastname, "
                    "username=:username, phone_no=:phone_no WHERE username=:orig_username",
                    soci::use(firstname), soci::use(lastname), soci::use(username),
                    soci::use(phone_no), soci::use(orig_username);
            
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Update user privileges
    router.Put("/api/v1/salamapos/users/:username/privileges", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string username = req.getPathParamValue("username");
            json requestBody = json::parse(req.body());
            
            bool canAddUser = requestBody.value("canAddUser", false);
            bool canRemoveUsers = requestBody.value("canRemoveUsers", false);
            bool canAddItems = requestBody.value("canAddItems", false);
            bool canRemoveItems = requestBody.value("canRemoveItems", false);
            bool canAddStock = requestBody.value("canAddStock", false);
            bool canRemoveStock = requestBody.value("canRemoveStock", false);
            bool canUndoSales = requestBody.value("canUndoSales", false);
            bool canBackupDb = requestBody.value("canBackupDb", false);
            
            auto sql = app.db().session();
            *sql << "UPDATE priviledges SET can_add_user=:can_add_user, can_remove_user=:can_remove_user, "
                    "can_add_product=:can_add_product, can_remove_product=:can_remove_product, "
                    "can_add_stock=:can_add_stock, can_remove_stock=:can_remove_stock, "
                    "can_remove_sales=:can_remove_sales, can_backup=:can_backup "
                    "WHERE username=:username",
                    soci::use(canAddUser), soci::use(canRemoveUsers), soci::use(canAddItems),
                    soci::use(canRemoveItems), soci::use(canAddStock), soci::use(canRemoveStock),
                    soci::use(canUndoSales), soci::use(canBackupDb), soci::use(username);
            
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Update password
    router.Put("/api/v1/salamapos/users/:username/password", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string username = req.getPathParamValue("username");
            json requestBody = json::parse(req.body());
            std::string oldPassword = requestBody.value("oldPassword", "");
            std::string newPassword = requestBody.value("newPassword", "");
            
            if (newPassword.empty()) {
                json response;
                response["success"] = false;
                response["error"] = "New password required";
                res.sendJson(400, response);
                return;
            }
            
            auto sql = app.db().session();
            
            // Verify old password
            soci::row row;
            *sql << "SELECT password FROM users WHERE username=:username",
                    soci::use(username), soci::into(row);
            
            if (!sql->got_data()) {
                json response;
                response["success"] = false;
                response["error"] = "User not found";
                res.sendJson(404, response);
                return;
            }
            
            // TODO: Verify password hash
            std::string storedPassword = row.get<std::string>(0);
            
            // Update password
            std::string salt = std::to_string(std::time(nullptr));
            std::string hashedPassword = hashPassword(newPassword, salt);
            *sql << "UPDATE users SET password=:password WHERE username=:username",
                    soci::use(hashedPassword), soci::use(username);
            
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Delete user
    router.Delete("/api/v1/salamapos/users/:username", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string username = req.getPathParamValue("username");
            auto sql = app.db().session();
            *sql << "DELETE FROM users WHERE username=:username", soci::use(username);
            
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Mark account for deletion
    router.Put("/api/v1/salamapos/users/:username/mark-delete", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string username = req.getPathParamValue("username");
            auto sql = app.db().session();
            *sql << "UPDATE users SET to_delete_account=true WHERE username=:username",
                    soci::use(username);
            
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupStockEndpoints(Router& router, MantisBase& app) {
    // Get all stock items
    router.Get("/api/v1/salamapos/stock", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            soci::rowset<soci::row> rs = (sql->prepare << 
                "SELECT s.barcode, s.stock_qty, s.last_update, p.product_name, "
                "p.product_unit, p.product_bp, p.product_sp, p.product_company, p.type_id "
                "FROM stock s "
                "INNER JOIN product p ON s.barcode = p.barcode "
                "WHERE p.product_deleted = false");
            
            json items = json::array();
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                soci::row const& row = *it;
                json item;
                item["barcode"] = row.get<std::string>(0);
                item["stock_qty"] = row.get<int>(1);
                item["last_update"] = row.get<std::string>(2);
                item["product_name"] = row.get<std::string>(3);
                item["product_unit"] = row.get<std::string>(4);
                item["product_bp"] = row.get<double>(5);
                item["product_sp"] = row.get<double>(6);
                item["product_company"] = row.get<std::string>(7);
                item["type_id"] = row.get<std::string>(8);
                items.push_back(item);
            }
            
            json response;
            response["success"] = true;
            response["items"] = items;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Update stock
    router.Put("/api/v1/salamapos/stock/:barcode", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            std::string barcode = req.getPathParamValue("barcode");
            json requestBody = json::parse(req.body());
            int stock_qty = requestBody.value("stock_qty", 0);
            std::string who_updated = requestBody.value("who_updated", "");
            bool is_adding = requestBody.value("is_adding", true);
            
            auto sql = app.db().session();
            
            // Get current stock
            soci::row row;
            *sql << "SELECT stock_qty FROM stock WHERE barcode=:barcode",
                    soci::use(barcode), soci::into(row);
            
            int stock_qty_before = 0;
            if (sql->got_data()) {
                stock_qty_before = row.get<int>(0);
            }
            
            int stock_qty_added = is_adding ? stock_qty : -stock_qty;
            int new_qty = stock_qty_before + stock_qty_added;
            
            // Update stock
            *sql << "UPDATE stock SET stock_qty=:qty, last_update=:timestamp WHERE barcode=:barcode",
                    soci::use(new_qty), soci::use(getCurrentTimestamp()), soci::use(barcode);
            
            // Insert stock history
            *sql << "INSERT INTO stock_history(barcode, stock_qty_before, stock_qty_added, "
                    "date_updated, who_updated, is_adding) "
                    "VALUES (:barcode, :before, :added, :timestamp, :who, :adding)",
                    soci::use(barcode), soci::use(stock_qty_before), soci::use(stock_qty_added),
                    soci::use(getCurrentTimestamp()), soci::use(who_updated), soci::use(is_adding);
            
            json response;
            response["success"] = true;
            response["stock_qty"] = new_qty;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupSalesEndpoints(Router& router, MantisBase& app) {
    // Get sales
    router.Get("/api/v1/salamapos/sales", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            soci::rowset<soci::row> rs = (sql->prepare << 
                "SELECT sales_id, sales_date, total_cost FROM sales ORDER BY sales_date DESC");
            
            json sales = json::array();
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                soci::row const& row = *it;
                json sale;
                sale["sales_id"] = row.get<int>(0);
                sale["sales_date"] = row.get<std::string>(1);
                sale["total_cost"] = row.get<double>(2);
                sales.push_back(sale);
            }
            
            json response;
            response["success"] = true;
            response["sales"] = sales;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
    
    // Create sale
    router.Post("/api/v1/salamapos/sales", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            json requestBody = json::parse(req.body());
            // TODO: Implement sale creation
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupCrediteeEndpoints(Router& router, MantisBase& app) {
    // Get all creditees
    router.Get("/api/v1/salamapos/creditees", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            soci::rowset<soci::row> rs = (sql->prepare << 
                "SELECT id, firstname, lastname, national_id, phone_no, amount_due, date_added "
                "FROM creditee ORDER BY date_added DESC");
            
            json creditees = json::array();
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                soci::row const& row = *it;
                json creditee;
                creditee["id"] = row.get<int>(0);
                creditee["firstname"] = row.get<std::string>(1);
                creditee["lastname"] = row.get<std::string>(2);
                creditee["national_id"] = row.get<std::string>(3);
                creditee["phone_no"] = row.get<std::string>(4);
                creditee["amount_due"] = row.get<double>(5);
                creditee["date_added"] = row.get<std::string>(6);
                creditees.push_back(creditee);
            }
            
            json response;
            response["success"] = true;
            response["creditees"] = creditees;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupProductEndpoints(Router& router, MantisBase& app) {
    // Get all products
    router.Get("/api/v1/salamapos/products", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            soci::rowset<soci::row> rs = (sql->prepare << 
                "SELECT barcode, product_name, product_unit, product_bp, product_sp, "
                "product_company, type_id FROM product WHERE product_deleted = false");
            
            json products = json::array();
            for (auto it = rs.begin(); it != rs.end(); ++it) {
                soci::row const& row = *it;
                json product;
                product["barcode"] = row.get<std::string>(0);
                product["product_name"] = row.get<std::string>(1);
                product["product_unit"] = row.get<std::string>(2);
                product["product_bp"] = row.get<double>(3);
                product["product_sp"] = row.get<double>(4);
                product["product_company"] = row.get<std::string>(5);
                product["type_id"] = row.get<std::string>(6);
                products.push_back(product);
            }
            
            json response;
            response["success"] = true;
            response["products"] = products;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupDashboardEndpoints(Router& router, MantisBase& app) {
    // Get sales statistics
    router.Get("/api/v1/salamapos/dashboard/sales-stats", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            // TODO: Implement sales statistics query
            json response;
            response["success"] = true;
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

void setupAllEndpoints(Router& router, MantisBase& app) {
    setupUserEndpoints(router, app);
    setupStockEndpoints(router, app);
    setupSalesEndpoints(router, app);
    setupCrediteeEndpoints(router, app);
    setupProductEndpoints(router, app);
    setupDashboardEndpoints(router, app);
}

