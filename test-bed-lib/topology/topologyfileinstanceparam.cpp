// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileinstanceparam.hpp"


TopologyFileInstanceParam::TopologyFileInstanceParam() :
    TopologyFileInstanceParam(QString(), QString())
{ }

TopologyFileInstanceParam::TopologyFileInstanceParam(const QString &key, const QVariant &value) :
   BmsGadget(),
    _Key(key),
    _Value(value)
{ }

void TopologyFileInstanceParam::RegisterMetaTypes()
{
    qRegisterMetaType<TopologyFileInstanceParam>();
    QMetaType::registerComparators<TopologyFileInstanceParam>();
}

const QMetaObject *TopologyFileInstanceParam::metaObject() const
{
    return &staticMetaObject;
}
