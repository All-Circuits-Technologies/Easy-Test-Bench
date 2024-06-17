// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilebinding.hpp"

#include <QVector>


/** @brief This file represent a Topology > Bindings file (ie a set of bindings)

    @see TopologyFileRoutes */
class TopologyFileBindings : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QVector<TopologyFileBinding>, Bindings)  /**< @brief Bindings */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileBindings();

        /** @brief All-options constructor
            @param bindings Bindings this file contains */
        TopologyFileBindings(const QVector<TopologyFileBinding> &bindings);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileBindings)
