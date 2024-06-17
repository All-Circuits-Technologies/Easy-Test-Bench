// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/dbobject.hpp"

#include <QDateTime>


/** @brief Model used to represent the PluginPermanentStorage table in the database
    @note This is used by plugins to store permanent data */
class PluginPermStorageValue : public DbObject
{
    Q_OBJECT
    DB_TABLE_NAME("pluginPermanentStorage")

    Q_PROPERTY(QString key READ getKey WRITE setKey)
    Q_PROPERTY(QString value READ getValue WRITE setValue)
    Q_PROPERTY(QString note READ getNote WRITE setNote)

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit PluginPermStorageValue(QObject *parent = nullptr);

    /** @brief Class constructor
        @param key The storage key
        @param value The storage value
        @param parent The class parent */
    explicit PluginPermStorageValue(const QString &key,
                                    const QString &value,
                                    QObject *parent = nullptr);

    /** @brief Class constructor
        @param id The model id
        @param key The storage key
        @param value The storage value
        @param note The note linked to the value
        @param lastModified Represent the last time the data was modified
        @param parent The class parent */
    explicit PluginPermStorageValue(qint64 id,
                                    const QString &key,
                                    const QString &value,
                                    const QString &note,
                                    const QDateTime &lastModified,
                                    QObject *parent = nullptr);

public:
    /** @brief Get the key linked to the permanent storage */
    const QString &getKey() const { return _key; }

    /** @brief Set the key linked to the permanent storage */
    void setKey(const QString &key) { _key = key; }

    /** @brief Get the value stored */
    const QString &getValue() const { return _value; }

    /** @brief Set the value stored */
    void setValue(const QString &value) { _value = value; }

    /** @brief Get the note linked to this element */
    const QString &getNote() const { return _note; }

    /** @brief Set the note linked to this element */
    void setNote(const QString &note) { _note = note; }

public:
    static const constexpr char *keyName = "key";

private:
    QString _key;
    QString _value;
    QString _note;
};
