// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "anodeport.hpp"

#include <QDebug>

#include "sequence/referencenodes/areferencenode.hpp"
#include "testbedcore/errors/helpers/fileloadingerrorcausehelper.hpp"


ANodePort::ANodePort(QObject *parent) : QObject(parent)
{
}

ANodePort::ANodePort(const ANodePort &other) :
    QObject(),
    _name(other._name),
    _unit(other._unit),
    _typeManaged(other._typeManaged)
{
}

bool ANodePort::validateReference(const QSharedPointer<AReferenceNode> &referenceNode,
                                  SequenceLoadingErrorHandler *errorHandler) const
{
    if(referenceNode)
    {
        if(referenceNode->getTypeManaged() != getTypeManaged() ||
           referenceNode->getUnit() != getUnit())
        {
            FileLoadingErrorCauseHelper::addNodePortAndRefNodeIncompatibleError(
                                                            getName(),
                                                            getTypeManaged(),
                                                            getUnit().toString(),
                                                            referenceNode->getInstanceName(),
                                                            referenceNode->getTypeManaged(),
                                                            referenceNode->getUnit().toString(),
                                                            errorHandler);

                    qWarning() << "The node port: "<< getName() << "(type: "
                       << TypeManaged::toString(getTypeManaged()) << ", unit: "
                       << getUnit().toString() << "), and the reference node: "
                       << referenceNode->getInstanceName() << "(type: "
                       << TypeManaged::toString(referenceNode->getTypeManaged()) << ", unit: "
                       << referenceNode->getUnit().toString() << "), are incompatible";
            return false;
        }
    }

    return true;
}

bool ANodePort::operator==(const ANodePort &nodePort) const
{
    return ((_name == nodePort._name) &&
            (_unit == nodePort._unit) &&
            (_typeManaged == nodePort._typeManaged));
}
