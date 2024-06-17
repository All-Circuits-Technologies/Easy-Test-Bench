// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QDateTime>

#include "testbedcore/testbedglobal.hpp"


/** @brief Database base model class */
class TESTBEDLIB_DLLSPEC DbObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ getId WRITE setId)
    Q_PROPERTY(qint64 lastModified READ getLastModifiedTs WRITE setLastModifiedTs)

    public:
        /** @brief Class constructor
            @param id The if of the dbObject
            @param lastModified The last time the data was modified
            @param parent Optional Qt parentship */
        explicit DbObject(qint64 id, const QDateTime &lastModified, QObject *parent = nullptr);

        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit DbObject(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbObject() override = default;

    public:
        /** @brief Get the DbObject Id */
        qint64 getId() const { return _id; }

        /** @brief Set the DbObject Id */
        void setId(qint64 id) { _id = id; }

        /** @brief Get the last time the value was modified */
        const QDateTime &getLastModified() const { return _lastModified; }

        /** @brief Set the last time the value was modified */
        void setLastModified(const QDateTime &lastModified) { _lastModified = lastModified; }

        /** @brief Get the last time the value was modified, in milliseconds timestamp */
        qint64 getLastModifiedTs() const { return _lastModified.toSecsSinceEpoch(); }

        /** @brief Set the last time the value was modified, in milliseconds timestamp */
        void setLastModifiedTs(qint64 lastModifiedTs)
        { _lastModified = QDateTime::fromSecsSinceEpoch(lastModifiedTs); }

        /** @brief Get the DbObject table name */
        virtual QString getTableName() = 0;

    public:
        static const constexpr char* idName = "id";
        static const constexpr char* lastModifiedName = "lastModified";

    private:
        static const constexpr quint64 defaultId = 0;

    private:
        qint64 _id{defaultId};
        QDateTime _lastModified;
};

#define DB_TABLE_NAME(name) \
    public : inline static const QString tableName = name; \
             virtual QString getTableName() override { return name; }
