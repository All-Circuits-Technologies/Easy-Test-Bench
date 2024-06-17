// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbpluginpermstorageasker.hpp"


#include "database/dbmanager.hpp"
#include "models/pluginpermstoragevalue.hpp"


DbPluginPermStorageAsker::DbPluginPermStorageAsker(DbManager &dbManager, QObject *parent) :
    QObject(parent),
    _dbManager(dbManager)
{
}

bool DbPluginPermStorageAsker::getValue(const QString &key,
                                        QSharedPointer<PluginPermStorageValue> &storageValue) const
{
    QString query = QString(DbManager::SelectQuery).arg(PluginPermStorageValue::tableName,
                                                        QString(DbManager::NameAndValue)
                                                               .arg(PluginPermStorageValue::keyName)
                                                               .arg(key));

    return _dbManager.selectOne(query, storageValue);
}

bool DbPluginPermStorageAsker::insertValue(
                                        const QSharedPointer<PluginPermStorageValue> &storageValue)
{
    return _dbManager.insert(storageValue);
}

bool DbPluginPermStorageAsker::modifyValue(
                                        const QSharedPointer<PluginPermStorageValue> &storageValue)
{
    return _dbManager.update(storageValue);
}

bool DbPluginPermStorageAsker::setValue(const QString &key, const QString &value)
{
    QSharedPointer<PluginPermStorageValue> storageValue;
    RETURN_IF_FALSE(getValue(key, storageValue));

    if(storageValue.isNull())
    {
        // If null, it means that the value doesn't exist, we have to insert it
        storageValue = QSharedPointer<PluginPermStorageValue>::create(key, value);
        RETURN_IF_FALSE(insertValue(storageValue));
    }
    else if(storageValue->getValue() != value)
    {
        // If not null and if the value are different we just modify the value
        storageValue->setValue(value);
        RETURN_IF_FALSE(modifyValue(storageValue));
    }

    return true;
}

bool DbPluginPermStorageAsker::getAllCacheValue(
                            QVector<QSharedPointer<PluginPermStorageValue>> &storageValues) const
{
    return _dbManager.select(storageValues);
}

bool DbPluginPermStorageAsker::clearStorage()
{
    if(!_dbManager.removeAll<PluginPermStorageValue>())
    {
        qWarning() << "An error occurred when tryied to erase all data in the plugin cache";
        return false;
    }

    return true;
}
