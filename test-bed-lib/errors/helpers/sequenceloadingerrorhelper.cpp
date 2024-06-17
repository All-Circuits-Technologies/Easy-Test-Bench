// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/helpers/sequenceloadingerrorhelper.hpp"

#include "errors/fileloadingerrorhandler.hpp"
#include "errors/sequenceerror.hpp"
#include "definesutility/definesutility.hpp"


void SequenceLoadingErrorHelper::addFailedNodesSequenceError(FileLoadingErrorHandler *errorHandler,
                                                             const QString &nodeName)
{
    RETURN_VOID_IF_NULL(errorHandler);

    if(nodeName.isEmpty())
    {
        addErrorDescriptor(ErrorLayer::FailedChildNode,
                           SequenceError::FailedUnknownNodesSequence,
                           *errorHandler);
    }
    else
    {
        addErrorDescriptor(ErrorLayer::FailedChildNode,
                           SequenceError::FailedNodesSequence,
                           *errorHandler,
                           { nodeName });
    }

}

void SequenceLoadingErrorHelper::addFailedChildNodeError(const QString &nodeName,
                                                         NodeType::Enum nodeType,
                                                         FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    QVector<QVariant> parameters = { NodeType::toString(nodeType) };

    if(nodeName.isEmpty())
    {
        addErrorDescriptor(ErrorLayer::FailedChildNode,
                           SequenceError::FailedUnknownChildNode,
                           *errorHandler,
                           parameters);
    }    
    else
    {
        parameters.append(nodeName);
        addErrorDescriptor(ErrorLayer::FailedChildNode,
                           SequenceError::FailedChildNode,
                           *errorHandler,
                           parameters);
    }
}
