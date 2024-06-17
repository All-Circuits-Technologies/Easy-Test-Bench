// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbuserasker.hpp"

#include "models/user.hpp"


DbUserAsker::DbUserAsker(DbManager& dbManager, QObject *parent) :
    QObject(parent),
    _dbManager(dbManager)
{}

bool DbUserAsker::addUser(const QSharedPointer<User> &user)
{
    return  _dbManager.insert(user);
}

bool DbUserAsker::getUserByUsername(const QString &username, QSharedPointer<User> &user) const
{
    QVector<QSharedPointer<User>> users;
    QString query(selectUserByName);
    query = query.arg(username);

    if(!_dbManager.select<User>(query, users))
    {
        qWarning() << "Can't get User " + username + " from the database";
        return false;
    }

    if(users.isEmpty())
    {
        qWarning() << "No user found with username : " + username;
        return false;
    }
    user = users.at(0);

    return true;
}

bool DbUserAsker::deleteUser(const QSharedPointer<User> &user)
{
    return _dbManager.remove(user);
}

bool DbUserAsker::updateUser(const QSharedPointer<User> &user)
{
    return _dbManager.update(user);
}

bool DbUserAsker::getAllUsers(QVector<QSharedPointer<User>> &users) const
{
    if(!_dbManager.select<User>(users))
    {
        qWarning() << "Can't get All users from the database";
        return false;
    }

    return true;
}
