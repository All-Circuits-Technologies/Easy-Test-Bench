// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abstractbloc.hpp"


/** @brief Backend class linked with the credits pop up **/
class CreditsPopUpBloc : public AbstractBloc
{
        Q_OBJECT
        Q_PROPERTY(QString yearsOfProduction READ getYearsOfProduction NOTIFY blocInitialized)
        Q_PROPERTY(QString author READ getAuthor NOTIFY blocInitialized)
        Q_PROPERTY(QString address READ getAddress NOTIFY blocInitialized)
        Q_PROPERTY(QString country READ getCountry NOTIFY blocInitialized)
        Q_PROPERTY(QString website READ getWebsite NOTIFY blocInitialized)
        Q_PROPERTY(QString appVersion READ getAppVersion NOTIFY blocInitialized)

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        CreditsPopUpBloc(QObject *parent = nullptr);

        /** @brief Get a string indicating the year of production of the software */
        QString getYearsOfProduction() const { return _yearsOfProduction; }

        /** @brief Get a string indicating the author of the software */
        QString getAuthor() const { return _author; }

        /** @brief Get a string indicating the software company address **/
        QString getAddress() const { return _address; }

        /** @brief Get a string indicating the software country **/
        QString getCountry() const { return _country; }

        /** @brief Get a string indicating the software website **/
        QString getWebsite() const { return _website; }

        /** @brief Get a string indicating the software version of the application **/
        QString getAppVersion() const { return _appVersion; }

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        QString _yearsOfProduction;
        QString _author;
        QString _address;
        QString _country;
        QString _website;
        QString _appVersion;
};
