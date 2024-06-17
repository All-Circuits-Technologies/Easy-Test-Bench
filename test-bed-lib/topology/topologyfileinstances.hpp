// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfileinstance.hpp"

#include <QVector>


/** @brief This class represents a single project topology sub-file.
    Such files contains a set of topology instances. */
class TopologyFileInstances : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QVector<TopologyFileInstance>, Instances)  /**< @brief Instances */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileInstances();

        /** @brief Full features conctructor
            @param instances Instances included in this file */
        explicit TopologyFileInstances(const QVector<TopologyFileInstance> &instances);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileInstances)
