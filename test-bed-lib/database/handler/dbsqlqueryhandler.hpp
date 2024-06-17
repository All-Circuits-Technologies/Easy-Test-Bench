// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "database/handler/dbhandler.hpp"

class DbHandlerManager;
class QSqlQuery;


/** @brief Helpful handler which also contains a SqlQuery */
class DbSqlQueryHandler : public DbHandler
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param handlerManager The handler manager which manages this handler
            @param connectionName The connection name linked to this handler
            @param parent The parent instance */
        explicit DbSqlQueryHandler(DbHandlerManager &handlerManager,
                                   const QString &connectionName =
                                                    QLatin1String(QSqlDatabase::defaultConnection),
                                   QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DbSqlQueryHandler() override;

    public:
        /** @brief Access SqlQuery */
        QSqlQuery &accessQuery() { return *_sqlQuery; }

    private:
        QSqlQuery *_sqlQuery{nullptr};

};
