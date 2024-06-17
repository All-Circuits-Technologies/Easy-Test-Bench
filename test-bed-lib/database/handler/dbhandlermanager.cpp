// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbhandlermanager.hpp"

#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include <QDebug>
#include <QTimer>


DbHandlerManager::DbHandlerManager(QObject *parent) :
    QObject(parent)
{
}

DbHandlerManager::~DbHandlerManager()
{
    QHash<QString, DatabaseContainer>::iterator iter = _dbContainers.begin();
    for(; iter != _dbContainers.end(); ++iter)
    {
        DatabaseContainer &container = iter.value();
        if(container.db.isOpen())
        {
            container.db.close();
        }
    }
}

void DbHandlerManager::setInactiveTimeoutInMs(int inactiveTimeout, const QString &connectionName)
{
    if(_timeoutOverwritten.contains(connectionName))
    {
        _timeoutOverwritten[connectionName] = inactiveTimeout;
    }
    else
    {
        _timeoutOverwritten.insert(connectionName, inactiveTimeout);
    }

    if(_dbContainers.contains(connectionName))
    {
        // If the timer is running, this will kill the timer and runs a new one with the new value
        manageTimer(inactiveTimeout, connectionName, _dbContainers[connectionName]);
    }
}

void DbHandlerManager::incrementHandlerNb(const DbHandlerKey &key, const QString &connectionName)
{
    Q_UNUSED(key)

    ThreadConcurrentRun::run(*this, &DbHandlerManager::incrementHandlerNbPriv, connectionName);
}

void DbHandlerManager::incrementHandlerNbPriv(const QString &connectionName)
{
    DatabaseContainer &dbContainer = getOrCreateIfNeededDbContainer(connectionName);

    ++dbContainer.handlerNb;
    if(dbContainer.handlerNb == 1)
    {
        firstHandlerCreated(dbContainer);
    }
}

void DbHandlerManager::decrementHandlerNb(const DbHandlerKey &key, const QString &connectionName)
{
    Q_UNUSED(key)

    ThreadConcurrentRun::run(*this, &DbHandlerManager::decrementHandlerNbPriv, connectionName);
}

void DbHandlerManager::decrementHandlerNbPriv(const QString &connectionName)
{
    if(!_dbContainers.contains(connectionName))
    {
        qWarning() << "We can't decrement the handle nb, there is no db container created";
        return;
    }

    DatabaseContainer &container = _dbContainers[connectionName];

    if(container.handlerNb == 0)
    {
        qWarning() << "We can't decrement the handle nb, there is no handle registered";
        return;
    }

    --container.handlerNb;
    if(container.handlerNb == 0)
    {
        lastHandlerRemoved(container);
    }
}

void DbHandlerManager::pingDb(const QString &connectionName)
{
    DatabaseContainer &dbContainer = getOrCreateIfNeededDbContainer(connectionName);

    if(dbContainer.handlerNb > 0)
    {
        // Nothing to do
        return;
    }

    qDebug() << "Ping Db";

    // We simulate the creation and destruction of a handler
    dbContainer.db = QSqlDatabase::database();
    startTimer(dbContainer);
}

void DbHandlerManager::onInactiveTimeout(const QString &connectionName)
{
    if(!_dbContainers.contains(connectionName))
    {
        // Nothing to do
        return;
    }

    DatabaseContainer &container = _dbContainers[connectionName];

    if(container.handlerNb == 0)
    {
        qDebug() << "Inactive Database, close it";
        container.db.close();
        container.db = QSqlDatabase();
    }
}

void DbHandlerManager::firstHandlerCreated(DatabaseContainer &dbContainer)
{
    qDebug() << "First handler created";
    stopTimer(dbContainer);
    // This will keep the session opened
    dbContainer.db = QSqlDatabase::database();
}

void DbHandlerManager::lastHandlerRemoved(DatabaseContainer &dbContainer)
{
    qDebug() << "Last handler removed";
    startTimer(dbContainer);
}

DbHandlerManager::DatabaseContainer &DbHandlerManager::getOrCreateIfNeededDbContainer(
                                                                    const QString &connectionName)
{
    if(_dbContainers.contains(connectionName))
    {
        // Already created, do nothing
        return _dbContainers[connectionName];
    }

    int timeout = DefaultInactiveTimeoutInMs;

    if(_timeoutOverwritten.contains(connectionName))
    {
        timeout = _timeoutOverwritten[connectionName];
    }

    _dbContainers.insert(connectionName, DatabaseContainer());

    DatabaseContainer &container = _dbContainers[connectionName];
    manageTimer(timeout, connectionName, container);

    return container;
}

void DbHandlerManager::manageTimer(int inactiveTimeout,
                                   const QString &connectionName,
                                   DbHandlerManager::DatabaseContainer &container)
{
    if(inactiveTimeout >= 0)
    {
        if(container.inactiveTimeout == nullptr)
        {
            container.inactiveTimeout = new QTimer(this);
            container.inactiveTimeout->setSingleShot(true);
            connect(container.inactiveTimeout, &QTimer::timeout,
                    this, [connectionName, this]() { onInactiveTimeout(connectionName); });
        }

        container.inactiveTimeout->setInterval(inactiveTimeout);

        if(container.db.isOpen() &&
           (container.handlerNb == 0) &&
           !container.inactiveTimeout->isActive())
        {
            // Start the timeout cause we are in the case where the timeout should be running
            container.inactiveTimeout->start();
        }
    }
    else if(container.inactiveTimeout != nullptr)
    {
        delete container.inactiveTimeout;
        container.inactiveTimeout = nullptr;
    }
}

void DbHandlerManager::startTimer(DbHandlerManager::DatabaseContainer &dbContainer)
{
    if(dbContainer.inactiveTimeout != nullptr)
    {
        dbContainer.inactiveTimeout->start();
    }
}

void DbHandlerManager::stopTimer(DbHandlerManager::DatabaseContainer &dbContainer)
{
    if(dbContainer.inactiveTimeout != nullptr)
    {
        dbContainer.inactiveTimeout->stop();
    }
}
