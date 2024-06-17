// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilebindingpin.hpp"
#include "topology/topologyfileroutepoint.hpp"

#include <QVector>


/** @brief This class represent a single Topology>Routes>Route node.

    A route is a known/prefered way to connetc to distant pins.

    Route versus binding
    ====================

     - A binding is a wire, ie an directly connected pins.
     - A route is a way to connect to pins which are not directly connected by a single wire

    @see TopologyFileBinding */
class TopologyFileRoute : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(TopologyFileBindingPin, FromPin)    /**< @brief From */
    SIMPLE_PROPERTY(TopologyFileBindingPin, ToPin)      /**< @brief To */
    SIMPLE_PROPERTY(QVector<TopologyFileRoutePoint>, Path)  /**< @brief Hops */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileRoute();

        /** @brief All-options constructor
            @param fromPin Route start pin
            @param toPin Route target pin
            @param path Route hops */
        TopologyFileRoute(const TopologyFileBindingPin &fromPin,
                          const TopologyFileBindingPin &toPin,
                          const QVector<TopologyFileRoutePoint> &path);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileRoute)
