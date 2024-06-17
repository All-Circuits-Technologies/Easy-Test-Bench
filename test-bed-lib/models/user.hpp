// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/dbobject.hpp"

#include <QVector>



/** @brief User database model class */
class TESTBEDLIB_DLLSPEC User : public DbObject
{
        Q_OBJECT
        Q_PROPERTY(QString firstName READ getFirstName WRITE setFirstName)
        Q_PROPERTY(QString lastName READ getLastName WRITE setLastName)
        Q_PROPERTY(QString username READ getUsername WRITE setUsername)
        Q_PROPERTY(QString password READ getPassword WRITE setPassword)
        Q_PROPERTY(QString salt READ getSalt WRITE setSalt)
        Q_PROPERTY(QString roles READ getSerializedRoles WRITE setSerializedRoles)
        DB_TABLE_NAME("users")

    public:
        /** @brief Class constructor
            @param firstName The user first name
            @param lastName The user last name
            @param username The user username
            @param password The user password salted and hashed
            @param salt The user salt
            @param roles The user first roles
            @param parent Optional Qt parentship */
        explicit User(const QString &firstName,
                      const QString &lastName,
                      const QString &username,
                      const QString &password,
                      const QString &salt,
                      const QVector<QString> &roles,
                      QObject* parent = nullptr);

        /** @brief Class constructor
            @param firstName The user first name
            @param lastName The user last name
            @param username The user username
            @param clearTextPassword The user password not yet salted and hashed
            @param salt The user salt
            @param roles The user first roles
            @param parent Optional Qt parentship */
        explicit User(const QString &firstName,
                      const QString &lastName,
                      const QString &username,
                      const QString &clearTextPassword,
                      const QVector<QString> &roles,
                      QObject* parent = nullptr);

        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit User(QObject* parent = nullptr);

        /** @brief Get the user first name */
        const QString &getFirstName() const { return _firstName; }

        /** @brief Set the user first name */
        void setFirstName(const QString &firstName) { _firstName = firstName; }

        /** @brief Get the user last name */
        const QString &getLastName() const { return _lastName;}

        /** @brief Set the user last name */
        void setLastName(const QString &lastName) { _lastName = lastName; }

        /** @brief Get the username */
        const QString &getUsername() const { return _username; }

        /** @brief Set the username */
        void setUsername(const QString &username) { _username = username; }

        /** @brief Get the user password */
        const QString &getPassword() const { return _password; }

        /** @brief Set the user password */
        void setPassword(const QString &password) { _password = password; }

        /** @brief Get the user salt */
        const QString &getSalt() const { return _salt; }

        /** @brief Set the user salt */
        void setSalt(const QString &salt) { _salt = salt; }

        /** @brief Get the user first name */
        const QVector<QString> &getRoles() const { return _roles; }

        /** @brief Set the user roles */
        void setRoles(const QVector<QString> &roles) { _roles = roles; }

    private:
        /** @brief Get user roles and convert them into one serialized string
            @return Serialized user roles */
        QString getSerializedRoles();

        /** @brief Convert serialized roles string into vector of roles and set it as user roles
            @param serializedRoles The user roles serialized in one string */
        void setSerializedRoles(const QString &serializedRoles);

    private:
        QString _firstName;
        QString _lastName;
        QString _username;
        QString _password;
        QString _salt;
        QVector<QString> _roles;
};
