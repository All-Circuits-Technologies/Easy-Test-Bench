// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "authenticationmanager.hpp"

#include <QCryptographicHash>

#include "database/dbuserasker.hpp"
#include "definesutility/definesutility.hpp"
#include "models/user.hpp"


AuthenticationManager::AuthenticationManager(DbManager &dbManager, QObject *parent):
    QObject(parent),
    _dbUserAsker(new DbUserAsker(dbManager, this))
{
}

AuthenticationManager::~AuthenticationManager()
{
}

bool AuthenticationManager::loginDefaultUser()
{
    return _dbUserAsker->getUserByUsername(defaultUsername, _currentUser);
}

bool AuthenticationManager::login(const QString &username,
                                  const QString &clearTextPassword,
                                  bool &idsAreCorrect)
{
    idsAreCorrect = false;
    QSharedPointer<User> user;

    RETURN_IF_FALSE(_dbUserAsker->getUserByUsername(username, user));

    if(!user.isNull())
    {
        QString hashedPassword = saltAndHashPassword(clearTextPassword, user->getSalt());

        if(hashedPassword == user->getPassword())
        {
            idsAreCorrect = true;
            _currentUser = user;
            emit userConnected(_currentUser);
        }
    }

    return true;
}

QString AuthenticationManager::saltAndHashPassword(const QString &clearTextPassword,
                                                   const QString &salt)
{
    QString password(clearTextPassword + salt);
    password = QString(QCryptographicHash::hash((clearTextPassword.toUtf8()),
                                                QCryptographicHash::Sha512).toHex());
    return password;
}

void AuthenticationManager::logout()
{
    _currentUser = nullptr;
    emit userDisconnected();
}
