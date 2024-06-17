// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/dbobject.hpp"


/** @brief Role database model class */
class TESTBEDLIB_DLLSPEC Role : public DbObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    DB_TABLE_NAME("roles")

    public:
        /** @brief Class constructor
            @param name The role name
            @param parent Optional Qt parentship */
        explicit Role(const QString &name, QObject *parent = nullptr);

        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit Role(QObject *parent = nullptr);

    public:
        /** @brief Get the role name */
        const QString &getName() const { return _name; }

        /** @brief Set the role name */
        void setName(const QString &name) { _name = name; }

    private:
        QString _name;
};
