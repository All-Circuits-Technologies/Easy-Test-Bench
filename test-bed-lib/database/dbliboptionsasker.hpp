// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class LibOptions;
class DbManager;
class QVersionNumber;


/** @brief This asker allows to formulate queries related to the libs options in the database */
class DbLibOptionsAsker : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbmanager A DbManager reference
            @param parent Optional Qt parentship */
        explicit DbLibOptionsAsker(DbManager &dbManager, QObject *parent = nullptr);

        /** @brief Retrieve libs options from the database
            @param libOptions Library otpions
            @return True if success, False if failure */
        bool getLibOptions(QSharedPointer<LibOptions> &libOptions) const;

        /** @brief Update libs options in the database
            @param libOptions Library otpions
            @return True if success, False if failure */
        bool updateLibOptions(const QSharedPointer<LibOptions> &libOptions);

    private :
        static const constexpr char *SelectLibOptionsWithMaxId = "SELECT * FROM libOptions "
                                                                 "ORDER BY id DESC LIMIT 1";

    private :
        DbManager &_dbManager;
};
