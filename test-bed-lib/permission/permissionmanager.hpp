// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "permission/loadingstatus.hpp"
#include "testbedcore/testbedglobal.hpp"

class AuthenticationManager;
class DbPermissionAsker;
class DbManager;
class Permission;
class User;


/** @brief Manage permissions of the current user */
class TESTBEDLIB_DLLSPEC PermissionManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbManager A DbManager reference
            @param authenticationManager An AuthenticationManager reference
            @param parent The parent class */
        explicit PermissionManager(DbManager &dbManager,
                                   AuthenticationManager &authenticationManager,
                                   QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~PermissionManager() override;

    public:
        /** @brief Check if the current user has a permission
            @param permissionName The name of the permission
            @return True if success, false if failure */
        bool hasPermission(const QString &permissionName) const;

        /** @brief Asynchronously load user permissions
            @param user The user
            @return True if success, false if failure */
        bool loadUserPermissions(const QSharedPointer<User> &user);

    public:
        /** @brief Call all the known meta and event types to register */
        static void RegisterMetaTypes();

    signals:
        /** @brief Emit when permissions loading status changed
            @param loadingStatus Status of the loading */
        void loadingStatusChanged(LoadingStatus::Enum loadingStatus);

    public slots:
        /** @brief Called when the user is connected
            @param user The User connected*/
        void onUserConnected(const QSharedPointer<User> &user);

        /** @brief Called when the user is disconnected */
        void onUserDisconnected();

    private :
        QVector<QSharedPointer<Permission>> _userPermissions;
        DbPermissionAsker *_dbPermissionAsker{nullptr};
};
