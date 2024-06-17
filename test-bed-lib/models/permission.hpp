// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/dbobject.hpp"


/** @brief Permission Database model class */
class TESTBEDLIB_DLLSPEC Permission : public DbObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)
    DB_TABLE_NAME("permissions")

    public:
        /** @brief Class constructor
            @param name The permission name
            @param description The permission description
            @param parent Optional Qt parentship */
        explicit Permission(const QString &name,
                            const QString &description,
                            QObject *parent = nullptr);

        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit Permission(QObject *parent = nullptr);

    public:
        /** @brief Get the permission name */
        const QString &getName() const { return _name; }

        /** @brief Set the permission name */
        void setName(const QString &name) { _name = name; }

        /** @brief Get the permission description */
        const QString &getDescription() const { return _description; }

        /** @brief Set the permission description */
        void setDescription(const QString &description) { _description = description; }

    private :
        QString _name;
        QString _description;
};
