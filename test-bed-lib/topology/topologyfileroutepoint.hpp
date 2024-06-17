// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"

#include <QString>


/** @brief This file represents a single Topology>Routes>Route>Path item.

    It describes a hop, ie to pins of a single component which are or can be internally connected.

    @see TopologyFileRoute */
class TopologyFileRoutePoint : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, Instance)  /**< @brief Instance name       (ex: "My Keithley") */
    SIMPLE_PROPERTY(QString, FromPin)   /**< @brief Source Pin          (ex: "ROW3") */
    SIMPLE_PROPERTY(QString, ToPin)     /**< @brief Target pin          (ex: "COL4") */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileRoutePoint();

        /** @brief All-options constructor
            @param instance Name of an instance
            @param fromPin Name of a pin of this instance
            @param toPin Name of a second pin of this instance */
        TopologyFileRoutePoint(const QString &instance,
                               const QString &fromPin,
                               const QString &toPin);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileRoutePoint)
