// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "authenticationmanager.hpp"

#include "database/dbmanager.hpp"
#include "database/dbpermissionasker.hpp"
#include "models/permission.hpp"
#include "permission/loaduserpermissionsthread.hpp"
#include "permission/permissionmanager.hpp"


PermissionManager::PermissionManager(
        DbManager &dbManager,
        AuthenticationManager &authenticationManager,
        QObject *parent):
        QObject(parent),
        _dbPermissionAsker(new DbPermissionAsker(dbManager, this))
{
    connect(&authenticationManager,
            &AuthenticationManager::userConnected,
            this,
            &PermissionManager::onUserConnected);

    connect(&authenticationManager,
            &AuthenticationManager::userDisconnected,
            this,
            &PermissionManager::onUserDisconnected);
}

PermissionManager::~PermissionManager()
{
}

bool PermissionManager::hasPermission(const QString &permissionName) const
{

    for(auto citer = _userPermissions.cbegin(); citer != _userPermissions.cend(); ++citer)
    {
        if(permissionName == (*citer)->getName())
        {
            return true;
        }
    }

    return false;
}

bool PermissionManager::loadUserPermissions(const QSharedPointer<User> &user)
{
    LoadUserPermissionsThread *loadUserPermissionsThread =
            new LoadUserPermissionsThread(*_dbPermissionAsker, _userPermissions, user);

    connect(loadUserPermissionsThread,
            &LoadUserPermissionsThread::loadingStatusChanged,
            this,
            &PermissionManager::loadingStatusChanged);

    connect(loadUserPermissionsThread,
            &LoadUserPermissionsThread::finished,
            loadUserPermissionsThread,
            &LoadUserPermissionsThread::deleteLater);

    loadUserPermissionsThread->start();

    return true;
}

void PermissionManager::onUserConnected(const QSharedPointer<User> &user)
{
    loadUserPermissions(user);
}

void PermissionManager::onUserDisconnected()
{
    _userPermissions.clear();
}

void PermissionManager::RegisterMetaTypes()
{
    LoadingStatus::RegisterMetaType();
}
