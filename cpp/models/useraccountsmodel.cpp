#include "useraccountsmodel.h"
#include "logger.h"

#include <memory>

UserAccountsModel::UserAccountsModel(QObject *parent) 
    : QAbstractListModel(parent)
    , m_dateTime(new DateTime())
    , m_backendManager(nullptr)
{
    QFile file(":/json/logged_user.json");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString jsonString = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    m_loggedInUser = doc.object();

    QFile status_file(":/json/status.json");
    status_file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString jsonStatus = status_file.readAll();
    status_file.close();

    QJsonDocument docStatus = QJsonDocument::fromJson(jsonStatus.toUtf8());
    m_status = docStatus.object();
}

int UserAccountsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mUserAccounts.size();
}

QVariant UserAccountsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > mUserAccounts.count())
        return QVariant();

    UserAccounts * accounts = mUserAccounts.at(index.row());

    // shared/unique ptr

    if(role == UserFirstnameRole)
        return accounts->userFirstname();

    if(role == UserLastnameRole)
        return accounts->userLastname();

    if(role == UserUsernameRole)
        return accounts->userUsername();

    if(role == UserPhoneNoRole)
        return accounts->userPhoneNo();

    if(role == UserPasswordRole)
        return accounts->userPassword();

    if(role == UserDateAddedRole)
        return accounts->userDateAdded();

    if(role == CanAddUsersRole)
        return accounts->canAddUsers();

    if(role == CanRemoveUsersRole)
        return accounts->canRemoveUsers();

    if(role == CanAddItemsRole)
        return accounts->canAddItems();

    if(role == CanRemoveItemsRole)
        return accounts->canRemoveItems();

    if(role == CanAddStockRole)
        return accounts->canAddStock();

    if(role == CanRemoveStockRole)
        return accounts->canRemoveStock();

    if(role == CanUndoSalesRole)
        return accounts->canUndoSales();

    if(role == CanBackupDbRole)
        return accounts->canBackupDb();

    if(role == ChangePasswordRole)
        return accounts->changePassword();

    if(role == RolesStringRole)
        return accounts->rolesString();

    else return QVariant();
}

