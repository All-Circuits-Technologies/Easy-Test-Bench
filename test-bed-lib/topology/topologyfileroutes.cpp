// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileroutes.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileRoutes::TopologyFileRoutes() :
    TopologyFileRoutes(QVector<TopologyFileRoute>())
{ }

TopologyFileRoutes::TopologyFileRoutes(const QVector<TopologyFileRoute> &routes) :
    BmsGadget(),
    _Routes(routes)
{ }

void TopologyFileRoutes::RegisterMetaTypes()
{
    TopologyFileRoute::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileRoute>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileRoute);

    qRegisterMetaType<TopologyFileRoutes>();
    QMetaType::registerComparators<TopologyFileRoutes>();
}

const QMetaObject *TopologyFileRoutes::metaObject() const
{
    return &staticMetaObject;
}
