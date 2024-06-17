// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "measureinputsboundstest.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


MeasureInputsBoundsTest::MeasureInputsBoundsTest(QObject *parent) :
    AMeasureBoundsTest(staticMetaObject.className(), parent)
{
}

MeasureInputsBoundsTest::~MeasureInputsBoundsTest()
{
}

bool MeasureInputsBoundsTest::processTest(const JsonArray &jsonObjectInputValues, bool &testSuccess)
{
    Number infBound;
    RETURN_IF_FALSE(BankJsonParser::getNumberValue(jsonObjectInputValues,
                                                   InputInfBoundName,
                                                   infBound));

    Number supBound;
    RETURN_IF_FALSE(BankJsonParser::getNumberValue(jsonObjectInputValues,
                                                   InputSupBoundName,
                                                   supBound));

    return compare(jsonObjectInputValues, infBound, supBound, testSuccess);
}

bool MeasureInputsBoundsTest::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AMeasureBoundsTest::initInputsFormat(inputsFormat));

    JsonObject infBound;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(InputInfBoundName,
                                                         infBound,
                                                         InputInfBoundDesc));

    inputsFormat.append(infBound);

    JsonObject supBound;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(InputSupBoundName,
                                                         supBound,
                                                         InputSupBoundDesc));

    inputsFormat.append(supBound);

    return true;
}
