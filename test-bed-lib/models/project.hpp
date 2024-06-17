// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "dbobject.hpp"


class TESTBEDLIB_DLLSPEC Project : public DbObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString path READ getPath WRITE setPath)
    Q_PROPERTY(qint64 lastOpened READ getLastOpenedTs WRITE setLastOpenedTs)
    DB_TABLE_NAME("projects")

    public:
        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit Project(QObject *parent = nullptr);

    public:
        /** @brief Get the project name */
        const QString &getName() const { return _name; }

        /** @brief Set the project name */
        void setName(const QString &value) { _name = value; }

        /** @brief Get the project path */
        const QString &getPath() const { return _path; }

        /** @brief Set the user project path */
        void setPath(const QString &value) { _path = value; }

        /** @brief Get the last time the project was opened */
        const QDateTime &getLastOpened() const { return _lastOpened; }

        /** @brief Set the last time the project was opened */
        void setLastOpened(const QDateTime &lastModified) { _lastOpened = lastModified; }

        /** @brief Get the last time the project was opened, in milliseconds timestamp */
        qint64 getLastOpenedTs() const { return _lastOpened.toSecsSinceEpoch(); }

        /** @brief Set the last time the project was opened, in milliseconds timestamp */
        void setLastOpenedTs(qint64 lastOpenedTs)
        { _lastOpened = QDateTime::fromSecsSinceEpoch(lastOpenedTs); }

    private:
        QString _name;
        QString _path;
        QString _uniquePathPortion;
        QDateTime _lastOpened;
};
