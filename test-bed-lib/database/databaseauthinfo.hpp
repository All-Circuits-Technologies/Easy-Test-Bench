// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Contains all the needed information in order to connect to database */
class DatabaseAuthInfo : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit DatabaseAuthInfo(QObject *parent = nullptr);

        /** @brief Get the name of the database authentication information */
        const QString &getName() const { return _name; }

        /** @brief Set the name of the database authentication information */
        void setName(const QString &name) { _name = name; }

        /** @brief Get the URL of the database authentication information */
        const QString &getUrl() const { return _url; }

        /** @brief Set the URL of the database authentication information */
        void setUrl(const QString &url) { _url = url; }

        /** @brief Get the authentication username */
        const QString &getAuthName() const { return _authName; }

        /** @brief Set the authentication username */
        void setAuthName(const QString &authName) { _authName = authName; }

    private:
        QString _name;
        QString _url;
        QString _authName;
};
