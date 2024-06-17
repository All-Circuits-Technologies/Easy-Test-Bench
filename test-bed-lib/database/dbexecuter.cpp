// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbexecuter.hpp"

#include "database/handler/dbhandlermanager.hpp"
#include "database/handler/dbhandler.hpp"
#include "database/handler/dbsqlqueryhandler.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVersionNumber>


DbExecuter::DbExecuter(QObject *parent) :
    QObject(parent),
    _handlerManager(new DbHandlerManager(this))
{
}

DbExecuter::~DbExecuter()
{
}

bool DbExecuter::initDbManager(const QString &path)
{
    QFileInfo databaseFileInfo(path);

    const QString driver(DriverName);
    if(!QSqlDatabase::isDriverAvailable(driver))
    {
        qWarning() << "Database drivers are not available";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(driver);

    if(!QDir(databaseFileInfo.path()).exists() &&
       !QDir().mkdir(databaseFileInfo.path()))
    {
        qWarning() << "Impossible to access the database path : " + path;
        return false;
    }

    db.setDatabaseName(path);

    if(!db.open())
    {
        qWarning() << "DB INIT ERROR: " << db.lastError();
        return false;
    }

    // This will fire the inactive timeout
    _handlerManager->pingDb();

    return true;
}

bool DbExecuter::exec(const QString &sqlQuery)
{
    DbHandler handler(*_handlerManager);
    QSqlQuery query(handler.accessDb());

    if(!handler.accessDb().transaction())
    {
        qWarning() << "EXEC: Failed to start transaction mode";
        return false;
    }

    if(!query.exec(sqlQuery))
    {
        qWarning() << "DB EXEC ERROR: " << query.lastError().text();
        handler.accessDb().rollback();
        return false;
    }

    if(!handler.accessDb().commit())
    {
        qWarning() << "DB EXEC: Failed to commit";
        handler.accessDb().rollback();
        return false;
    }

    return true;
}

bool DbExecuter::exec(const QString &sqlQuery,
                      const QList<QString> &placeholderNames,
                      const QList<QVariant> &values)
{
    if(placeholderNames.length() != values.length())
    {
        qWarning() << "Placeholder names list and values list haven't the same size, can't exec "
                   << "query";
        return false;
    }

    DbHandler handler(*_handlerManager);
    QSqlQuery query(handler.accessDb());

    if(!query.prepare(sqlQuery))
    {
        qWarning() << "Preparing sql query failed: " << sqlQuery << ", error: "
                   << query.lastError();
        return false;
    }

    for(int idx = 0; idx < placeholderNames.length(); ++idx)
    {
        query.bindValue(placeholderNames.at(idx), values.at(idx));
    }

    if(!handler.accessDb().transaction())
    {
        qWarning() << "EXEC WITH PLACEHOLDERS: Failed to start transaction mode";
        return false;
    }

    if(!query.exec())
    {
        qWarning() << "DB EXEC WITH PLACEHOLDERS ERROR: " << query.lastError().text();
        handler.accessDb().rollback();
        return false;
    }

    if(!handler.accessDb().commit())
    {
        qWarning() << "DB EXEC WITH PLACEHOLDERS: Failed to commit";
        handler.accessDb().rollback();
        return false;
    }

    return true;
}

DbSqlQueryHandler *DbExecuter::execSelect(const QString &sqlQuery) const
{
    DbSqlQueryHandler *queryHandler = new DbSqlQueryHandler(*_handlerManager);

    if(!queryHandler->accessQuery().prepare(sqlQuery))
    {
        qWarning() << "A problem occurred when tryied to prepare the SQL query: " << sqlQuery
                   << ", error: " << queryHandler->accessQuery().lastError();
        delete queryHandler;
        return nullptr;
    }

    if(!queryHandler->accessQuery().exec())
    {
        qWarning() << "ERROR SELECT ALL : " << queryHandler->accessQuery().lastError().text();
        delete queryHandler;
        return nullptr;
    }

    return queryHandler;
}

bool DbExecuter::executeSqlScriptFile(const QString &filePath)
{
    QFile sqlScriptFile(filePath);
    if (!sqlScriptFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Can't open SQL script file : " + filePath;
        return  false;
    }

    QTextStream textStream(&sqlScriptFile);
    QString sql = textStream.readAll();
    QStringList sqlStatements = sql.split(ScriptSeparator, Qt::SkipEmptyParts);

    DbHandler handler(*_handlerManager);
    if(!handler.accessDb().transaction())
    {
        qWarning() << "Failed to start transaction mode, tried to execute SQL script file";
        return false;
    }

    for(QString &statement : sqlStatements)
    {
        const QString trimmed = statement.trimmed();
        if (!trimmed.isEmpty())
        {
            QSqlQuery query(handler.accessDb());
            if (!query.exec(statement))
            {
                qWarning() << "DB scripts execution failed: " << trimmed
                           << ", reason: " << query.lastError();
                handler.accessDb().rollback();
                return false;
            }
        }
    }

    if(!handler.accessDb().commit())
    {
        qWarning() << "Failed to commit, tried to execute SQL script file";
        handler.accessDb().rollback();
        return false;
    }

    return true;
}
