// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilebinding.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileBinding::TopologyFileBinding() :
    TopologyFileBinding(QVector<TopologyFileBindingPin>())
{ }

TopologyFileBinding::TopologyFileBinding(const QVector<TopologyFileBindingPin> &pins) :
    BmsGadget(),
    _Pins(pins)
{ }

void TopologyFileBinding::RegisterMetaTypes()
{
    TopologyFileBindingPin::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileBindingPin>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileBindingPin);

    qRegisterMetaType<TopologyFileBinding>();
    QMetaType::registerComparators<TopologyFileBinding>();
}

const QMetaObject *TopologyFileBinding::metaObject() const
{
    return &staticMetaObject;
}
