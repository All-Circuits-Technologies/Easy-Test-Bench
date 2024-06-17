// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "typemanaged.hpp"

#include <QMetaEnum>


TypeManaged::Enum TypeManaged::parseFromString(const QString &strValue)
{
    Enum value = Unknown;
    QString lowerStr = strValue.toLower();

    if(lowerStr == "boolean" || lowerStr == "bool")
    {
        value = Boolean;
    }
    else if(lowerStr == "string")
    {
        value = String;
    }
    else if(lowerStr == "number")
    {
        value = Number;
    }
    else if(lowerStr == "list<boolean>" || lowerStr == "list<bool>" ||
            lowerStr == "boollist"      || lowerStr == "booleanlist")
    {
        value = BooleanList;
    }
    else if(lowerStr == "list<string>" || lowerStr == "stringlist")
    {
        value = StringList;
    }
    else if(lowerStr == "list<number>" || lowerStr == "numberlist")
    {
        value = NumberList;
    }

    return value;
}

QString TypeManaged::toString(TypeManaged::Enum typeManaged)
{
    switch(typeManaged)
    {
        case Boolean:
            return "bool";
        case String:
            return "string";
        case Number:
            return "number";
        case BooleanList:
            return "list<bool>";
        case StringList:
            return "list<string>";
        case NumberList:
            return "list<number>";
        case Unknown:
            return "unknown";
    }

    return "unknown";
}

bool TypeManaged::isList(TypeManaged::Enum typeManaged)
{
    switch(typeManaged)
    {
        case Boolean:
        case String:
        case Number:
            return false;

        case BooleanList:
        case StringList:
        case NumberList:
            return true;

        case Unknown:
            return false;
    }

    return false;
}

QVector<TypeManaged::Enum> TypeManaged::getAvailableElements()
{
    QVector<Enum> enumList;
    const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        enumList.append(static_cast<Enum>(metaEnum.value(idx)));
    }

    return enumList;
}
