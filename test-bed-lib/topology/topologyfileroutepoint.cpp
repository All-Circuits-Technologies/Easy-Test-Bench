// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileroutepoint.hpp"


TopologyFileRoutePoint::TopologyFileRoutePoint() :
    TopologyFileRoutePoint(QString(), QString(), QString())
{ }

TopologyFileRoutePoint::TopologyFileRoutePoint(const QString &instance,
                                               const QString &fromPin,
                                               const QString &toPin) :
    BmsGadget(),
    _Instance(instance),
    _FromPin(fromPin),
    _ToPin(toPin)
{ }

void TopologyFileRoutePoint::RegisterMetaTypes()
{
    qRegisterMetaType<TopologyFileRoutePoint>();
    QMetaType::registerComparators<TopologyFileRoutePoint>();
}

const QMetaObject *TopologyFileRoutePoint::metaObject() const
{
    return &staticMetaObject;
}
