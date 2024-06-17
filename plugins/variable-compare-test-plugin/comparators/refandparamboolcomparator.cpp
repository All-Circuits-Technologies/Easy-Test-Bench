// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "refandparamboolcomparator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


RefAndParamBoolComparator::RefAndParamBoolComparator(QObject *parent) :
    ARefAndParamComparator<bool>(staticMetaObject.className(),
                                 parent)
{
}

bool RefAndParamBoolComparator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ARefAndParamComparator<bool>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(valueParamName,
                                                               valueParamDesc,
                                                               valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(false, valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    return true;
}
