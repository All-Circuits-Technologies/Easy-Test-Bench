// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"


/** @brief This class represents a single Topology>Bindings>Binding>BindingPin JSON node.
    Each instance of this node represents the pin of an instance.
    They are listed in a upper Binding node which mark them as connected */
class TopologyFileBindingPin : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, InstanceName)
    SIMPLE_PROPERTY(QString, PinName)

    public:
        /** @brief Empty invokable constructor useful for automatic loading of JSON node */
        Q_INVOKABLE explicit TopologyFileBindingPin();

        /** @brief All-options constructor
            @param instanceName Name of the instance the pin belongs to
            @param pinName  Name of the pin inside given instance */
        TopologyFileBindingPin(const QString &instanceName,
                               const QString &pinName);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileBindingPin)
