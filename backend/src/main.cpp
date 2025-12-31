#include <mantisbase/mantisbase.h>
#include <mantisbase/core/router.h>
#include <mantisbase/core/database.h>
#include <iostream>
#include <soci/soci.h>
#include <nlohmann/json.hpp>
#include "api_endpoints.h"

using namespace mb;
using json = nlohmann::json;

// Health check endpoint
void setupHealthEndpoint(Router& router) {
    router.Get("/api/v1/health", [](MantisRequest& req, MantisResponse& res) {
        json response;
        response["status"] = "ok";
        response["service"] = "salama-pos-backend";
        res.sendJson(200, response);
    }, {});
}

// Database initialization endpoint
void setupDatabaseEndpoints(Router& router, MantisBase& app) {
    // Initialize database tables
    router.Post("/api/v1/salamapos/database/init", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            auto sql = app.db().session();
            
            // Read SQL from request body or use default schema
            json requestBody;
            if (!req.body().empty()) {
                requestBody = json::parse(req.body());
            }
            
            // TODO: Execute table creation SQL
            // For now, just return success
            json response;
            response["success"] = true;
            response["message"] = "Database initialized";
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

// User authentication endpoint
void setupAuthEndpoints(Router& router, MantisBase& app) {
    router.Post("/api/v1/salamapos/auth/login", [&app](MantisRequest& req, MantisResponse& res) {
        try {
            json requestBody = json::parse(req.body());
            std::string username = requestBody.value("username", "");
            std::string password = requestBody.value("password", "");
            
            if (username.empty() || password.empty()) {
                json response;
                response["success"] = false;
                response["error"] = "Username and password required";
                res.sendJson(400, response);
                return;
            }
            
            auto sql = app.db().session();
            soci::row row;
            *sql << "SELECT u.id, u.username, u.firstname, u.lastname, u.phone_no, "
                    "p.can_add_user, p.can_remove_user, p.can_add_product, p.can_remove_product, "
                    "p.can_add_stock, p.can_remove_stock, p.can_remove_sales, p.can_backup "
                    "FROM users u "
                    "INNER JOIN priviledges p ON u.username = p.username "
                    "WHERE u.username = :username",
                    soci::use(username), soci::into(row);
            
            if (!sql->got_data()) {
                json response;
                response["success"] = false;
                response["error"] = "Invalid credentials";
                res.sendJson(401, response);
                return;
            }
            
            // TODO: Verify password hash
            // For now, just return user data
            json response;
            response["success"] = true;
            response["user"] = json::object();
            response["user"]["user_id"] = row.get<int>(0);
            response["user"]["username"] = row.get<std::string>(1);
            response["user"]["firstname"] = row.get<std::string>(2);
            response["user"]["lastname"] = row.get<std::string>(3);
            response["user"]["phone_no"] = row.get<std::string>(4);
            response["user"]["canAddUser"] = row.get<bool>(5);
            response["user"]["canRemoveUsers"] = row.get<bool>(6);
            response["user"]["canAddItems"] = row.get<bool>(7);
            response["user"]["canRemoveItems"] = row.get<bool>(8);
            response["user"]["canAddStock"] = row.get<bool>(9);
            response["user"]["canRemoveStock"] = row.get<bool>(10);
            response["user"]["canUndoSales"] = row.get<bool>(11);
            response["user"]["canBackupDb"] = row.get<bool>(12);
            
            res.sendJson(200, response);
        } catch (const std::exception& e) {
            json response;
            response["success"] = false;
            response["error"] = e.what();
            res.sendJson(500, response);
        }
    }, {});
}

int main(int argc, char** argv)
{
    // Configure mantisbase with PostgreSQL
    json config;
    config["database"] = "PSQL";
    
    // Get database connection from environment variables
    std::string dbHost = std::getenv("S_DB_HOST") ? std::getenv("S_DB_HOST") : "localhost";
    std::string dbUser = std::getenv("S_DB_USERNAME") ? std::getenv("S_DB_USERNAME") : "postgres";
    std::string dbName = std::getenv("S_DB_NAME") ? std::getenv("S_DB_NAME") : "salama";
    std::string dbPass = std::getenv("S_DB_PASSWORD") ? std::getenv("S_DB_PASSWORD") : "";
    
    std::string connectionString = "dbname=" + dbName + " host=" + dbHost + " user=" + dbUser;
    if (!dbPass.empty()) {
        connectionString += " password=" + dbPass;
    }
    
    config["connection"] = connectionString;
    config["dev"] = true;
    
    // Set port (default 7070, can be overridden via env var)
    int port = 7090;
    if (const char* portEnv = std::getenv("SALAMA_BACKEND_PORT")) {
        port = std::stoi(portEnv);
    }
    
    json serveConfig;
    serveConfig["port"] = port;
    serveConfig["host"] = "127.0.0.1";
    config["serve"] = serveConfig;
    
    // Create app instance
    auto& app = MantisBase::create(config);
    
    // Get router reference
    Router& router = app.router();
    
    // Setup endpoints
    setupHealthEndpoint(router);
    setupAuthEndpoints(router, app);
    setupAllEndpoints(router, app);
    
    std::cout << "Salama POS Backend starting on port " << port << std::endl;
    
    // Start the server (blocking call)
    return app.run();
}
