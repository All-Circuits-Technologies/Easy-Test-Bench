// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "outnodeport.hpp"

#include <QDebug>

#include "sequence/referencenodes/variablenode.hpp"
#include "testbedcore/testbedglobal.hpp"


OutNodePort::OutNodePort(QObject *parent) :
    ANodePort(parent)
{
}

OutNodePort::OutNodePort(const OutNodePort &outPort) :
    ANodePort(outPort)
{
    _outputNode         = outPort._outputNode;
    _internalOutputNode = outPort._internalOutputNode;
}

OutNodePort &OutNodePort::operator=(const OutNodePort &outPort)
{
    _outputNode         = outPort._outputNode;
    _internalOutputNode = outPort._internalOutputNode;

    return *this;
}

bool OutNodePort::setOutputNode(const QSharedPointer<VariableNode> &variableNode)
{
    if(!variableNode.isNull())
    {
        RETURN_IF_FALSE(validateReference(variableNode));
    }

    _outputNode = variableNode;
    return true;
}

bool OutNodePort::setInternalOutputNode(const QSharedPointer<VariableNode> &internalOutputNode)
{
    if(!internalOutputNode.isNull())
    {
        RETURN_IF_FALSE(validateReference(internalOutputNode));
    }

    _internalOutputNode = internalOutputNode;
    return true;
}

bool OutNodePort::operator==(const OutNodePort &nodePort) const
{
    return (ANodePort::operator==(nodePort)                             &&
            (_outputNode            == nodePort._outputNode)            &&
            (_internalOutputNode    == nodePort._internalOutputNode));
}
