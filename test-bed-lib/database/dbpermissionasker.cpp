// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "database/dbpermissionasker.hpp"

#include "database/dbmanager.hpp"
#include "models/user.hpp"
#include "models/permission.hpp"


DbPermissionAsker::DbPermissionAsker(DbManager &dbManager,QObject* parent):
    QObject(parent),
    _dbManager(dbManager)
{
}

bool DbPermissionAsker::getAllUserPermissions(const QSharedPointer<User> &user,
                                              QVector<QSharedPointer<Permission>> &permissions)
                                              const
{
    QString query(selectUserPermissions);
    query = query.arg(user->getRoles().at(0));
    if(!_dbManager.select<Permission>(query, permissions))
    {
        qWarning() << "Can't load user permissions";
        return false;
    }

    return true;
}
