// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/anode.hpp"

#include <QVariant>

#include "testbedcore/types/typemanaged.hpp"
#include "testbedcore/types/unit.hpp"

class Unit;


/** @brief Represents an abstract reference node */
class AReferenceNode : public ANode
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param nodeType The type of node
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit AReferenceNode(NodeType::Enum nodeType,
                                const QSharedPointer<NodesSequence> &sequenceParent,
                                QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AReferenceNode() override;

    public:
        /** @brief Set the type managed by this reference
            @note If the type managed is an array and the value is invalid, this will set a default
                  empty array to value */
        void setTypeManaged(TypeManaged::Enum typeManaged);

        /** @brief Get the type managed by this reference */
        TypeManaged::Enum getTypeManaged() const { return _typeManaged; }

        /** @brief Get the reference node value */
        const QVariant &getValue() const { return _value; }

        /** @brief Get the unit linked to the reference node */
        const Unit &getUnit() const { return _unit; }

        /** @brief Set the unit linked to the reference node */
        void setUnit(const Unit &unit) { _unit = unit; }

    protected slots:
        /** @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

    private:
        /** @brief Same as forgetAllAboutIt but manages the case where the node to detach is this
                   instance
            @see ANode::forgetAllAboutIt */
        void forgetAllAboutMe();

    protected:
        /** @brief Set the reference node value
            @param value The value to set in node
            @return True if no problem occurs */
        bool setValue(const QVariant &value);

    private:
        QVariant _value{};
        TypeManaged::Enum _typeManaged{TypeManaged::Unknown};
        Unit _unit;
};

/** @brief Return a qHash for AReferenceNode
    @param aReferenceNode The class to get the qHash from
    @param seed The seed to add in the qHash
    @return The hash created */
inline uint qHash(const AReferenceNode &aReferenceNode, uint seed)
{
    return qHash(aReferenceNode.getTypeManaged(), seed) ^
           qHash(aReferenceNode.getUnit(), seed) ^
           qHash(aReferenceNode.getValue().toString(), seed);
}
