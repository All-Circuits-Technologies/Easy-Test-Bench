// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileroute.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileRoute::TopologyFileRoute() :
    TopologyFileRoute(TopologyFileBindingPin(), TopologyFileBindingPin(), {})
{ }

TopologyFileRoute::TopologyFileRoute(const TopologyFileBindingPin &fromPin,
                                     const TopologyFileBindingPin &toPin,
                                     const QVector<TopologyFileRoutePoint> &path) :
    BmsGadget(),
    _FromPin(fromPin),
    _ToPin(toPin),
    _Path(path)
{ }

void TopologyFileRoute::RegisterMetaTypes()
{
    TopologyFileRoutePoint::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileRoutePoint>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileRoutePoint);

    qRegisterMetaType<TopologyFileRoute>();
    QMetaType::registerComparators<TopologyFileRoute>();
}

const QMetaObject *TopologyFileRoute::metaObject() const
{
    return &staticMetaObject;
}
