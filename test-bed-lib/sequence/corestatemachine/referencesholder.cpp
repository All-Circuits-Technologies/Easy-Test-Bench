// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "referencesholder.hpp"

#include "collectionutility/qvectorhelper.hpp"


ReferencesHolder::ReferencesHolder(ReferencesHolderType::Enum refHolderType, QObject *parent) :
    QObject(parent),
    _referenceHolderType(refHolderType)
{
}

bool ReferencesHolder::operator==(const ReferencesHolder &refHolder) const
{
    if(_referenceHolderType != refHolder._referenceHolderType)
    {
        return false;
    }

    return (QVectorHelper::compareList(_inputs, refHolder._inputs, false)   &&
            QVectorHelper::compareList(_outputs, refHolder._outputs, false));
}
