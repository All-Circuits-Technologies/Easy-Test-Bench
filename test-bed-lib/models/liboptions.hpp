// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QVersionNumber>

#include "dbobject.hpp"


/** @brief Librairy options database model class */
class TESTBEDLIB_DLLSPEC LibOptions : public DbObject
{
    Q_OBJECT
    Q_PROPERTY(QString dbVersion READ getDbVersionString WRITE setDbVersionString)
    Q_PROPERTY(bool guestModeAllowed READ isGuestModeAllowed WRITE setGuestModeAllowed)
    DB_TABLE_NAME("libOptions")

    public:
        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit LibOptions(QObject *parent = nullptr);

        /** @brief Get Database version */
        const QVersionNumber &getDbVersion() const { return _dbVersion; }

        /** @brief Set Database version */
        void setDbVersion(const QVersionNumber &value) { _dbVersion = value; }

        /** @brief Get Database version formatted as string */
        const QString getDbVersionString() const { return _dbVersion.toString(); }

        /** @brief Set Database version with a string value */
        void setDbVersionString(const QString &value)
        { _dbVersion = QVersionNumber::fromString(value); }

        /** @brief Get boolean which informs if guest mode is allowed  */
        bool isGuestModeAllowed() const { return _guestModeAllowed; }

        /** @brief Set boolean which informs if guest mode is allowed  */
        void setGuestModeAllowed(bool guestModeAllowed) { _guestModeAllowed = guestModeAllowed; }

    private:
        static const constexpr int defaultVersionNumber = 0;

    private:
        QVersionNumber _dbVersion{defaultVersionNumber};
        bool _guestModeAllowed{false};
};