bool UserAccountsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    UserAccounts * accounts = mUserAccounts.at(index.row());
    bool changed = false;

    switch (role)
    {
    case UserFirstnameRole:
    {
        if( accounts->userFirstname() != value.toString())
        {
            accounts->setUserFirstname(value.toString());
            changed = true;
        }

        break;
    }

    case UserLastnameRole:
    {
        if( accounts->userLastname() != value.toString())
        {
            accounts->setUserLastname(value.toString());
            changed = true;
        }

        break;
    }

    case UserUsernameRole:
    {
        if( accounts->userUsername() != value.toString())
        {
            accounts->setUserUsername(value.toString());
            changed = true;
        }

        break;
    }

    case UserPhoneNoRole:
    {
        if( accounts->userPhoneNo() != value.toString())
        {
            accounts->setUserPhoneNo(value.toString());
            changed = true;
        }

        break;
    }

    case UserPasswordRole:
    {
        if( accounts->userPassword() != value.toString())
        {
            accounts->setUserPassword(value.toString());
            changed = true;
        }

        break;
    }


    case UserDateAddedRole:
    {
        if( accounts->userDateAdded() != value.toString())
        {
            accounts->setUserDateAdded(value.toString());
            changed = true;
        }

        break;
    }

    case CanAddUsersRole:
    {
        if( accounts->canAddUsers() != value.toBool())
        {
            accounts->setCanAddUsers(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveUsersRole:
    {
        if( accounts->canRemoveUsers() != value.toBool())
        {
            accounts->setCanRemoveUsers(value.toBool());
            changed = true;
        }

        break;
    }

    case CanAddItemsRole:
    {
        if( accounts->canAddItems() != value.toBool())
        {
            accounts->setCanAddItems(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveItemsRole:
    {
        if( accounts->canRemoveItems() != value.toBool())
        {
            accounts->setCanRemoveItems(value.toBool());
            changed = true;
        }

        break;
    }

    case CanAddStockRole:
    {
        if( accounts->canAddStock() != value.toBool())
        {
            accounts->setCanAddStock(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveStockRole:
    {
        if( accounts->canRemoveStock() != value.toBool())
        {
            accounts->setCanRemoveStock(value.toBool());
            changed = true;
        }

        break;
    }

    case CanUndoSalesRole:
    {
        if( accounts->canUndoSales() != value.toBool())
        {
            accounts->setCanUndoSales(value.toBool());
            changed = true;
        }

        break;
    }

    case CanBackupDbRole:
    {
        if( accounts->canBackupDb() != value.toBool())
        {
            accounts->setCanBackupDb(value.toBool());
            changed = true;
        }

        break;
    }

    case ChangePasswordRole:
    {
        if( accounts->changePassword() != value.toBool())
        {
            accounts->setChangePassword(value.toBool());
            changed = true;
        }

        break;
    }

    case RolesStringRole:
    {
        if( accounts->rolesString() != value.toString())
        {
            accounts->setRolesString(value.toString());
            changed = true;
        }

        break;
    }

    }

    if(changed)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags UserAccountsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> UserAccountsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[UserFirstnameRole] = "user_fname";
    roles[UserLastnameRole] = "user_lname";
    roles[UserUsernameRole] = "user_username";
    roles[UserPhoneNoRole] = "user_phoneNo";
    roles[UserPasswordRole] = "user_password";
    roles[UserDateAddedRole] = "date_added";
    roles[CanAddUsersRole] = "can_addUsers";
    roles[CanRemoveUsersRole] = "can_removeUsers";
    roles[CanAddItemsRole] = "can_addProducts";
    roles[CanRemoveItemsRole] = "can_removeProducts";
    roles[CanAddStockRole] = "can_addStock";
    roles[CanRemoveStockRole] = "can_removeStock";
    roles[CanUndoSalesRole] = "can_undoSales";
    roles[CanBackupDbRole] = "can_backup";
    roles[ChangePasswordRole] = "to_changePassword";
    roles[RolesStringRole] = "user_roles";

    return roles;
}

void UserAccountsModel::addNewUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPassword, const QVariant &userPhoneNo, const QVariant &userDateAdded)
{
    emit logDataChanged("INFO", "Starting AddNewUserAccount()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userAddedChanged(false);
        return;
    }

    if(getUserIndex(userUsername.toString()) != -1)
    {
        emit usernameExistsChanged(true);
        emit logDataChanged("WARNING", "Username entered exists in the database");
        return;
    }

    QString dateToday = userDateAdded.toString().isEmpty() ? m_dateTime->getTimestamp("now").at(0) : userDateAdded.toString();
    
    QJsonObject data;
    data["firstname"] = userFirstname.toString();
    data["lastname"] = userLastname.toString();
    data["username"] = userUsername.toString();
    data["password"] = userPassword.toString(); // Backend will hash it
    data["phone_no"] = userPhoneNo.toString();
    data["date_added"] = dateToday;
    
    m_pendingEndpoint = "/api/v1/salamapos/users";
    m_backendManager->postRequest(m_pendingEndpoint, data);

    emit logDataChanged("INFO", "Ending AddNewUserAccount()");
}

void UserAccountsModel::updatePassword(const QVariant &userUsername, const QVariant &passwordOld, const QVariant &passwordNew)
{
    emit logDataChanged("INFO", "Starting updatePassword()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userPasswordChanged(false);
        return;
    }

    QJsonObject data;
    data["oldPassword"] = passwordOld.toString();
    data["newPassword"] = passwordNew.toString();
    
    QString endpoint = QString("/api/v1/salamapos/users/%1/password").arg(userUsername.toString());
    m_pendingEndpoint = endpoint;
    m_backendManager->putRequest(endpoint, data);

    emit logDataChanged("INFO", "Ending updatePassword()");
}

void UserAccountsModel::removeUserAccount(const QVariant &userUsername, QVariant index)
{
    emit logDataChanged("INFO", "Starting removeUserAccount()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userRemovedChanged(false);
        return;
    }

    QString endpoint = QString("/api/v1/salamapos/users/%1").arg(userUsername.toString());
    m_pendingEndpoint = endpoint;
    m_backendManager->deleteRequest(endpoint);

    emit logDataChanged("INFO", "Ending removeUserAccount()");
}

void UserAccountsModel::updateUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPhoneNo, const QVariant &orig_username)
{
    emit logDataChanged("INFO", "Starting updateUserAccount() -> Account Details");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userUpdatedChanged(false);
        return;
    }

    if(userUsername != orig_username && getUserIndex(userUsername.toString()) != -1) {
        emit usernameExistsChanged(true);
        return;
    }

    QJsonObject data;
    data["firstname"] = userFirstname.toString();
    data["lastname"] = userLastname.toString();
    data["username"] = userUsername.toString();
    data["phone_no"] = userPhoneNo.toString();
    data["orig_username"] = orig_username.toString();
    
    QString endpoint = QString("/api/v1/salamapos/users/%1").arg(userUsername.toString());
    m_pendingEndpoint = endpoint;
    m_backendManager->putRequest(endpoint, data);

                        emit logged_inUserChanged();
                    }

                }

                else
                {
                    m_db.rollback();

                    emit userUpdatedChanged(false);

                    auto errStr = "Error executing SQL: " + query.lastError().text();

                    // qDebug() << errStr;

                    emit logDataChanged("CRITICAL", errStr);
                }
            }

            else
                Logger::logWarning("User account index not found");

        else
            Logger::logWarning("User account index not found");
    }

    emit logDataChanged("INFO", "Ending updateUserAccount() -> Account Details");
}

void UserAccountsModel::updateUserAccount(const QVariant &userUsername, const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb)
{
    emit logDataChanged("INFO", "Starting updateUserAccount() -> Account Priviledges");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userPriviledgesChanged(false);
        return;
    }

    int index_ = getUserIndex(userUsername.toString());
    if(index_ == -1) {
        emit userPriviledgesChanged(false);
        return;
    }

    QJsonObject data;
    data["canAddUser"] = canAddUsers;
    data["canRemoveUsers"] = canRemoveUsers;
    data["canAddItems"] = canAddItems;
    data["canRemoveItems"] = canRemoveItems;
    data["canAddStock"] = canAddStock;
    data["canRemoveStock"] = canRemoveStock;
    data["canUndoSales"] = canUndoSales;
    data["canBackupDb"] = canBackupDb;
    
    QString endpoint = QString("/api/v1/salamapos/users/%1/privileges").arg(userUsername.toString());
    m_pendingEndpoint = endpoint;
    m_backendManager->putRequest(endpoint, data);

    emit logDataChanged("INFO", "Ending updateUserAccount() -> Account Priviledges");
}

