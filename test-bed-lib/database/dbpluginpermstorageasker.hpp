// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class DbManager;
class PluginPermStorageValue;


/** @brief This asker allows to manage @see PluginPermStorageValue database model */
class DbPluginPermStorageAsker : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbManager The database manager
            @param parent The class parent */
        explicit DbPluginPermStorageAsker(DbManager &dbManager, QObject *parent = nullptr);

    public:
        /** @brief Get the value linked to the key given
            @param key The permanent storage key to use to get the permanent storage value
            @param storageValue The permanent storage value got from database
            @return True if no problem occurred */
        bool getValue(const QString &key,
                      QSharedPointer<PluginPermStorageValue> &storageValue) const;

        /** @brief Insert the value into the database
            @param storageValue The value to store in database
            @return True if no problem occurred */
        bool insertValue(const QSharedPointer<PluginPermStorageValue> &storageValue);

        /** @brief Modidfy the value to the database
            @param storageValue The value to store in database
            @return True if no problem occurred */
        bool modifyValue(const QSharedPointer<PluginPermStorageValue> &storageValue);

        /** @brief Set the value to the database
            @note If the value doesn't exist, it adds a new one
            @param storageValue The value to store in database
            @return True if no problem occurred */
        bool setValue(const QString &key, const QString &value);

        /** @brief Get all the values stored in the database
            @param storageValues All the values stored
            @return True if no problem occurred */
        bool getAllCacheValue(QVector<QSharedPointer<PluginPermStorageValue>> &storageValues) const;

        /** @brief Clear the permanent storage and remove all the data
            @return True if no problem occurred */
        bool clearStorage();

    private :
        DbManager &_dbManager;
};
