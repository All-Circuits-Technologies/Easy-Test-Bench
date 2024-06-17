// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilecomponent.hpp"

#include "collectionutility/qvariantlisthelper.hpp"

TODO("Maybe mute Category property into an enum?")


TopologyFileComponent::TopologyFileComponent() :
    TopologyFileComponent(QString(), QString(), QString(), QString(), {})
{ }

TopologyFileComponent::TopologyFileComponent(const QString &name,
                                             const QString &description,
                                             const QString &type,
                                             const QString &category,
                                             const QVector<TopologyFileComponentPinGroup> &pins) :
    BmsGadget(),
    _Name(name),
    _Description(description),
    _Type(type),
    _Category(category),
    _Pins(pins)
{ }

void TopologyFileComponent::RegisterMetaTypes()
{
    TopologyFileComponentPinGroup::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileComponentPinGroup>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileComponentPinGroup);

    qRegisterMetaType<TopologyFileComponent>();
    QMetaType::registerComparators<TopologyFileComponent>();
}

const QMetaObject *TopologyFileComponent::metaObject() const
{
    return &staticMetaObject;
}
