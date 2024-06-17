// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "unit.hpp"


Unit::Unit(QObject *parent) :
    QObject(parent)
{
}

Unit::Unit(const QString &type, qint8 powerOfTen, QObject *parent) :
    QObject(parent),
    _powerOfTen(powerOfTen)
{
    _unitType = UnitType::parseFromString(type);

    if(_unitType == UnitType::Other)
    {
        _specificSymbolType = type;
    }
}

Unit::Unit(UnitType::Enum type, qint8 powerOfTen, QObject *parent) :
    QObject(parent),
    _unitType(type),
    _powerOfTen(powerOfTen)
{
}

Unit::Unit(const Unit &unit) :
    QObject(),
    _unitType(unit._unitType),
    _powerOfTen(unit._powerOfTen),
    _specificSymbolType(unit._specificSymbolType)
{
}

void Unit::setUnitType(const QString &unitType)
{
    _unitType = UnitType::parseFromString(unitType);

    if(_unitType == UnitType::Other)
    {
        _specificSymbolType = unitType;
    }
}

QString Unit::toString() const
{
    if(_unitType == UnitType::Other)
    {
        return _specificSymbolType;
    }

    return UnitType::getSymbol(_unitType);
}

Unit &Unit::operator=(const Unit &otherUnit)
{
    _unitType           = otherUnit._unitType;
    _powerOfTen         = otherUnit._powerOfTen;
    _specificSymbolType = otherUnit._specificSymbolType;

    return *this;
}

bool Unit::operator==(const Unit &otherUnit) const
{
    return (_unitType == otherUnit._unitType                         &&
            (otherUnit._unitType != UnitType::Other                  ||
             otherUnit._specificSymbolType == _specificSymbolType));
}

bool Unit::operator!=(const Unit &otherUnit) const
{
    return (_unitType != otherUnit._unitType                         ||
            (otherUnit._unitType == UnitType::Other                  &&
             otherUnit._specificSymbolType != _specificSymbolType));
}

QString Unit::getSiPrefix() const
{
    switch(_powerOfTen)
    {
        case -24:       // yocto
            return "y";
        case -21:       // zepto
            return "z";
        case -18:       //atto
            return "a";
        case -15:       // femto
            return "f";
        case -12:       // pico
            return "p";
        case -9:        // nano
            return "n";
        case -6:        // micro
            return "µ";
        case -3:        // milli
            return "m";
        case -2:        // centi
            return "c";
        case -1:        // deci
            return "d";
        case 0:         // unit
            return "";
        case 1:         // deca
            return "da";
        case 2:         // hecto
            return "h";
        case 3:         // kilo
            return "k";
        case 6:         // mega
            return "M";
        case 9:         // giga
            return "G";
        case 12:        // tera
            return "T";
        case 15:        // peta
            return "P";
        case 18:        // exa
            return "E";
        case 21:        // zetta
            return "Z";
        case 24:        // yotta
            return "Y";
    }

    return "";
}
