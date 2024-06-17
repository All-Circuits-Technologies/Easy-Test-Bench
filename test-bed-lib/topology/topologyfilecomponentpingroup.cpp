// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilecomponentpingroup.hpp"

#include "collectionutility/qvariantlisthelper.hpp"

#include <QMetaType>


TopologyFileComponentPinGroup::TopologyFileComponentPinGroup() :
    TopologyFileComponentPinGroup(QString(), {})
{ }

TopologyFileComponentPinGroup::TopologyFileComponentPinGroup(
                                                const QString &groupName,
                                                const QVector<TopologyFileComponentPin> &elements) :
    BmsGadget(),
    _GroupName(groupName),
    _Elements(elements)
{ }

void TopologyFileComponentPinGroup::RegisterMetaTypes()
{
    TopologyFileComponentPin::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileComponentPin>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector, TopologyFileComponentPin);

    qRegisterMetaType<TopologyFileComponentPinGroup>();
    QMetaType::registerComparators<TopologyFileComponentPinGroup>();
}

const QMetaObject *TopologyFileComponentPinGroup::metaObject() const
{
    return &staticMetaObject;
}
