// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/abstractbloc.hpp"

#include <QTimer>


AbstractBloc::AbstractBloc(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, this, &AbstractBloc::initBloc);
}

AbstractBloc::~AbstractBloc()
{
}
