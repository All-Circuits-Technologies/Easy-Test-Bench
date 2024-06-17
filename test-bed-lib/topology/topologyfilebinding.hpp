// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilebindingpin.hpp"

#include <QVector>


/** @brief This class represents a Topology > Bindings > Binding node.
    This JSON node lists a set of Pins to mark them as connected 'same wire)

    @see TopologyFileRoute */
class TopologyFileBinding : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QVector<TopologyFileBindingPin>, Pins)  /**< @brief Connected pins */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileBinding();

        /** @brief All-options constructor
            @param pins Electrically-connected pins */
        TopologyFileBinding(const QVector<TopologyFileBindingPin> &pins);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileBinding)
