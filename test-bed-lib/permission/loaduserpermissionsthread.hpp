// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "loadingstatus.hpp"

#include <QThread>

#include "testbedcore/testbedglobal.hpp"

class DbPermissionAsker;
class Permission;
class User;


/** @brief Allows to load a user's permissions asynchronously */
class TESTBEDLIB_DLLSPEC LoadUserPermissionsThread : public QThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbPermissionAsker A DbPermissionAsker reference
            @param userPermissions Vector of permissions which will be loaded
            @param user The User
            @param parent Optional Qt parentship*/
        LoadUserPermissionsThread(
                const DbPermissionAsker &dbPermissionAsker,
                QVector<QSharedPointer<Permission>> &userPermissions,
                const QSharedPointer<User> &user,
                QObject *parent = nullptr);

    signals:
        /** @brief Emit when permissions loading status changed
            @param loadingStatus Status of the loading */
        void loadingStatusChanged(LoadingStatus::Enum loadingStatus);

    protected:
        /** @brief Start the thread */
        void run() override;

    private:
        const QSharedPointer<User> &_user;
        QVector<QSharedPointer<Permission>> &_userPermissions;
        const DbPermissionAsker &_dbPermissionAsker;
};
