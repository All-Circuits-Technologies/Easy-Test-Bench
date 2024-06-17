// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "innodeport.hpp"

#include <QDebug>

#include "sequence/referencenodes/areferencenode.hpp"
#include "testbedcore/testbedglobal.hpp"


InNodePort::InNodePort(QObject *parent) :
    ANodePort(parent)
{
}

InNodePort::InNodePort(const InNodePort &inPort) :
    ANodePort(inPort),
    _inputNode(inPort._inputNode),
    _internalInputNode(inPort._internalInputNode),
    _canBeUndefined(inPort._canBeUndefined)
{
}

InNodePort &InNodePort::operator=(const InNodePort &inPort)
{
    _inputNode          = inPort._inputNode;
    _internalInputNode  = inPort._internalInputNode;
    _canBeUndefined     = inPort._canBeUndefined;

    return *this;
}

bool InNodePort::setInputNode(const QSharedPointer<AReferenceNode> &referenceNode,
                              SequenceLoadingErrorHandler */*errorHandler*/)
{
    if(!referenceNode.isNull())
    {
        RETURN_IF_FALSE(validateReference(referenceNode));
    }

    _inputNode = referenceNode;
    return true;
}

bool InNodePort::setInternalInputNode(const QSharedPointer<AReferenceNode> &internalInputNode)
{
    if(!internalInputNode.isNull())
    {
        RETURN_IF_FALSE(validateReference(internalInputNode));
    }

    _internalInputNode = internalInputNode;
    return true;
}

bool InNodePort::operator==(const InNodePort &nodePort) const
{
    return (ANodePort::operator==(nodePort)                     &&
            (_inputNode         == nodePort._inputNode)         &&
            (_internalInputNode == nodePort._internalInputNode) &&
            (_canBeUndefined    == nodePort._canBeUndefined));
}
