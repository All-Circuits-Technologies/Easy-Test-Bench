// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "refandparamstringcomparator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


RefAndParamStringComparator::RefAndParamStringComparator(QObject *parent) :
    ARefAndParamComparator<QString>(staticMetaObject.className(),
                                    parent)
{
}

bool RefAndParamStringComparator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ARefAndParamComparator<QString>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(valueParamName,
                                                                 valueParamDesc,
                                                                 valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    return true;
}
