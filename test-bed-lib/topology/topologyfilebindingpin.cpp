// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilebindingpin.hpp"

#include <QMetaType>


TopologyFileBindingPin::TopologyFileBindingPin() :
    TopologyFileBindingPin(QString(), QString())
{ }

TopologyFileBindingPin::TopologyFileBindingPin(const QString &instanceName,
                                                 const QString &pinName) :
     BmsGadget(),
    _InstanceName(instanceName),
    _PinName(pinName)
{ }

void TopologyFileBindingPin::RegisterMetaTypes()
{
    qRegisterMetaType<TopologyFileBindingPin>();
    QMetaType::registerComparators<TopologyFileBindingPin>();
}

const QMetaObject *TopologyFileBindingPin::metaObject() const
{
    return &staticMetaObject;
}
