// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "models/user.hpp"

#include <QRandomGenerator>

#include "permission/authenticationmanager.hpp"


User::User(const QString &firstName,
           const QString &lastName,
           const QString &username,
           const QString &password,
           const QString &salt,
           const QVector<QString> &roles,
           QObject *parent) :
           DbObject(parent),
           _firstName(firstName),
           _lastName(lastName),
           _username(username),
           _password(password),
           _salt(salt),
           _roles(roles)
{
}

User::User(const QString &firstName,
           const QString &lastName,
           const QString &username,
           const QString &clearTextPassword,
           const QVector<QString> &roles,
           QObject *parent):
           DbObject(parent),
           _firstName(firstName),
           _lastName(lastName),
           _username(username),
           _roles(roles)
{
    _salt = QString::number(QRandomGenerator::securelySeeded().generate());
    _password = AuthenticationManager::saltAndHashPassword(clearTextPassword, _salt);
}

User::User(QObject *parent) : DbObject(parent)
{
}

QString User::getSerializedRoles()
{
    QString rolesToString;
    for(int idx = 0; idx < _roles.size(); idx++)
    {
        rolesToString += _roles.at(idx);

        if(idx < _roles.size() - 1)
        {
            rolesToString += ",";
        }
    }

    return rolesToString;
}

void User::setSerializedRoles(const QString &serializedRoles)
{
    _roles = serializedRoles.split(",", Qt::SkipEmptyParts).toVector();
}
