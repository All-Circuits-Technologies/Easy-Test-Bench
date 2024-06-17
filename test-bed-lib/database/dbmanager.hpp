// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "database/handler/dbsqlqueryhandler.hpp"
#include "models/dbobject.hpp"

#include <QMetaProperty>
#include <QMetaObject>
#include <QtSql/QSqlQuery>

class DbThread;
class LibOptions;
class QVersionNumber;


/** @brief The DbManager class manages the execution of requests to the database */
class TESTBEDLIB_DLLSPEC DbManager : public QObject
{
    public:
        /** @brief Class constructor
            @param parent Optional Qt parentship */
        explicit DbManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbManager() override;

    public:
        /** @brief Initialize the DbManager
            @param path The path of the database file
            @return True if success, false if failure */
        bool initDbManager(const QString &path);

        /** @brief Retrieve elements from the database
            @note This method allows to specifiy
            @tparam T any object derived from DbObject class
            @param queryOperation Redefinition of the sql query to execute
            @param dbObjects Vector of DbObjects retrieved from the database
            @return True if success, false if failure */
        template <typename T>
        bool select(const QString &queryOperation,
                    QVector<QSharedPointer<T>> &dbObjects) const;

        /** @brief Retrieve elements from the database
            @tparam T any object derived from DbObject class
            @param dbObjects Vector of DbObjects retrieved from the database
            @return True if success, false if failure */
        template <typename T>
        bool select(QVector<QSharedPointer<T>> &dbObjects) const;

        /** @brief Retrieve first element found in the database
            @tparam T any object derived from DbObject class
            @param queryOperation Redefinition of the sql query to execute
            @param dbObject The first DbObject retrieved from the database
            @return True if success, false if failure */
        template<class T>
        bool selectOne(const QString &queryOperation, QSharedPointer<T> &dbObject) const;

        /** @brief Retrieve the element found in the database thanks to its id
            @tparam T any object derived from DbObject class
            @param id The id of the dbObject to get
            @param dbObject The first DbObject retrieved from the database
            @return True if success, false if failure */
        template<class T>
        bool selectOne(qint64 id, QSharedPointer<T> &dbObject) const;

        /** @brief Insert an element in the databse
            @param dbObject The element that will be inserted
            @return True if success, false if failure */
        bool insert(const QSharedPointer<DbObject> &dbObject);

        /** @brief Remove an element in the database
            @param dbObject The element that will be removed
            @return True if success, false if failure */
        bool remove(const QSharedPointer<DbObject> &dbObject);

        /** @brief Remove all the elements from database
            @tparam T any object derived from DbObject class
            @return True if success, false if failure */
        template<class T>
        bool removeAll();

        /** @brief Update an element in the databse
            @param dbObject The element that will be updated
            @param propertiesToNotUpdate If not empty, the properties given (if they exist) won't be
                                         updated in database
            @return True if success, false if failure */
        bool update(const QSharedPointer<DbObject> &dbObject,
                    const QVector<QString> &propertiesToNotUpdate = QVector<QString>());

        /** @brief Get lib Options */
        QSharedPointer<LibOptions> getLibOptions() const { return _libOptions; }

    private:
        /** @brief Execute sql scripts if the database is not up to date
            @return True if success, false if failure */
        bool updateDb();

        /** @brief Get database version
            @param dbVersion Database version number returned, 0 if no version found
            @return True if success, false if failure */
        bool getDbVersion(QVersionNumber &dbVersion);

        /** @brief Init lib options
            @return True if success, false if failure */
        bool initLibOptions();

        /** @brief Set database version
            @param dbVersion Database version
            @return True if success, false if failure */
        bool setDbVersion(QVersionNumber &dbVersion);

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

        /** @brief Execute an sql script contained in a file
            @param filePath The path of the sql script file
            @return True if success, false if failure */
        bool executeSqlScriptFile(const QString &filePath);

        /** @brief Execute a sql query and returns the query with its response
            @note If the method returns the true, the query can't be null
            @param sqlQuery The query to execture
            @param query Contains the query which contains the answers
            @return True if success */
        bool execSelect(const QString &sqlQuery, QScopedPointer<DbSqlQueryHandler> &query) const;

