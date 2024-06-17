// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "adisplayuserinterface.hpp"

#include <QCoreApplication>
#include <QSharedPointer>

#include "bank/displayuitoken.hpp"


ADisplayUserInterface::ADisplayUserInterface(QObject *parent) :
    QObject(parent)
{
    if(QCoreApplication::instance()->thread() != thread())
    {
        qWarning() << "The ADisplayUserInterface has to be created in the GUI thread (if it exists)"
                   << " or the main thread";
    }

    connect(this, &ADisplayUserInterface::_displayWidget,
            this, &ADisplayUserInterface::displayWidgetImpl);
    connect(this, &ADisplayUserInterface::_updateWidget,
            this, &ADisplayUserInterface::updateWidgetImpl);
}

ADisplayUserInterface::~ADisplayUserInterface()
{
}

QSharedPointer<DisplayUiToken> ADisplayUserInterface::displayWidget(const JsonObject &elemToDisplay)
{
    QSharedPointer<DisplayUiToken> uiToken = DisplayUiToken::generateToken();

    emit _displayWidget(uiToken, elemToDisplay);

    return uiToken;
}

void ADisplayUserInterface::updateWidget(const QSharedPointer<DisplayUiToken> &uiToken,
                                         const JsonObject &elemToUpdate)
{
    emit _updateWidget(uiToken, elemToUpdate);
}
