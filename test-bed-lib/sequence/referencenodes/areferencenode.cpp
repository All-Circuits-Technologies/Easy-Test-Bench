// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "areferencenode.hpp"

#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


AReferenceNode::AReferenceNode(NodeType::Enum nodeType,
                               const QSharedPointer<NodesSequence> &sequenceParent,
                               QObject *parent) :
    ANode(nodeType, sequenceParent, parent)
{
}

AReferenceNode::~AReferenceNode()
{
    forgetAllAboutMe();
}

void AReferenceNode::setTypeManaged(TypeManaged::Enum typeManaged)
{
    if(typeManaged == _typeManaged)
    {
        // Nothing to do
        return;
    }

    _typeManaged = typeManaged;

    if(_value.isValid())
    {
        // Nothing to do
        return;
    }

    if(_typeManaged == TypeManaged::BooleanList)
    {
         if(!CommonJsonParser::setValue(QVector<bool>(), _value))
         {
             qWarning() << "A problem occurred when tried to set a default BooleanList value for "
                           "the reference node: " << getInstanceName();
         }
    }
    else if(_typeManaged == TypeManaged::NumberList)
    {
        if(!CommonJsonParser::setValue(QVector<Number>(), _value))
        {
            qWarning() << "A problem occurred when tried to set a default NumberList value for "
                          "the reference node: " << getInstanceName();
        }
    }
    else if(_typeManaged == TypeManaged::StringList)
    {
        if(!CommonJsonParser::setValue(QVector<QString>(), _value))
        {
            qWarning() << "A problem occurred when tried to set a default QStringList value for "
                          "the reference node: " << getInstanceName();
        }
    }
}

void AReferenceNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ANode::forgetAllAboutIt(nodeToDetachWith);

    if(nodeToDetachWith.data() == this)
    {
        forgetAllAboutMe();
    }
}

void AReferenceNode::forgetAllAboutMe()
{
    accessInstanceParent().clear();
}

bool AReferenceNode::setValue(const QVariant &value)
{
    if(!CommonJsonParser::testIfCanBeConverted(_typeManaged, value))
    {
        qWarning() << "The value given: " << value << " for the reference node: "
                   << getInstanceName() << ", can't be converted to: "
                   << TypeManaged::toString(_typeManaged);
        return false;
    }

    _value = value;

    return true;
}