void UserAccountsModel::markAccountForDeleting(const QVariant &userUsername)
{
    emit logDataChanged("INFO", "Starting markAccountForDeleting()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit toDeleteAccountChanged(false);
        return;
    }

    int index = getUserIndex(userUsername.toString());
    if(index == -1) {
        emit logDataChanged("WARNING", "User index could not be found");
        emit toDeleteAccountChanged(false);
        return;
    }

    QString endpoint = QString("/api/v1/salamapos/users/%1/mark-delete").arg(userUsername.toString());
    m_pendingEndpoint = endpoint;
    m_backendManager->putRequest(endpoint, QJsonObject());

    emit logDataChanged("INFO", "Ending markAccountForDeleting()");
}

void UserAccountsModel::loadAllUserAccounts()
{
    emit logDataChanged("INFO", "Starting loadAllUserAccounts()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit userAccountsLoaded(false);
        return;
    }

    m_pendingEndpoint = "/api/v1/salamapos/users";
    m_backendManager->getRequest(m_pendingEndpoint);
}

void UserAccountsModel::onApiResponse(const QString &endpoint, const QJsonObject &response)
{
    if (endpoint == "/api/v1/salamapos/users" && m_pendingEndpoint == endpoint) {
        // GET request - load all users
        if (response.value("success").toBool()) {
            QJsonArray users = response.value("users").toArray();
            
            beginResetModel();
            mUserAccounts.clear();
            
            for (const QJsonValue &value : users) {
                QJsonObject user = value.toObject();
                QString firstname = user.value("firstname").toString();
                QString lastname = user.value("lastname").toString();
                QString username = user.value("username").toString();
                QString password = user.value("password").toString();
                QString phone_no = user.value("phone_no").toString();
                QString date_added = user.value("date_added").toString();
                bool canAddUsers = user.value("canAddUser").toBool();
                bool canRemoveUsers = user.value("canRemoveUsers").toBool();
                bool canAddItems = user.value("canAddItems").toBool();
                bool canRemoveItems = user.value("canRemoveItems").toBool();
                bool canAddStock = user.value("canAddStock").toBool();
                bool canRemoveStock = user.value("canRemoveStock").toBool();
                bool canUndoSales = user.value("canUndoSales").toBool();
                bool canBackupDb = user.value("canBackupDb").toBool();
                bool changePassword = user.value("changePassword").toBool();
                
                QString role = getUserRoleAsAString(canAddUsers, canRemoveUsers, canAddItems, 
                    canRemoveItems, canAddStock, canRemoveStock, canUndoSales, canBackupDb);
                
                addNewUserAccount(new UserAccounts(firstname, lastname, username, phone_no, 
                    password, date_added, canAddUsers, canRemoveUsers, canAddItems, 
                    canRemoveItems, canAddStock, canRemoveStock, canUndoSales, 
                    canBackupDb, changePassword, role));
            }
            
            endResetModel();
            emit userAccountsLoaded(true);
            emit logDataChanged("INFO", "Users loaded successfully");
        } else {
            emit userAccountsLoaded(false);
            emit logDataChanged("ERROR", response.value("error").toString());
        }
    } else if (m_pendingEndpoint == "/api/v1/salamapos/users") {
        // POST request - create user
        if (response.value("success").toBool()) {
            emit userAddedChanged(true);
            emit logDataChanged("INFO", "User added successfully");
            loadAllUserAccounts(); // Reload list
        } else {
            if (response.value("error").toString().contains("already exists")) {
                emit usernameExistsChanged(true);
            }
            emit userAddedChanged(false);
            emit logDataChanged("ERROR", response.value("error").toString());
        }
    } else if (m_pendingEndpoint.contains("/password")) {
        // Password update
        if (response.value("success").toBool()) {
            emit userPasswordChanged(true);
            emit logDataChanged("INFO", "Password updated successfully");
        } else {
            emit userPasswordChanged(false);
            emit userPasswordChangeError(response.value("error").toString());
        }
    } else if (m_pendingEndpoint.contains("/privileges")) {
        // Privileges update
        if (response.value("success").toBool()) {
            emit userPriviledgesChanged(true);
            emit logDataChanged("INFO", "Privileges updated successfully");
            loadAllUserAccounts(); // Reload to get updated data
        } else {
            emit userPriviledgesChanged(false);
            emit logDataChanged("ERROR", response.value("error").toString());
        }
    } else if (m_pendingEndpoint.contains("/mark-delete")) {
        // Mark for deletion
        if (response.value("success").toBool()) {
            emit toDeleteAccountChanged(true);
            emit logDataChanged("INFO", "Account marked for deletion");
            loadAllUserAccounts(); // Reload list
        } else {
            emit toDeleteAccountChanged(false);
            emit logDataChanged("ERROR", response.value("error").toString());
        }
    } else if (m_pendingEndpoint.contains("/users/") && !m_pendingEndpoint.contains("/password") && !m_pendingEndpoint.contains("/privileges")) {
        // User update or delete
        if (response.value("success").toBool()) {
            if (m_pendingEndpoint.contains("DELETE") || response.value("message").toString().contains("deleted")) {
                emit userRemovedChanged(true);
                emit logDataChanged("INFO", "User removed successfully");
                loadAllUserAccounts(); // Reload list
            } else {
                emit userUpdatedChanged(true);
                emit logDataChanged("INFO", "User updated successfully");
                loadAllUserAccounts(); // Reload list
            }
        } else {
            emit userUpdatedChanged(false);
            emit userRemovedChanged(false);
            emit logDataChanged("ERROR", response.value("error").toString());
        }
    }
}

