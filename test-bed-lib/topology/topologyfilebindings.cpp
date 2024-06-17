// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilebindings.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileBindings::TopologyFileBindings() :
    TopologyFileBindings(QVector<TopologyFileBinding>())
{ }

TopologyFileBindings::TopologyFileBindings(const QVector<TopologyFileBinding> &bindings) :
    BmsGadget(),
    _Bindings(bindings)
{ }

void TopologyFileBindings::RegisterMetaTypes()
{
    TopologyFileBinding::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileBinding>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileBinding);

    qRegisterMetaType<TopologyFileBindings>();
    QMetaType::registerComparators<TopologyFileBindings>();
}

const QMetaObject *TopologyFileBindings::metaObject() const
{
    return &staticMetaObject;
}
