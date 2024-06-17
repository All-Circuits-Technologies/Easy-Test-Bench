// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QMetaObject>


/** @brief This class create a simple super-class to our Q_GADGETS.

    @note Most of Qt object are Q_OBJECTs and share common superclass QObject.
          Lighter Q_GADGET has no superclass requirements, but we like having one
          in order to simplify a bit a few algorithms, especially by adding metaObject
          member getter.

    @note Please ensure your leaf sub-classes contains Q_GADGET macro.
          Also, read @ref BmsGadgetJsonLoader doxygen for extra requirements if you use this loader.

    @see BmsGadgetJsonLoader */
class BmsGadget
{
    Q_GADGET

    protected:
        /** @brief Simple default constructor */
        explicit BmsGadget() {}

    public:
        /** @brief Destroy a gadget
            @note We need to declare it explicitely because we want it to be virtual,
                  which requires its implementation to be moved into cpp file by the way. */
        virtual ~BmsGadget() = default;

    public:
        /** @brief Equality operator.
            This method loop over all Qt properties of this (aka left-hand-side) and rhs,
            compare them all and return true if they equals.
            @see QMetaType::registerComparators
            @see BmsObjectJsonLoaderTest
            @param rhs Right-hand side of the operator */
        bool operator==(const BmsGadget &rhs) const;

        /** @brief Ordering operator.
            This method loop over all Qt properties of this  (aka left-hand-side) and rhs,
            compare them one-by-one and return true on first strict inferiority result.
            @see QMetaType::registerComparators
            @see BmsObjectJsonLoaderTest
            @param rhs Right-hand side of the operator */
        bool operator<(const BmsGadget &rhs) const;

    public:
        /** @brief Simple instance-based meta object getter
            @note Please implement it and return your own &staticMetaObject */
        virtual const QMetaObject *metaObject() const = 0;
};

Q_DECLARE_METATYPE(BmsGadget*)