void UserAccountsModel::onApiError(const QString &error)
{
    emit logDataChanged("ERROR", error);
    if (m_pendingEndpoint == "/api/v1/salamapos/users") {
        emit userAccountsLoaded(false);
    }
}

void UserAccountsModel::onLoginResponse(const QJsonObject &response)
{
    if (response.value("success").toBool()) {
        QJsonObject user = response.value("user").toObject();
        setLoggedInUser(user);
        emit logged_inUserChanged();
        emit loggingInUsernameStatus(true);
        emit loggingInPasswordStatus(true);
        emit logDataChanged("INFO", "Login successful");
    } else {
        emit loggingInUsernameStatus(false);
        emit loggingInPasswordStatus(false);
        emit logDataChanged("ERROR", response.value("error").toString());
    }
}

void UserAccountsModel::loginUser(const QVariant &uname, const QVariant &pswd)
{
    emit logDataChanged("INFO", "Starting loginUser()");

    if (!m_backendManager) {
        emit logDataChanged("ERROR", "BackendManager not available");
        emit loggingInUsernameStatus(false);
        emit loggingInPasswordStatus(false);
        return;
    }

    m_backendManager->login(uname.toString(), pswd.toString());
    
    // Response will be handled in onLoginResponse slot
}
            docObj["canRemoveStock"] = data(this->index(ind), CanRemoveStockRole).toBool();
            docObj["canUndoSales"] = data(this->index(ind), CanUndoSalesRole).toBool();
            docObj["canBackupDb"] = data(this->index(ind), CanBackupDbRole).toBool();

            setLoggedInUser(docObj);

            emit logged_inUserChanged();
        }

        else
        {
            emit loggingInPasswordStatus(false);

            // qDebug() << ">> Wrong User password";
        }
    }

    emit logDataChanged("INFO", "Ending loginUser()");
}

