// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"


/** @brief This class represents a single Topology>Component>PinGroup>Pin JSON node */
class TopologyFileComponentPin : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, Label)      /**< @brief Pin name       (ex: "GP-IB") */
    SIMPLE_PROPERTY(QString, Kind)       /**< @brief Pin "mime"     (ex: "stream/ieee488") */
    SIMPLE_PROPERTY(QString, Direction)  /**< @brief Pin direction  (ex: "output") */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileComponentPin();

        /** @brief All-options constructor
            @param label Name of this pin
            @param kind Kind of this pin (used as a pin-to-pin compatibility filter)
            @param direction Is this pin an output/input/inout of the component */
        TopologyFileComponentPin(const QString &label,
                                 const QString &kind,
                                 const QString &direction);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileComponentPin)
