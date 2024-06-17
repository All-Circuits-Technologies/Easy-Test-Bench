// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "dbmanager.hpp"

class User;


/** @brief This asker allows to formulate queries related to the User class in the database */
class DbUserAsker : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbmanager A DbManager reference
            @param parent Optional Qt parentship */
        explicit DbUserAsker(DbManager &dbManager, QObject *parent = nullptr);

        /** @brief Add a user in the database
            @param user The user which will be added
            @return True if success, false if failure */
        bool addUser(const QSharedPointer<User> &user);

        /** @brief Get a user from the database according to its username
            @param username The username of the user
            @param user The user which will be retrived
            @return True if a user was found, False if failure or no user was found */
        bool getUserByUsername(const QString &username, QSharedPointer<User> &user) const;

        /** @brief Delete a user in the database
            @param user The user which will be delete
            @return True if success, false if failure */
        bool deleteUser(const QSharedPointer<User> &user);

        /** @brief Update a user in the database
            @param user The user which will be update
            @return True if success, false if failure */
        bool updateUser(const QSharedPointer<User> &user);

        /** @brief Retrieve all users registered in the database
            @param users Vector which contains users found
            @return True if success, False if failure */
        bool getAllUsers(QVector<QSharedPointer<User>> &users) const;

    private:
        static const constexpr char *selectUserByName =
                "SELECT * FROM users WHERE username = '%1'";

    private :
        DbManager &_dbManager;
};
