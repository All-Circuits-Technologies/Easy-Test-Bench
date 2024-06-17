// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "measureparamsboundstest.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


MeasureParamsBoundsTest::MeasureParamsBoundsTest(QObject *parent) :
    AMeasureBoundsTest(staticMetaObject.className(), parent)
{
}

MeasureParamsBoundsTest::~MeasureParamsBoundsTest()
{
}

bool MeasureParamsBoundsTest::validateAndSetParameters(const JsonArray &parameters,
                                                       JsonArray &errors)
{
    RETURN_IF_FALSE(AMeasureBoundsTest::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getNumberValue(parameters, InfBoundParamName, _infBound, &error))
    {
        fillErrorArray(InfBoundParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getNumberValue(parameters, SupBoundParamName, _supBound, &error))
    {
        fillErrorArray(SupBoundParamName, error, errors);
        return false;
    }

    if(_infBound > _supBound)
    {
        error = "The inferior bound: " + _infBound.toString().toLatin1() + " can't be higher " +
                "than the superior bound: " + _supBound.toString().toLatin1();
        fillErrorArray(SupBoundParamName, error, errors);
        qWarning() << error;
        return false;
    }

    return true;
}

bool MeasureParamsBoundsTest::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AMeasureBoundsTest::initParamsFormat(paramsFormat));

    JsonObject infBound;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(InfBoundParamName,
                                                                 InfBoundParamDesc,
                                                                 0,
                                                                 0,
                                                                 infBound));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(infBound,
                                                                   Number(),
                                                                   Number(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, infBound));

    paramsFormat.append(infBound);

    JsonObject supBound;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(SupBoundParamName,
                                                                 SupBoundParamDesc,
                                                                 0,
                                                                 0,
                                                                 supBound));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(supBound,
                                                                   Number(),
                                                                   Number(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, supBound));

    paramsFormat.append(supBound);

    return true;
}

bool MeasureParamsBoundsTest::processTest(const JsonArray &jsonObjectInputValues, bool &testSuccess)
{
    return compare(jsonObjectInputValues, _infBound, _supBound, testSuccess);
}