        /** @brief Get from the Database the last modified timestamp value of the db object, and
                   update the given object last modified property
            @note The method has to be used after having update the object in database, in order to
                  get the last modified ts set by the database.
                  Any other usage will break the process of last modified verification in the
                  database
            @param dbObject The db object to update
            @return True if success */
        bool getLastModifiedAndUpdateObject(const QSharedPointer<DbObject> &dbObject);

        /** @brief Fill the dbObject with the data got from query
            @tparam T any object derived from DbObject class
            @param sqlQuery The SQL query already executed, it has to be valid
            @param idxByNames Useful correlation table to know the index in the sql response of
                              each property value
            @param dbObject The db object to fill with the values got from database
            @return True if success, false if failure */
        template<class T>
        bool fillFromQuery(const QSqlQuery &sqlQuery,
                           const QHash<QString, int> &idxByNames,
                           QSharedPointer<T> &dbObject) const;

    private:
        /** @brief Get a correlation table to know where to find the property values in the SQL
                   answer
            @tparam T any object derived from DbObject class
            @note The method is used to find properties in order to write values in it
            @param sqlQuery The SQL query already executed, it has to be valid
            @param idxByNames Useful correlation table to know the index in the sql response of
                              each property value
            @return True if success, false if failure */
        template<class T>
        static bool getColumnIdxsInQueryByNames(const QSqlQuery &sqlQuery,
                                                QHash<QString, int> &idxByNames);

        /** @brief Get a correlation table to know where to find the property values in the SQL
                   answer
            @note The method is used to find properties in order to write values in it
            @param metaObject The meta object of the derived DbObject class linked to the table
            @param sqlQuery The SQL query already executed, it has to be valid
            @param idxByNames Useful correlation table to know the index in the sql response of
                              each property value
            @return True if success, false if failure */
        static bool getColumnIdxsInQueryByNames(const QMetaObject &metaObject,
                                                const QSqlQuery &sqlQuery,
                                                QHash<QString, int> &idxByNames);

        /** @brief Get db object details
            @note The columnNames, placeholdersNames and values lists are returned in the same
                  order. They are bound together.
                  This means that at index 0 of the three lists we will have the property name,
                  the property value and the placeholder name
            @param dbObject The dbObject to get its values from
            @param columnNames The column names, which corresponds to the properties name
            @param placeholdersNames The placeholders names to use in a SqlQuery to make data match
            @param values The propertie values linked to the column names
            @param removeColumns If not empty, the columns contains in this list won't be added to
                                 the three previous list.
                                 For instance, if the property "id" is in this column, it won't be
                                 contained in the returned lists */
        static void getObjectDetail(const QSharedPointer<DbObject> &dbObject,
                                    QList<QString> &columnNames,
                                    QList<QString> &placeholdersNames,
                                    QList<QVariant> &values,
                                    const QVector<QString> &removeColumns = QVector<QString>());

    public:
        static const constexpr char *SelectQuery = "SELECT * FROM %1 WHERE %2";
        static const constexpr char *NameAndValue = "%1 = '%2'";
        static const constexpr char *NameAndPlaceholders = "%1 = %2";

        static const constexpr char *SelectOneColumn = "SELECT %1 FROM %2 WHERE %3";

    private:
        static const constexpr char *SelectAllQuery = "SELECT * FROM %1";
        static const constexpr char *SelectDbVersionQuery = "SELECT version FROM db_version";

        static const constexpr char *DeleteQuery = "DELETE FROM %1 WHERE %2";
        static const constexpr char *DeleteAll = "DELETE FROM %1";

        static const constexpr char *InsertInto = "INSERT INTO %1 (%2) VALUES (%3)";

        static const constexpr char *UpdateOne = "UPDATE %1 SET %2 WHERE %3";

        static const constexpr char *SqlFilePrefix = ".sql";
        static const constexpr char *AllSqlFilePrefix = "*.sql";
        static const constexpr char *Separator = ", ";
        static const constexpr char *Placeholder = ":%1";
        static const constexpr char *ScriptDirPath = ":/sql/sql-scripts";

