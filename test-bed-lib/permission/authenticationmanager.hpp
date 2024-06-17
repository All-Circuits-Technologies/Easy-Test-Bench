// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "testbedcore/testbedglobal.hpp"

class DbLibOptionsAsker;
class DbManager;
class DbUserAsker;
class User;


/** @brief Manages the user's connection and disconnection */
class TESTBEDLIB_DLLSPEC AuthenticationManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbManager Reference to a DbManager
            @param parent Optional Qt parentship */
        explicit AuthenticationManager(DbManager &dbManager, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AuthenticationManager() override;

    public:
        /** @brief log the default user
            @return True if success, false if failure */
        bool loginDefaultUser();

        /** @brief Check if username and password are correct and log the associated user
            @param username The user username
            @param clearTextPassword The user clear text password
            @param idsAreCorrect Set to true if username and password are correct, false if not
            @return True if success, false if failure */
        bool login(const QString &username, const QString &clearTextPassword, bool &idsAreCorrect);

        /** @brief Logout current user */
        void logout();

        /** @brief Get the current user */
        const QSharedPointer<User> &getCurrentUser() const { return _currentUser; }

    public :
        /** @brief Salt and hash a password
            @param clearTextPassword A user clear text password
            @param salt The salt associated with the password*/
        static QString saltAndHashPassword(const QString &clearTextPassword, const QString &salt);

    signals :
        /** @brief Emitted when the user logs in
            @param user The user connected */
        void userConnected(QSharedPointer<User> &user);

        /** @brief Emitted when the user logs out */
        void userDisconnected();

    private:
        static const constexpr char *defaultUsername = "default";

    private:
        QSharedPointer<User> _currentUser;
        DbUserAsker *_dbUserAsker{nullptr};
};
