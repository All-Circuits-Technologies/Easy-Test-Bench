// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbthread.hpp"

#include "database/dbexecuter.hpp"
#include "database/handler/dbsqlqueryhandler.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include <QTimer>


DbThread::DbThread(QObject *parent) :
    BaseThread(parent)
{
}

DbThread::~DbThread()
{
}

bool DbThread::initDbManager(const QString &path)
{
    RETURN_IF_FALSE(startThreadAndWaitToBeReady());

    return ThreadConcurrentRun::run(*_dbExecuter, &DbExecuter::initDbManager, path);
}

bool DbThread::exec(const QString &sqlQuery)
{
    return ThreadConcurrentRun::run(*_dbExecuter, &DbExecuter::exec, sqlQuery);
}

bool DbThread::exec(const QString &sqlQuery,
                    const QList<QString> &placeholderNames,
                    const QList<QVariant> &values)
{
    return ThreadConcurrentRun::run(*_dbExecuter,
                                    &DbExecuter::exec,
                                    sqlQuery,
                                    placeholderNames,
                                    values);
}

bool DbThread::execSelect(const QString &sqlQuery, QScopedPointer<DbSqlQueryHandler> &query) const
{
    DbSqlQueryHandler *tmpQuery = ThreadConcurrentRun::run(*_dbExecuter,
                                                           &DbExecuter::execSelect,
                                                           sqlQuery);
    if(tmpQuery == nullptr)
    {
        return false;
    }

    query.reset(tmpQuery);
    return true;
}

bool DbThread::executeSqlScriptFile(const QString &filePath)
{
    return ThreadConcurrentRun::run(*_dbExecuter,
                                    &DbExecuter::executeSqlScriptFile,
                                    filePath);
}

bool DbThread::stopThread()
{
    if(_dbExecuter != nullptr)
    {
        QTimer::singleShot(0, _dbExecuter, &DbExecuter::deleteLater);
        _dbExecuter = nullptr;
    }

    return BaseThread::stopThread();
}

void DbThread::run()
{
    _dbExecuter = new DbExecuter();

    BaseThread::run();
}
