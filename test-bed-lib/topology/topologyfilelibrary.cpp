// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilelibrary.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFileLibrary::TopologyFileLibrary() :
    TopologyFileLibrary(QVector<TopologyFileComponent>())
{ }

TopologyFileLibrary::TopologyFileLibrary(const QVector<TopologyFileComponent> &components) :
    BmsGadget(),
    _Components(components)
{ }

void TopologyFileLibrary::RegisterMetaTypes()
{
    TopologyFileComponent::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileComponent>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileComponent);

    qRegisterMetaType<TopologyFileLibrary>();
    QMetaType::registerComparators<TopologyFileLibrary>();
}

const QMetaObject *TopologyFileLibrary::metaObject() const
{
    return &staticMetaObject;
}
