// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "displayuitoken.hpp"

#include <QDebug>
#include <QSharedPointer>
#include <QUuid>


DisplayUiToken::DisplayUiToken(QObject *parent) :
    QObject(parent)
{
    _uuid = QUuid::createUuid().toString();
}

DisplayUiToken::DisplayUiToken(const DisplayUiToken &uiToken) :
    DisplayUiToken()
{
    _finished = uiToken._finished;
    _uuid = uiToken._uuid;
}

DisplayUiToken::~DisplayUiToken()
{
}

void DisplayUiToken::finish(bool success, const JsonArray &valuesSet)
{
    if(_finished)
    {
        qWarning() << "The token is already finished";
        return;
    }

    _finished = true;
    emit finished(success, _uuid, valuesSet);
}

void DisplayUiToken::RegisterMetaTypes()
{
    qRegisterMetaType<DisplayUiToken>();
    qRegisterMetaType<QSharedPointer<DisplayUiToken>>();
}

QSharedPointer<DisplayUiToken> DisplayUiToken::generateToken()
{
    return QSharedPointer<DisplayUiToken>(new DisplayUiToken());
}

void DisplayUiToken::cancelDisplay()
{
    _canceled = true;
    emit closeWidget();
}
