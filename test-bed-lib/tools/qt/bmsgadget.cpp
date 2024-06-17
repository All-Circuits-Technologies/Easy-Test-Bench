// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bmsgadget.hpp"

#include "tools/qt/qobjecthelper.hpp"

#include <QDebug>


bool BmsGadget::operator==(const BmsGadget &rhs) const
{
    const BmsGadget &lhs = *this;               // Left-hand-side of the operator
    const bool failureResult = false;

    QObjectHelper::Properties lhsProperties;    // Left-hand-side properties
    QObjectHelper::Properties rhsProperties;    // Right-hand-side properties

    if(!QObjectHelper::getAllProperties(lhsProperties,
                                        &lhs,
                                        lhs.metaObject(),
                                        &QObject::staticMetaObject))
    {
        qWarning() << "Failed to extract left-hand-side properties";
        return failureResult;
    }

    if(!QObjectHelper::getAllProperties(rhsProperties,
                                        &rhs,
                                        rhs.metaObject(),
                                        &QObject::staticMetaObject))
    {
        qWarning() << "Failed to extract right-hand-side properties";
        return failureResult;
    }

    return lhsProperties == rhsProperties;
}

bool BmsGadget::operator<(const BmsGadget &rhs) const
{
    /* This is not easy to create a generic ordering method here,
     * but operator< is marked as mandatory for QMetaType::registerComparators
     * hence we create one here, which tests all properties (excluding QObject objectName if any)
     * in the order there were declared.
     */
    const BmsGadget &lhs = *this;               // Left-hand-side of the operator
    const bool failureResult = false;

    QObjectHelper::Properties lhsProperties;    // Left-hand-side properties
    QObjectHelper::Properties rhsProperties;    // Right-hand-side properties

    if(!QObjectHelper::getAllProperties(lhsProperties,
                                        &lhs,
                                        &BmsGadget::staticMetaObject,
                                        &QObject::staticMetaObject))
    {
        qWarning() << "Failed to extract left-hand-side properties";
        return failureResult;
    }

    if(!QObjectHelper::getAllProperties(rhsProperties,
                                        &rhs,
                                        &BmsGadget::staticMetaObject,
                                        &QObject::staticMetaObject))
    {
        qWarning() << "Failed to extract right-hand-side properties";
        return failureResult;
    }

    if(lhsProperties.count() != rhsProperties.count())
    {
        qWarning() << "Both items do not have the same property count";
        return failureResult;
    }

    for(int i = 0; i < lhsProperties.count(); ++i)
    {
        const QString &lhsPropertyKey = lhsProperties[i].first;
        const QVariant &lhsPropertyValue = lhsProperties[i].second;
        const QString &rhsPropertyKey = rhsProperties[i].first;
        const QVariant &rhsPropertyValue = rhsProperties[i].second;

        if(lhsPropertyKey != rhsPropertyKey)
        {
            qWarning() << "Property key mismatch";
            return failureResult;
        }

        if(lhsPropertyValue < rhsPropertyValue)
        {
            return true;
        }

        // Lets loop to next property
    }

    // No right-hand-side properties value were strictly lesser than right-hand-side
    return false;
}
