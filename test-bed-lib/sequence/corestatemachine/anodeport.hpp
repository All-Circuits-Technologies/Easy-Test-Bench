// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "errors/sequenceloadingerrorhandler.hpp"
#include "testbedcore/types/typemanaged.hpp"
#include "testbedcore/types/unit.hpp"

class AReferenceNode;


/** @brief Abstract port which can be an input or an output and points to a reference node */
class ANodePort : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class parent
            @param parent The parent object*/
        explicit ANodePort(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param other The object to copy */
        ANodePort(const ANodePort &other);

        /** @brief Class destructor */
        virtual ~ANodePort() override = default;

    public:
        /** @brief Get the node port name */
        const QString &getName() const { return _name; }

        /** @brief Set the node port name */
        void setName(const QString &name) { _name = name; }

        /** @brief Get type managed by the node port */
        TypeManaged::Enum getTypeManaged() const { return _typeManaged; }

        /** @brief Set the type managed by the node port */
        void setTypeManaged(TypeManaged::Enum typeManaged) { _typeManaged = typeManaged; }

        /** @brief Get the unit of the node port */
        const Unit &getUnit() const { return _unit; }

        /** @brief Set the unit of the node port
            @param unit The unit to set */
        void setUnit(const Unit &unit) { _unit = unit; }

        /** @brief Validate if the reference node given can be linked with this node port
            @param referenceNode The reference node to test with
            @param errorHandler An optional sequence loading error handler pointer
            @return True If the reference node and node port are compatible */
        bool validateReference(const QSharedPointer<AReferenceNode> &referenceNode,
                               SequenceLoadingErrorHandler *errorHandler = nullptr) const;

        /** @brief Equality operator
            @param nodePort The other ANodePort to test with
            @return True if the other nodes attributes are identical of this one */
        bool operator==(const ANodePort &nodePort) const;

    private:
        QString _name{""};
        Unit _unit;
        TypeManaged::Enum _typeManaged{TypeManaged::Unknown};
};
