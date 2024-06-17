// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbmanager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QVersionNumber>
#include <QtSql/QSqlRecord>

#include "database/dbliboptionsasker.hpp"
#include "database/dbthread.hpp"
#include "models/dbobject.hpp"
#include "models/liboptions.hpp"

const QVersionNumber DbManager::versionTableNotReadable = { 0, 1, 0 };


DbManager::DbManager(QObject *parent) :
    QObject(parent),
    _dbThread(new DbThread(this))
{
}

DbManager::~DbManager()
{
}

bool DbManager::initDbManager(const QString &path)
{
    RETURN_IF_FALSE(_dbThread->initDbManager(path));

    if(!updateDb())
    {
        qWarning() << "error while updating database";
        return false;
    }

     return true;
}

bool DbManager::exec(const QString &sqlQuery)
{
    return _dbThread->exec(sqlQuery);
}

bool DbManager::exec(const QString &sqlQuery,
                     const QList<QString> &placeholderNames,
                     const QList<QVariant> &values)
{
    return _dbThread->exec(sqlQuery, placeholderNames, values);
}


bool DbManager::remove(const QSharedPointer<DbObject> &dbObject)
{
    QString idPlaceholder = QString(Placeholder).arg(DbObject::idName);

    QString queryOperation = QString(DeleteQuery).arg(dbObject->getTableName(),
                                                      QString(NameAndPlaceholders)
                                                                      .arg(DbObject::idName,
                                                                           idPlaceholder));

    if(!exec(queryOperation, { idPlaceholder }, { dbObject->getId() }))
    {
        qWarning() << "Failed to remove the wanted dbObject: " << dbObject->getId() << ", in "
                   << "table: " << dbObject->getTableName();
        return false;
    }

    return true;
}

bool DbManager::insert(const QSharedPointer<DbObject> &dbObject)
{
    QList<QString> columnNames;
    QList<QString> placeholderNames;
    QList<QVariant> values;

    getObjectDetail(dbObject, columnNames, placeholderNames, values, { DbObject::idName });

    QString queryStr = QString(InsertInto).arg(dbObject->getTableName(),
                                               columnNames.join(Separator),
                                               placeholderNames.join(Separator));

    if(!exec(queryStr, placeholderNames, values))
    {
        qWarning() << "Failed to insert the wanted dbObject: " << dbObject->getId() << ", in "
                   << "table: " << dbObject->getTableName();
        return false;
    }

    return true;
}

bool DbManager::update(const QSharedPointer<DbObject> &dbObject,
                       const QVector<QString> &propertiesToNotUpdate)
{
    QList<QString> columnNames;
    QList<QString> placeholderNames;
    QList<QVariant> values;

    QVector<QString> tmpProperties(propertiesToNotUpdate);
    if(tmpProperties.contains(DbObject::idName))
    {
        // We want the id name because we don't use it in the SET field but in the WHERE field
        tmpProperties.removeAll(DbObject::idName);
    }

    getObjectDetail(dbObject, columnNames, placeholderNames, values, tmpProperties);

    QList<QString> setField;
    QList<QString> whereField;
    for(int idx = 0; idx < placeholderNames.length(); ++idx)
    {
        const QString &columName = columnNames.at(idx);
        const QString &placeholderName = placeholderNames.at(idx);
        if(columName == DbObject::idName)
        {
            // We don't add id in the SET field but in the where field
            whereField.append(QString(NameAndPlaceholders).arg(columName, placeholderName));
            continue;
        }

        setField.append(QString(NameAndPlaceholders).arg(columName, placeholderName));
    }

    QString queryStr = QString(UpdateOne).arg(dbObject->getTableName(),
                                              setField.join(Separator),
                                              whereField.join(Separator));

    if(!exec(queryStr, placeholderNames, values))
    {
        qWarning() << "Failed to update the wanted dbObject: " << dbObject->getId() << ", in "
                   << "table: " << dbObject->getTableName();
        return false;
    }

    // Now we get the lastModified new value
    return getLastModifiedAndUpdateObject(dbObject);
}

void DbManager::getObjectDetail(const QSharedPointer<DbObject> &dbObject,
                                QList<QString> &columnNames,
                                QList<QString> &placeholdersNames,
                                QList<QVariant> &values,
                                const QVector<QString> &removeColumns)
{
    const QMetaObject *meta = dbObject->metaObject();
    const int propertyOffset = DbObject::staticMetaObject.propertyOffset();

    for(int idx = propertyOffset; idx < meta->propertyCount(); ++idx)
    {
        QMetaProperty property = meta->property(idx);
        QString propertyName = property.name();

        if(removeColumns.contains(propertyName))
        {
            // We don't add this property
            continue;
        }

        if(!property.isReadable())
        {
            // We test if property is readable because we try then to read the property value
            qWarning() << "The property: " << propertyName << ", of the class linked to the table: "
                       << dbObject->getTableName() << ", is not readable";
            continue;
        }

        QVariant value = property.read(dbObject.data());
        if(!value.isValid())
        {
            qWarning() << "A problem occurred when tried to read the value of property: "
                       << propertyName << ", in class: " << dbObject->getTableName();
            continue;
        }

        values.append(value);
        columnNames.append(propertyName);
        placeholdersNames.append(QString(Placeholder).arg(propertyName));
    }
}

