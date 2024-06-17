// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QtSql/QSqlDatabase>

class DbHandlerKey;
class QTimer;


/** @brief This manage the Database handlers, it keeps a link to the @ref QSqlDatabase, to avoid to
           close the connection after each request.
           It manages an inactive timeout, to close the connection to database when it's no more
           needed. To disable this timeout and keeps the connection always open, set a negative
           number */
class DbHandlerManager : public QObject
{
    Q_OBJECT

    private:
        /** @brief Helpful container which contains all the needed information for a given
                   connection */
        struct DatabaseContainer
        {
            int handlerNb{0};
            QSqlDatabase db;
            QTimer *inactiveTimeout{nullptr};
        };

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit DbHandlerManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbHandlerManager() override;

    public:
        /** @brief Set the inactive timeout in Ms which is linked to the connection name
            @note The manager will keeps the wanted connection opened until the timeout raises and
                  if no handler has been created before
            @note By default the timeout is equals to @ref DefaultInactiveTimeoutInMs
            @note To disable the inactive timeout sets a negative number
            @param inactiveTimeout The inactive timeout to set
            @param connectionName The name of the connection where to set the timeout */
        void setInactiveTimeoutInMs(int inactiveTimeout,
                                    const QString &connectionName =
                                                    QLatin1String(QSqlDatabase::defaultConnection));

    public slots:
        /** @brief Call to increment the number of handler alive
            @note This method can only be called by a @ref DbHandler class
            @note This method is thread safe
            @param key The key linked to the @ref DbHandler class
            @param connectionName The name of the connection liked to the handler */
        void incrementHandlerNb(const DbHandlerKey &key, const QString &connectionName);

        /** @brief Call to decrement the number of handler alive
            @note This method can only be called by a @ref DbHandler class
            @note This method is thread safe
            @param key The key linked to the @ref DbHandler class
            @param connectionName The name of the connection liked to the handler */
        void decrementHandlerNb(const DbHandlerKey &key, const QString &connectionName);

        /** @brief Ping the connection targeted, to create and keeps a link to the database
            @note Call this method is like create a DbHandler and removes it right away, it fires
                  the timeout (if existent)
            @param connectionName The name of the connection to ping */
        void pingDb(const QString &connectionName = QLatin1String(QSqlDatabase::defaultConnection));

    private slots:
        /** @brief Called when the timeout of a connection is raised
            @param connectionName The name of the connection to close */
        void onInactiveTimeout(const QString &connectionName);

        /** @see incrementHandlerNb
            @note This is the implementation of @ref incrementHandlerNb */
        void incrementHandlerNbPriv(const QString &connectionName);

        /** @see decrementHandlerNb
            @note This is the implementation of @ref decrementHandlerNb */
        void decrementHandlerNbPriv(const QString &connectionName);

    private:
        /** @brief Called when the first handler, for a connection, is created
            @param dbContainer The database container linked to the connection */
        void firstHandlerCreated(DatabaseContainer &dbContainer);

        /** @brief Called when the last handler of a connection is removed
            @param dbContainer The database container linked to the connection */
        void lastHandlerRemoved(DatabaseContainer &dbContainer);

        /** @brief Useful method to get or create the wanted db container
            @param connectionName The name of the connection
            @return The Database container linked to the connection */
        DatabaseContainer &getOrCreateIfNeededDbContainer(const QString &connectionName);

        /** @brief Manage the timer depending of the inactive timeout value
            @note If the timeout is positive (0 included), it will creates the timer (if it doesn't
                  exist)
            @note If the timeout is negative, it will removes the timer
            @param inactiveTimeout The timeout to apply
            @param connectionName The name of the connection linked to the timeout
            @param container The container linked to the connection */
        void manageTimer(int inactiveTimeout,
                         const QString &connectionName,
                         DatabaseContainer &container);

        /** @brief Start the timer linked to the container if it's not null */
        void startTimer(DatabaseContainer &dbContainer);

        /** @brief Stop the timer linked to the container if it's not null */
        void stopTimer(DatabaseContainer &dbContainer);

    private:
        static const constexpr int DefaultInactiveTimeoutInMs = 5000;

    private:
        QHash<QString, DatabaseContainer> _dbContainers;
        QHash<QString, int> _timeoutOverwritten;
};
