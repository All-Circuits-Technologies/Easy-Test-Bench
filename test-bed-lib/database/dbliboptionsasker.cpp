// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbliboptionsasker.hpp"

#include <QVersionNumber>

#include "database/dbmanager.hpp"
#include "models/liboptions.hpp"


DbLibOptionsAsker::DbLibOptionsAsker(DbManager &dbManager, QObject *parent) :
    QObject(parent),
    _dbManager(dbManager)
{
}

bool DbLibOptionsAsker::getLibOptions(QSharedPointer<LibOptions> &libOptions) const
{
    const QString QueryOperation(SelectLibOptionsWithMaxId);

    if(!_dbManager.selectOne<LibOptions>(QueryOperation, libOptions))
    {
        qWarning() << "Can't get lib options from the database";
        return false;
    }

    return true;
}

bool DbLibOptionsAsker::updateLibOptions(const QSharedPointer<LibOptions> &libOptions)
{
    if(!_dbManager.update(libOptions))
    {
        qWarning() << "Can't update lib options in the database";
        return false;
    }

    return true;
}