bool DbManager::updateDb()
{
    QVersionNumber dbVersion;
    bool neverGotFromDb = true;

    if (!getDbVersion(dbVersion))
    {
        qInfo() << "No database version found, we play all the scripts from start";
    } else {
        neverGotFromDb = false;
    }

    QDir directory(ScriptDirPath);

    if(!directory.exists())
    {
        qWarning() << "A problem occurred, the sql directory path doesn't exist: " << ScriptDirPath;
        return false;
    }

    QFileInfoList infoList = directory.entryInfoList({ QString(AllSqlFilePrefix) },
                                                     { QDir::Files },
                                                     QDir::Name | QDir::IgnoreCase);
    for(auto citer = infoList.cbegin(); citer != infoList.cend(); ++citer)
    {
        QString completeBaseName = citer->completeBaseName();
        QVersionNumber versionNumber = QVersionNumber::fromString(completeBaseName);
        if(QVersionNumber::compare(dbVersion, versionNumber) == -1)
        {
            if(!executeSqlScriptFile(citer->filePath()))
            {
                qWarning() << "Can't update database to version " << completeBaseName;
                return false;
            }

            if(neverGotFromDb) {
               if(QVersionNumber::compare(versionNumber, versionTableNotReadable) <= 0)
               {
                   // We can't get the version number from database
               }
               else if(!initLibOptions())
               {
                   qWarning() << "A problem occurred when tried to init the lib options table from "
                              << "database";
               } else {
                   neverGotFromDb = false;
               }
            }

            dbVersion = versionNumber;
            setDbVersion(dbVersion);

            qInfo() << "Database updated to version " << completeBaseName;
        }
    }

    return true;
}

bool DbManager::getDbVersion(QVersionNumber &dbVersion)
{
    if(_libOptions.isNull() && !initLibOptions())
    {
        return false;
    }

    dbVersion = _libOptions->getDbVersion();
    qInfo() << "Database version found : " << dbVersion.toString();

    return true;
}

bool DbManager::initLibOptions()
{
    DbLibOptionsAsker dbLibOptionAsker(*this);

    if(!dbLibOptionAsker.getLibOptions(_libOptions))
    {
        _libOptions = QSharedPointer<LibOptions>::create();
        return false;
    }

    return true;
}

bool DbManager::setDbVersion(QVersionNumber &dbVersion)
{
    _libOptions->setDbVersion(dbVersion);

    if(QVersionNumber::compare(dbVersion, versionTableNotReadable) <= 0)
    {
        // We can't get the version number from database; therefore the next update will fail
        // We return true in that case and doesn't try to request the database
        return true;
    }

    DbLibOptionsAsker dbLibOptionAsker(*this);

    return dbLibOptionAsker.updateLibOptions(_libOptions);
}

bool DbManager::executeSqlScriptFile(const QString &filePath)
{
    return _dbThread->executeSqlScriptFile(filePath);
}

bool DbManager::execSelect(const QString &sqlQuery,
                           QScopedPointer<DbSqlQueryHandler> &query) const
{
    return _dbThread->execSelect(sqlQuery, query);
}

bool DbManager::getLastModifiedAndUpdateObject(const QSharedPointer<DbObject> &dbObject)
{
    QString query = QString(SelectOneColumn).arg(DbObject::lastModifiedName,
                                                 dbObject->getTableName(),
                                                 QString(NameAndValue).arg(DbObject::idName)
                                                                       .arg(dbObject->getId()));

    QScopedPointer<DbSqlQueryHandler> handler;
    RETURN_IF_FALSE(execSelect(query, handler));

    if(!handler->accessQuery().next())
    {
        qWarning() << "No dbObject has been found in the table: " << dbObject->getTableName()
                   << ", and id: " << dbObject->getId() << ", we can't find and get the last "
                   << "modified value";
        return false;
    }

    QSqlRecord record = handler->accessQuery().record();
    if(!record.contains(DbObject::lastModifiedName))
    {
        qWarning() << "The property: " << DbObject::lastModifiedName << ", hasn't been found in "
                   << "the record, to find and get the last modified value";
        return false;
    }

    QVariant value = record.value(DbObject::lastModifiedName);

    bool ok = false;
    qint64 lastModified = value.toLongLong(&ok);
    if(!ok)
    {
        qWarning() << "The value got from database for the property: " << DbObject::lastModifiedName
                   << ", isn't a qint64: " << value;
        return false;
    }

    dbObject->setLastModifiedTs(lastModified);
    return true;
}

bool DbManager::getColumnIdxsInQueryByNames(const QMetaObject &metaObject,
                                            const QSqlQuery &sqlQuery,
                                            QHash<QString, int> &idxByNames)
{
    if(!sqlQuery.isValid())
    {
        qWarning() << "The current record of the sql query is invalid, can't get the column "
                   << "indexes";
        return false;
    }

    const int propertyOffset = DbObject::staticMetaObject.propertyOffset();
    QSqlRecord record = sqlQuery.record();

    for(int idx = propertyOffset; idx < metaObject.propertyCount(); ++idx)
    {
        QMetaProperty property = metaObject.property(idx);
        QString propertyName = property.name();

        if(!property.isWritable())
        {
            // We test if the property is writable because we will get values from database and
            // write them in the properties of a DbObject
            qWarning() << "The property: " << propertyName << ", of the class linked: "
                       << metaObject.className() << ", is not writable";
            continue;
        }

        int columnIdx = record.indexOf(propertyName);
        idxByNames.insert(propertyName, columnIdx);
    }

    return true;
}
