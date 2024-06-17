// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "csvaddstring.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


CsvAddString::CsvAddString(QObject *parent) :
    ACsvAdd(staticMetaObject.className(), parent)
{
}

CsvAddString::~CsvAddString()
{
}

bool CsvAddString::append(const JsonArray &jsonObjectInputValues, QVector<QString> &csvLine)
{
    QString inputString;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputStringToAddName,
                                                   inputString));

    csvLine.append(inputString);
    return true;
}

bool CsvAddString::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ACsvAdd::initInputsFormat(inputsFormat));

    JsonObject csvElem;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputStringToAddName,
                                                         csvElem,
                                                         InputStringToAddDesc));

    inputsFormat.append(csvElem);

    return true;
}
