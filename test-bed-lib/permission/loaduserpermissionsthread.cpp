// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "database/dbpermissionasker.hpp"

#include "permission/loaduserpermissionsthread.hpp"
#include "permission/permissionmanager.hpp"


LoadUserPermissionsThread::LoadUserPermissionsThread(
        const DbPermissionAsker &dbPermissionAsker,
        QVector<QSharedPointer<Permission>> &userPermissions,
        const QSharedPointer<User> &user,
        QObject *parent):
        QThread(parent),
        _user(user),
        _userPermissions(userPermissions),
        _dbPermissionAsker(dbPermissionAsker)
{
}

void LoadUserPermissionsThread::run()
{
    emit loadingStatusChanged(LoadingStatus::Enum::Started);

    if(_dbPermissionAsker.getAllUserPermissions(_user, _userPermissions))
    {
        emit loadingStatusChanged(LoadingStatus::Enum::Finished);
    }
    else
    {
        emit loadingStatusChanged(LoadingStatus::Enum::Failed);
    }
}
