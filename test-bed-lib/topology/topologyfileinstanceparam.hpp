// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"


/** @brief This class represents a single parameter for an instance.

    Each instance parameter somewhat links to a component parameter (by its name)
    to assign it a value.

    Parameter value can be either string|bool|int on JSON side.
    To handle it we choosen QVariant type for our value.

    @see TopologyFileInstance
    @see TopologyFileComponent */
class TopologyFileInstanceParam : public BmsGadget
{
    Q_GADGET
    SIMPLE_PROPERTY(QString, Key)      /**< @brief Parameter name   (ex: "exclusive") */
    SIMPLE_PROPERTY(QVariant, Value)   /**< @brief Parameter value  (ex: true) */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFileInstanceParam();

        /** @brief Full-option constructor
            @param key Name of the parameter
            @param value Value to assign for this parameter */
        TopologyFileInstanceParam(const QString &key,
                                  const QVariant &value);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFileInstanceParam)
