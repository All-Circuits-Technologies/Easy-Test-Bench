// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbsqlqueryhandler.hpp"

#include <QtSql/QSqlQuery>


DbSqlQueryHandler::DbSqlQueryHandler(DbHandlerManager &handlerManager,
                                     const QString &connectionName,
                                     QObject *parent) :
    DbHandler(handlerManager, connectionName, parent)
{
    _sqlQuery = new QSqlQuery(accessDb());
}

DbSqlQueryHandler::~DbSqlQueryHandler()
{
    _sqlQuery->finish();
    delete _sqlQuery;
}
