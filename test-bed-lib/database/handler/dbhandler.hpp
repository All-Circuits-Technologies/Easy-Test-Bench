// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QtSql/QSqlDatabase>

class DbHandlerManager;


/** @brief Useful class which keeps a link to the QSqlDatabase
    @note Use the database link of this handler (and keep the handler) to maintain the connection
          open */
class DbHandler : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param handlerManager The handler manager which manages this handler
            @param connectionName The connection name linked to this handler
            @param parent The parent instance */
        explicit DbHandler(DbHandlerManager &handlerManager,
                           const QString &connectionName =
                                                    QLatin1String(QSqlDatabase::defaultConnection),
                           QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbHandler() override;

    public:
        /** @brief Access SqlDatabase */
        QSqlDatabase &accessDb();

    private:
        DbHandlerManager &_handlerManager;
        QSqlDatabase _db;
        QString _connectionName;
};
