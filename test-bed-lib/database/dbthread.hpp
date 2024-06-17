// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

class DbExecuter;
class DbSqlQueryHandler;


/** @brief Thread which contains all the database calls
    @note This thread allows to not block the main thread when executing some code on the
          database */
class DbThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit DbThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbThread() override;

    public:
        /** @brief Initialize the DbManager
            @param path The path of the database file
            @return True if success, false if failure */
        bool initDbManager(const QString &path);

        /** @brief Execute the given SQL query
            @param sqlQuery The query to execute
            @return True if success, false if failure */
        bool exec(const QString &sqlQuery);

        /** @brief Execute the given SQL query
            @note The placeholderNames list has to be in the same order as the values list
                  Example: if the id placeholder is at index 0 in the placeholderNames list, its
                           value has also to be at index 0 in the values list
            @param sqlQuery The query to execute, the query contains placeholders
            @param placeholderNames The placeholder names
            @param values The values to set in the query
            @return True if success, false if failure */
        bool exec(const QString &sqlQuery,
                  const QList<QString> &placeholderNames,
                  const QList<QVariant> &values);

        /** @brief Execute a sql query and returns the query with its response
            @note If the method returns the true, the query can't be null
            @param sqlQuery The query to execture
            @param query Contains the query which contains the answers
            @return True if success */
        bool execSelect(const QString &sqlQuery, QScopedPointer<DbSqlQueryHandler> &query) const;

        /** @brief Execute an sql script contained in a file
            @param filePath The path of the sql script file
            @return True if success, false if failure */
        bool executeSqlScriptFile(const QString &filePath);

    public slots:
        /** @brief Call to stop the thread
            @return True if no problem occurs */
        virtual bool stopThread() override;

    protected:
        /** @see BaseThread::run */
        virtual void run() override;

    private:
        DbExecuter *_dbExecuter{nullptr};
};