QJsonObject UserAccountsModel::getUpdatedPriviledges(int index)
{
    emit logDataChanged("INFO", "Starting getUpdatedPriviledges()");

    if(index < mUserAccounts.size())
    {
        m_status["canAddUser"] = data(this->index(index), CanAddUsersRole).toBool();
        m_status["canRemoveUsers"] = data(this->index(index), CanRemoveUsersRole).toBool();
        m_status["canAddItems"] = data(this->index(index), CanAddItemsRole).toBool();
        m_status["canRemoveItems"] = data(this->index(index), CanRemoveItemsRole).toBool();
        m_status["canAddStock"] = data(this->index(index), CanAddStockRole).toBool();
        m_status["canRemoveStock"] = data(this->index(index), CanRemoveStockRole).toBool();
        m_status["canUndoSales"] = data(this->index(index), CanUndoSalesRole).toBool();
        m_status["canBackupDb"] = data(this->index(index), CanBackupDbRole).toBool();

        return m_status;
    }

    // qDebug() << " [ERROR] Invalid User Account index...";

    emit logDataChanged("WARNING", "Invalid User Account index");

    emit logDataChanged("INFO", "Ending getUpdatedPriviledges()");

    return QJsonObject();
}

void UserAccountsModel::addNewUserAccount(UserAccounts *user)
{
    const int index = mUserAccounts.size();

    beginInsertRows(QModelIndex(), index, index);
    mUserAccounts.append(user);
    endInsertRows();
}

void UserAccountsModel::removeUserAccount(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    mUserAccounts.removeAt(index);
    endRemoveRows();
}

QString UserAccountsModel::hashPassword(const QString &pswd)
{
    QString salt = QString::number(QDateTime::currentSecsSinceEpoch());
    Logger::logDebug("Password hash: Salt generated", salt);
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    hash.addData(pswd.toUtf8() + salt.toUtf8());
    auto hashedPassword = hash.result().toHex()+":"+salt;
    Logger::logDebug("Password hash: Final hash generated");
    return hashedPassword;
}

QString UserAccountsModel::hashPassword(const QString &pswd, const QString &salt)
{
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    hash.addData(pswd.toUtf8() + salt.toUtf8());
    auto hashedPassword = hash.result().toHex()+":"+salt;
    Logger::logDebug("Password hash: Final hash generated");
    return hashedPassword;
}

bool UserAccountsModel::login(const QString &savedPswd, const QString &inputPswd)
{
    try {
        QString _pswd = savedPswd.split(":").at(0);
        QString _salt = savedPswd.split(":").at(1);

        QCryptographicHash hash(QCryptographicHash::Sha3_256);
        hash.addData(inputPswd.toUtf8() + _salt.toUtf8());

        if(hash.result().toHex() == _pswd)
            return true;

        else
            return false;
    } catch(char e) {
        return false;
    }
}

int UserAccountsModel::getUserIndex(const QString &username)
{
    QString _username;

    for(int i=0; i<mUserAccounts.size(); i++)
    {
        _username = data(this->index(i), UserUsernameRole).toString();

        if(username == _username)
            return i;
    }

    return -1;
}

QString UserAccountsModel::stringifyBool(const bool &state)
{
    if(state)
        return "true";

    return "false";
}

QString UserAccountsModel::getUserRoleAsAString(const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb)
{
    QString _string;

    _string += canAddUsers? "Can create accounts, ":"";
    _string += canRemoveUsers? "Can delete accounts, ":"";
    _string += canAddItems? "Can add products, ":"";
    _string += canRemoveItems? "Can delete products, ":"";
    _string += canAddStock? "Can add stock, ":"";
    _string += canRemoveStock? "Can delete stock, ":"";
    _string += canUndoSales? "Can delete sales, ":"";
    _string += canBackupDb? "Can backup database": "";

    if(_string.toStdString()[_string.length()-2] == ',')
        _string = _string.left(_string.length()-2);

    if(_string == "")
        _string = "---";

    return _string;
}

QJsonObject UserAccountsModel::loggedInUser() const
{
    return m_loggedInUser;
}

void UserAccountsModel::setLoggedInUser(QJsonObject loggedInUser)
{
    if (m_loggedInUser == loggedInUser)
        return;

    m_loggedInUser = loggedInUser;
    emit loggedInUserChanged(m_loggedInUser);
}
