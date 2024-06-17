// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "recbuttonbloc.hpp"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QEvent>
#include <QQmlEngine>


RecButtonBloc::RecButtonBloc(QObject *parent)
    : ABasePageBloc{parent}
{
}

RecButtonBloc::~RecButtonBloc()
{
}

void RecButtonBloc::click(QObject *button)
{
    QPointF pos(0, 0);

    // We simulate a click on the button
    QMouseEvent* down = new QMouseEvent(
        QMouseEvent::Type::MouseButtonPress,
        pos,
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::KeyboardModifier::NoModifier);

    QMouseEvent* up = new QMouseEvent(
        QMouseEvent::Type::MouseButtonRelease,
        pos,
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::KeyboardModifier::NoModifier);

    QCoreApplication::postEvent(button, down);
    QCoreApplication::postEvent(button, up);
}

void RecButtonBloc::registerQmlTypes()
{
    qmlRegisterType<RecButtonBloc>("app.blocs", 1, 0, "RecButtonBloc");
}
