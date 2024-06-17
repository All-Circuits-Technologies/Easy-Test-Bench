// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QtSql/QSqlDatabase>

class DbHandlerManager;
class DbSqlQueryHandler;
class QTimer;


/** @brief Useful class to execute the sql scripts */
class DbExecuter : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit DbExecuter(QObject *parent = nullptr);

        /** @brief The class destructor */
        virtual ~DbExecuter() override;

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

        /** @brief Execute the sql query to select elements
            @param sqlQuery The SQL query to exectute
            @return The object which contains the query answers */
        DbSqlQueryHandler *execSelect(const QString &sqlQuery) const;

        /** @brief Execute an sql script contained in a file
            @param filePath The path of the sql script file
            @return True if success, false if failure */
        bool executeSqlScriptFile(const QString &filePath);

    public:
        static const constexpr char ScriptSeparator = '$';

    private:
        static const constexpr char *DriverName = "QSQLITE";

    private:
        DbHandlerManager *_handlerManager{nullptr};
};
