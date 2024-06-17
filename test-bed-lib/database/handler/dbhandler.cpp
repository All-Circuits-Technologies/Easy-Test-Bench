// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbhandler.hpp"

#include "database/handler/dbhandlerkey.hpp"
#include "database/handler/dbhandlermanager.hpp"

#include <QtSql/QSqlQuery>


DbHandler::DbHandler(DbHandlerManager &handlerManager,
                     const QString &connectionName,
                     QObject *parent) :
    QObject(parent),
    _handlerManager(handlerManager),
    _connectionName(connectionName)
{
    _handlerManager.incrementHandlerNb(DbHandlerKey(), connectionName);
}

DbHandler::~DbHandler()
{
    _handlerManager.decrementHandlerNb(DbHandlerKey(), _connectionName);
}

QSqlDatabase &DbHandler::accessDb()
{
    if(!_db.isValid())
    {
        _db = QSqlDatabase::database();
    }

    return _db;
}
