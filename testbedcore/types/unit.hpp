// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "testbedcore/types/unittype.hpp"


/** @brief This class represents the unit of a number */
class Unit : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit Unit(QObject *parent = nullptr);

        /** @brief Class constructor
            @param type The unit type to parse
                        If the type is not known, it will be kept raw
            @param powerOfTen The power of ten which represents the SI prefix of the unit,
                              for instance: milli
            @param parent The class parent */
        explicit Unit(const QString &type, qint8 powerOfTen, QObject *parent = nullptr);

        /** @brief Class constructor
            @param type The unit type
            @param powerOfTen The power of ten which represents the SI prefix of the unit,
                              for instance: milli
            @param parent The class parent */
        explicit Unit(UnitType::Enum type, qint8 powerOfTen, QObject *parent = nullptr);

        /** @brief Copy constructor */
        explicit Unit(const Unit &unit);

    public:
        /** @brief Set the power of ten */
        void setPowerOfTen(qint8 powerOfTen) { _powerOfTen = powerOfTen; }

        /** @brief Get the power of ten */
        qint8 getPowerOfTen() const { return _powerOfTen; }

        /** @brief Set the unit type
            @param type The unit type to parse
                        If the type is not known, it will be kept raw*/
        void setUnitType(const QString &unitType);

        /** @brief Set the unit type */
        void setUnitType(UnitType::Enum unitType) { _unitType = unitType; }

        /** @brief Get the unit type */
        UnitType::Enum getUnitType() const { return _unitType; }

        /** @brief Returns a string representation of the unit */
        QString toString() const;

        /** @brief Set unit to this instance
            @param otherUnit The other unit to copy
            @return The unit updated */
        Unit &operator=(const Unit &otherUnit);

        /** @brief Test if this unit and the other one are eguals
            @param otherUnit The other unit to test this unit with
            @return True if the units are equals */
        bool operator==(const Unit &otherUnit) const;

        /** @brief Test if this unit and the other one are not eguals
            @param otherUnit The other unit to test this unit with
            @return True if the units are not equals */
        bool operator!=(const Unit &otherUnit) const;

    private:
        /** @brief Get the SI prefix of the unit
            @return The SI prefix */
        QString getSiPrefix() const Q_DECL_CONST_FUNCTION;

    private:
        UnitType::Enum _unitType{UnitType::None};
        qint8 _powerOfTen{0};
        QString _specificSymbolType{""};
};

/** @brief Return a qHash for Unit
    @param number The class to get the qHash from
    @param seed The seed to add in the qHash
    @return The hash created */
inline uint qHash(const Unit &unit, uint seed)
{
    return qHash(unit.toString(), seed) ^ qHash(unit.getPowerOfTen(), seed);
}
