// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilecomponentpin.hpp"

#include <QMetaType>

TODO("Maybe mute direction property into an enum?")


TopologyFileComponentPin::TopologyFileComponentPin() :
    TopologyFileComponentPin(QString(), QString(), QString())
{ }

TopologyFileComponentPin::TopologyFileComponentPin(const QString &label,
                                                   const QString &kind,
                                                   const QString &direction) :
    BmsGadget(),
    _Label(label),
    _Kind(kind),
    _Direction(direction)
{ }

void TopologyFileComponentPin::RegisterMetaTypes()
{
    qRegisterMetaType<TopologyFileComponentPin>();
    QMetaType::registerComparators<TopologyFileComponentPin>();
}

const QMetaObject *TopologyFileComponentPin::metaObject() const
{
    return &staticMetaObject;
}
