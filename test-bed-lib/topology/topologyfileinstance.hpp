// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfileinstanceparam.hpp"

#include <QString>
#include <QVector>


/** @brief This class represents a single Topology>Instances>Instance JSON node.

    Component versus instance
    =========================

    This topology instance represents one of devices you own.
    This is distinguished from topology components which are devices definition, like in a catalog.
    There is always at most one "component" object per device, let say "Keithley 2000",
    but you can own zero or several such component (called instances), let say "My multimeter k2k".

    @see TopologyFileComponent */
class TopologyFileInstance : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, Name)       /**< @brief Instance name        (ex: "My Keithley") */
    SIMPLE_PROPERTY(QString, Component)  /**< @brief Component definition (ex: "Keithley 2000") */
    SIMPLE_PROPERTY(QVector<TopologyFileInstanceParam>, Params)  /**< @brief Parameters */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileInstance();

        /** @brief All-options constructor
            @param name Name of this pin
            @param component Kind of this pin (used as a pin-to-pin compatibility filter)
            @param params Is this pin an output/input/inout of the component */
        TopologyFileInstance(const QString &name,
                             const QString &component,
                             const QVector<TopologyFileInstanceParam> &params);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileInstance)
