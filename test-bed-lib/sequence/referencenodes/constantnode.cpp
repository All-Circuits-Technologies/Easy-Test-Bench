// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "constantnode.hpp"

#include <QDebug>

#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "sequence/nodessequence.hpp"


ConstantNode::ConstantNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    AReferenceNode(NodeType::ConstantNode, sequenceParent, parent)
{
}

bool ConstantNode::setConstantValue(const QVariant &value,
                                    SequenceLoadingErrorHandler *errorHandler)
{
    if(getInstanceParent()->isSequenceRunning())
    {
        FileLoadingErrorCauseHelper::addSetConstantValueWhileSequenceRunningError(value.toString(),
                                                                                  errorHandler);
        qWarning() << "Can't set the value of the node constant: " << getInstanceName() << ", when "
                   << "sequence is running";
        return false;
    }

    return setValue(value);
}
