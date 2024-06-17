// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "csvaddts.hpp"

#include <QDateTime>

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/types/qtdateformathelper.hpp"
#include "testbedcore/types/qttimespechelper.hpp"


CsvAddTs::CsvAddTs(QObject *parent) :
    ACsvAdd(staticMetaObject.className(), parent)
{
}

bool CsvAddTs::append(const JsonArray &jsonObjectInputValues, QVector<QString> &csvLine)
{
    Q_UNUSED(jsonObjectInputValues)

    QDateTime currentTs;

    switch (_timeSpec)
    {
        case Qt::LocalTime:
        case Qt::TimeZone:
            currentTs = QDateTime::currentDateTime();
            break;
        case Qt::UTC:
        case Qt::OffsetFromUTC:
            currentTs = QDateTime::currentDateTimeUtc();
            break;
    }

    QString tsToLog;

    if(!_datetimeFormatPattern.isEmpty())
    {
        tsToLog = currentTs.toString(_datetimeFormatPattern);
    }
    else
    {
        tsToLog = currentTs.toString(_datetimeFormat);
    }

    csvLine.append(tsToLog);

    return true;
}

bool CsvAddTs::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(ACsvAdd::validateAndSetParameters(parameters, errors));

    QVector<QString> timeSpecStr;
    QString error;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                             TimeSpecParamName,
                                            timeSpecStr,
                                            &error,
                                            true))
    {
        fillErrorArray(TimeSpecParamName, error, errors);
        return false;
    }

    bool ok = false;

    if(!timeSpecStr.isEmpty())
    {
        _timeSpec = QtTimeSpecHelper::parseFromString(timeSpecStr.first(), &ok);

        if(!ok)
        {
            error = "A wrong timespec has been given " + timeSpecStr.first();
            fillErrorArray(TimeSpecParamName, error, errors);
            return false;
        }
    }

    QVector<QString> datetimeFormatList;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                             DatetimeFormatEnumParamName,
                                            datetimeFormatList,
                                            &error,
                                            true))
    {
        fillErrorArray(DatetimeFormatEnumParamName, error, errors);
        return false;
    }

    if(!datetimeFormatList.isEmpty())
    {
        _datetimeFormat = QtDateFormatHelper::parseFromString(datetimeFormatList.first(), &ok);

        if(!ok)
        {
            error = "A wrong datetime format has been given " + datetimeFormatList.first();
            fillErrorArray(DatetimeFormatEnumParamName, error, errors);
            return false;
        }
    }

    if(!BankJsonParser::getStringValue(parameters,
                                        DatetimeFormatStrParamName,
                                       _datetimeFormatPattern,
                                       &error,
                                       true))
    {
        fillErrorArray(DatetimeFormatStrParamName, error, errors);
        return false;
    }

    return true;
}

bool CsvAddTs::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject timeSpecObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(TimeSpecParamName,
                                                                 TimeSpecParamDesc,
                                                                 timeSpecObj));

    for(Qt::TimeSpec timeSpec : QtTimeSpecHelper::getAllEnums())
    {
        QString formatTypestr = QtTimeSpecHelper::toString(timeSpec);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(formatTypestr,
                                                                      formatTypestr,
                                                                      timeSpecObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { QtTimeSpecHelper::toString(TimeSpecDefaultValue) },
                                    timeSpecObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, timeSpecObj));

    paramsFormat.append(timeSpecObj);

    JsonObject dateTimeFormatObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(DatetimeFormatEnumParamName,
                                                                 DatetimeFormatEnumParamDesc,
                                                                 dateTimeFormatObj));

    for(Qt::DateFormat datetimeFormat : QtDateFormatHelper::getAllEnums())
    {
        QString formatTypestr = QtDateFormatHelper::toString(datetimeFormat);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(formatTypestr,
                                                                      formatTypestr,
                                                                      dateTimeFormatObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { QtDateFormatHelper::toString(DatetimeFormatDefaultValue) },
                                    dateTimeFormatObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, dateTimeFormatObj));

    paramsFormat.append(dateTimeFormatObj);

    JsonObject datetimeFormatStrObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(DatetimeFormatStrParamName,
                                                                 DatetimeFormatStrParamDesc,
                                                                 datetimeFormatStrObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, datetimeFormatStrObj));

    paramsFormat.append(datetimeFormatStrObj);

    return true;
}
