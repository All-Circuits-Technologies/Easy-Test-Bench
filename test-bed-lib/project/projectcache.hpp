// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QVariant>


/** @brief This is the cache linked to the project, the value are stored in cache while the project
           is alive. If the user closes the application or unload the project, the cache values are
           lost */
class ProjectCache : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class construtor
            @param parent The parent instance */
        explicit ProjectCache(QObject *parent = nullptr);

    public:
        /** @brief Get the cache value linked to the key
            @param key The key of the value to get
            @return The value got or null QVariant if the value hasn't been found */
        QVariant getValue(const QString &key) const;

        /** @brief Set the cache value linked to the key
            @param key The key of the value to set
            @param value The value to set */
        void setValue(const QString &key, const QVariant &value);

    private:
        QHash<QString, QVariant> _projectCache;
};
