// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "elapsedtimermanager.hpp"

#include <QElapsedTimer>
#include <QUuid>

#include "testbedcore/testbedglobal.hpp"


ElapsedTimerManager::ElapsedTimerManager(QObject *parent) : QObject(parent)
{
}

ElapsedTimerManager::~ElapsedTimerManager()
{
    qDeleteAll(_elapsedTimers);
}

const QString &ElapsedTimerManager::createAnElapsedTimer()
{
    QString token = QUuid::createUuid().toString();
    return _elapsedTimers.insert(token, new QElapsedTimer()).key();
}

bool ElapsedTimerManager::fireAnElapsedTimer(const QString &elapsedToken)
{
    if(!_elapsedTimers.contains(elapsedToken))
    {
        qWarning() << "No elapsed timer found with the token: " << elapsedToken;
        return false;
    }

    QElapsedTimer *elapsedTimer = _elapsedTimers[elapsedToken];

    if(!elapsedTimer->isValid())
    {
        elapsedTimer->start();
    }
    else
    {
        elapsedTimer->restart();
    }

    qDebug() << "Start elapsed timer: " << elapsedToken;

    return true;
}

const QElapsedTimer *ElapsedTimerManager::getAnElapsedTimer(const QString &elapsedToken) const
{
    return _elapsedTimers[elapsedToken];
}

void ElapsedTimerManager::removeElapsedTimer(const QString &elapsedToken)
{
    if(!_elapsedTimers.contains(elapsedToken))
    {
        // Nothing to do
        return;
    }

    delete _elapsedTimers[elapsedToken];
    _elapsedTimers.remove(elapsedToken);
}
