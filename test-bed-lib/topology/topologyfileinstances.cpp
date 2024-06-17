// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileinstances.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileInstances::TopologyFileInstances() :
    TopologyFileInstances(QVector<TopologyFileInstance>())
{ }

TopologyFileInstances::TopologyFileInstances(const QVector<TopologyFileInstance> &instances) :
    BmsGadget(),
    _Instances(instances)
{ }

void TopologyFileInstances::RegisterMetaTypes()
{
    TopologyFileInstance::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileInstance>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileInstance);

    qRegisterMetaType<TopologyFileInstances>();
    QMetaType::registerComparators<TopologyFileInstances>();
}

const QMetaObject *TopologyFileInstances::metaObject() const
{
    return &staticMetaObject;
}
