// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include <QString>
#include <QVector>

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilecomponentpingroup.hpp"

TODO("Want we rename pins into slots?")


/** @brief This class represents a single Topology>Library>Cmponent JSON object.

    Component versus instance
    =========================

    This topology component represents a device in a catalog.
    This is distinguished from topology instances which are devices you own and can use.
    There is always at most one "component" object per device, let say "Keithley 2000",
    but you can own zero or several such component (called instances), let say "My multimeter k2k".

    @see TopologyFileInstance */
class TopologyFileComponent : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, Name)          /**< @brief Component name, ex: "Keithley 2000" */
    SIMPLE_PROPERTY(QString, Description)   /**< @brief Free text */
    SIMPLE_PROPERTY(QString, Type)          /**< @brief Free-text type */
    SIMPLE_PROPERTY(QString, Category)      /**< @brief GUI grouping hint */
    SIMPLE_PROPERTY(QVector<TopologyFileComponentPinGroup>, Pins)  /**< @brief Component pins */
    /* No params in user-custom components.
     * Only components announced by a plugin can contain parameters
     */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileComponent();

        /** @brief Full option constructor
            @param name Name of this component (ex: "Keithley 2000")
            @param description Description of the component (ex: "High-precision multimeter")
            @param type Component type (ex: "???")
            @param category Component GUI grouping hint (ex: "Multimeters")
            @param pins Pin of this component (packed in groups, pins can be of any type
                                               such as test points, leds, etc) */
        TopologyFileComponent(const QString &name,
                              const QString &description,
                              const QString &type,
                              const QString &category,
                              const QVector<TopologyFileComponentPinGroup> &pins);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileComponent)
