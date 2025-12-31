#ifndef USERACCOUNTSMODEL_H
#define USERACCOUNTSMODEL_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QAbstractListModel>
#include "src/useraccounts.h"
#include "../datetime.h"

class BackendManager;

class UserAccountsModel : public QAbstractListModel
{
    Q_OBJECT

    enum UserAccountsRoles {
        UserFirstnameRole = Qt::UserRole+1,
        UserLastnameRole,
        UserUsernameRole,
        UserPhoneNoRole,
        UserPasswordRole,
        UserDateAddedRole,
        CanAddUsersRole,
        CanRemoveUsersRole,
        CanAddItemsRole,
        CanRemoveItemsRole,
        CanAddStockRole,
        CanRemoveStockRole,
        CanUndoSalesRole,
        CanBackupDbRole,
        ChangePasswordRole,
        RolesStringRole
    };

public:
    explicit UserAccountsModel(QObject *parent = nullptr);
    explicit UserAccountsModel(BackendManager *backendManager, QObject *parent = nullptr);

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QHash<int, QByteArray> roleNames() const;

    // Q_INVOKABLES

    Q_INVOKABLE void addNewUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPassword, const QVariant &userPhoneNo, const QVariant &userDateAdded = "");

    Q_INVOKABLE void updatePassword(const QVariant &userUsername, const QVariant&, const QVariant&);

    Q_INVOKABLE void removeUserAccount(const QVariant &userUsername, QVariant index);

    Q_INVOKABLE void updateUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPhoneNo, const QVariant &orig_username);

    Q_INVOKABLE void updateUserAccount(const QVariant &userUsername, const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb);

    Q_INVOKABLE void markAccountForDeleting(const QVariant &userUsername);

    Q_INVOKABLE void loadAllUserAccounts();

    Q_INVOKABLE void loginUser(const QVariant &uname, const QVariant &pswd);

    Q_INVOKABLE QJsonObject getUpdatedPriviledges(int index);

    // User Properties
    Q_PROPERTY(QJsonObject loggedInUser READ loggedInUser WRITE setLoggedInUser NOTIFY loggedInUserChanged)

    // Q_PROPERTY(QString priviledgesString READ priviledgesString WRITE setPriviledgesString NOTIFY priviledgesStringChanged)

    // Internals
    void addNewUserAccount(UserAccounts * user);

    void removeUserAccount(int index);

    QString hashPassword(const QString &pswd);

    QString hashPassword(const QString &pswd, const QString &salt);

    bool login(const QString &savedPswd, const QString &inputPswd);

    int getUserIndex(const QString &username);

    QString stringifyBool(const bool &state);

    QString getUserRoleAsAString(const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb);

    // Properties
    QJsonObject loggedInUser() const;

    void setLoggedInUser(QJsonObject loggedInUser);

signals:
    void userAddedChanged(bool status);

    void userRemovedChanged(bool status);

    void userUpdatedChanged(bool status);

    void userPriviledgesChanged(bool status);

    void userPasswordChanged(bool status);

    void userPasswordChangeError(const QString& reason);

    void toDeleteAccountChanged(bool status);

    void loggedInUserChanged(QJsonObject loggedInUser);

    void logged_inUserChanged();

    void loggingInUsernameStatus(bool status);

    void loggingInPasswordStatus(bool status);

    void userAccountsLoaded(bool status);

    void usernameExistsChanged(bool status);

    void logDataChanged(QString level, QString info);

private slots:
    void onApiResponse(const QString &endpoint, const QJsonObject &response);
    void onApiError(const QString &error);
    void onLoginResponse(const QJsonObject &response);

private:
    QList<UserAccounts *> mUserAccounts;

    QJsonObject m_loggedInUser, m_status;

    DateTime * m_dateTime;
    BackendManager *m_backendManager;
    QString m_pendingEndpoint;
};

#endif // USERACCOUNTSMODEL_H
