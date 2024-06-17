// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "csvaddnumber.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


CsvAddNumber::CsvAddNumber(QObject *parent) :
    ACsvAdd(staticMetaObject.className(), parent)
{
}

CsvAddNumber::~CsvAddNumber()
{
}

bool CsvAddNumber::append(const JsonArray &jsonObjectInputValues, QVector<QString> &csvLine)
{
    Number inputNumber;
    RETURN_IF_FALSE(BankJsonParser::getNumberValue(jsonObjectInputValues,
                                                   InputNumberToAddName,
                                                   inputNumber));

    csvLine.append(inputNumber.toString());
    return true;
}

bool CsvAddNumber::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ACsvAdd::initInputsFormat(inputsFormat));

    JsonObject csvElem;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(InputNumberToAddName,
                                                         csvElem,
                                                         InputNumberToAddDesc));

    inputsFormat.append(csvElem);

    return true;
}
