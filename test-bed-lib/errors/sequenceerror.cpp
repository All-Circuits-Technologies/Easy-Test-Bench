// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/sequenceerror.hpp"

#include <QDebug>


const char *SequenceError::getTrKey(SequenceError::Enum sequenceError)
{
    switch(sequenceError)
    {
        case FailedNodesSequence :
            return failedNodesSequenceKey;

        case FailedUnknownNodesSequence :
            return failedUnknownNodesSequenceKey;

        case FailedChildNode :
            return failedChildNodeKey;

        case FailedUnknownChildNode :
            return failedUnknownChildNodeKey;
    }

    qWarning() << "This kind of sequence error: " << sequenceError
               << ", doesn't exist or it's not managed";
    return unknownErrorKey;
}