        static const QVersionNumber versionTableNotReadable;

    private:
        /** @brief Attribute which handles a connection to a database */
        QSharedPointer<LibOptions> _libOptions;
        DbThread *_dbThread{nullptr};
};

template<class T>
bool DbManager::removeAll()
{
    QString queryOperation = QString(DeleteAll).arg(T::tableName);

    if(!exec(queryOperation))
    {
        qWarning() << "Failed to remove all the wanted dbObject from table: "
                   << T::tableName;
        return false;
    }

    return true;
}

template<typename T>
bool DbManager::select(const QString &queryOperation, QVector<QSharedPointer<T>> &dbObjects) const
{
    static_assert(std::is_base_of_v<DbObject, T>,
                  "Error : class passed as template is not of type 'DbObject'");

    QScopedPointer<DbSqlQueryHandler> handler;
    RETURN_IF_FALSE(execSelect(queryOperation, handler));

    dbObjects.clear();

    QHash<QString, int> idxByNames;

    while (handler->accessQuery().next())
    {
        if(idxByNames.isEmpty())
        {
            // Call the method at the first occurence, to be sure to deal with a valid record
            RETURN_IF_FALSE(getColumnIdxsInQueryByNames<T>(handler->accessQuery(), idxByNames));
        }

        QSharedPointer<T> pDbObject = QSharedPointer<T>::create();
        RETURN_IF_FALSE(fillFromQuery(handler->accessQuery(), idxByNames, pDbObject));

        dbObjects.append(pDbObject);
    }

    return true;
}

template<typename T>
bool DbManager::select(QVector<QSharedPointer<T> > &dbObjects) const
{
    return select(QString(SelectAllQuery).arg(T::tableName), dbObjects);
}

template<class T>
bool DbManager::selectOne(const QString &queryOperation, QSharedPointer<T> &dbObject) const
{
    static_assert(std::is_base_of_v<DbObject, T>,
            "Error : class passed as template is not of type 'DbObject'");

    QScopedPointer<DbSqlQueryHandler> handler;
    RETURN_IF_FALSE(execSelect(queryOperation, handler));

    dbObject.reset();

    if(!handler->accessQuery().next())
    {
        // There is no element in the te database with the id given
        return true;
    }

    QHash<QString, int> idxByNames;
    RETURN_IF_FALSE(getColumnIdxsInQueryByNames<T>(handler->accessQuery(), idxByNames));

    dbObject = QSharedPointer<T>::create();
    return fillFromQuery(handler->accessQuery(), idxByNames, dbObject);
}

template<class T>
bool DbManager::selectOne(qint64 id, QSharedPointer<T> &dbObject) const
{
    QString queryStr = QString(SelectQuery).arg(T::tableName).arg(QString(NameAndValue)
                                                                              .arg(DbObject::idName)
                                                                              .arg(id));
    return selectOne(queryStr, dbObject);
}

template<class T>
bool DbManager::getColumnIdxsInQueryByNames(const QSqlQuery &sqlQuery,
                                            QHash<QString, int> &idxByNames)
{
    static_assert(std::is_base_of_v<DbObject, T>,
                  "Error : class passed as template is not of type 'DbObject'");

    return getColumnIdxsInQueryByNames(T::staticMetaObject, sqlQuery, idxByNames);
}

template<class T>
bool DbManager::fillFromQuery(const QSqlQuery &sqlQuery,
                              const QHash<QString, int> &idxByNames,
                              QSharedPointer<T> &dbObject) const
{
    if(!sqlQuery.isValid())
    {
        qWarning() << "The current record of the sql query is invalid, can't fill the given "
                   << "DbObject";
        return false;
    }

    QHash<QString, int>::const_iterator citer = idxByNames.cbegin();
    for(; citer != idxByNames.cend(); ++citer)
    {
        dbObject->setProperty(citer.key().toLatin1().constData(), sqlQuery.value(citer.value()));
    }

    return true;
}
