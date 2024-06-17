// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilecomponentpin.hpp"

#include <QMetaType>
#include <QString>
#include <QVector>


/** @brief This class represents a single Topology>Component>PinGroup JSON node */
class TopologyFileComponentPinGroup : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, GroupName)                     /**< @brief Group name, ex: "slots" */
    SIMPLE_PROPERTY(QVector<TopologyFileComponentPin>, Elements)          /**< @brief Pins list */

    public:
        /** @brief Empty invokable constructor useful for automatic loading of JSON node */
        Q_INVOKABLE explicit TopologyFileComponentPinGroup();

        /** @brief All-options constructor
            @param groupName Name of this component pin group
            @param elements  Pins inside this group */
        TopologyFileComponentPinGroup(const QString &groupName,
                                      const QVector<TopologyFileComponentPin> &elements);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileComponentPinGroup)
