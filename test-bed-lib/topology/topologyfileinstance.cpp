// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfileinstance.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileInstance::TopologyFileInstance() :
    TopologyFileInstance(QString(), QString(), {})
{ }

TopologyFileInstance::TopologyFileInstance(const QString &name,
                                           const QString &component,
                                           const QVector<TopologyFileInstanceParam> &params) :
    BmsGadget(),
    _Name(name),
    _Component(component),
    _Params(params)
{ }

void TopologyFileInstance::RegisterMetaTypes()
{
    TopologyFileInstanceParam::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileInstanceParam>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileInstanceParam);

    qRegisterMetaType<TopologyFileInstance>();
    QMetaType::registerComparators<TopologyFileInstance>();
}

const QMetaObject *TopologyFileInstance::metaObject() const
{
    return &staticMetaObject;
}
