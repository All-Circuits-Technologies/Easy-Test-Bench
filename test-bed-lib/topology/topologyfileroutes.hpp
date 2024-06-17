// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfileroute.hpp"

#include <QVector>


/** @brief This class represents a single Topology>Routes JSON node.

    Routes versus bindings
    ======================

     - A binding is a wire, ie an directly connected pins.
     - A route is a way to connect to pins which are not directly connected by a single wire

    @see TopologyFileBindings */
class TopologyFileRoutes : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QVector<TopologyFileRoute>, Routes)  /**< @brief Routes */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileRoutes();

        /** @brief All-options constructor
            @param routes Routes listed in this JSON file */
        TopologyFileRoutes(const QVector<TopologyFileRoute> &routes);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileRoutes)
