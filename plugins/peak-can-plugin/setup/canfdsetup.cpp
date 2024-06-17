// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canfdsetup.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "qtpeakcanlib/src/models/candevicefdconfigdetails.hpp"


CanFdSetup::CanFdSetup(QObject *parent)
    : ACanSetup{staticMetaObject.className(), true, parent}
{
}

CanFdSetup::~CanFdSetup()
{
}

bool CanFdSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACanSetup::initParamsFormat(paramsFormat));

    RETURN_IF_FALSE(addCommonParamsFormat(paramsFormat, true));

    JsonObject fClockInMHz;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(FClockInMHzParamName,
                                                                 FClockInMHzParamDesc,
                                                                 0,
                                                                 0,
                                                                 fClockInMHz));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Hertz, 6),
                                                             fClockInMHz));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(fClockInMHz,
                                                                   Number::minUint16(),
                                                                   Number::maxUint16()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, fClockInMHz));

    paramsFormat.append(fClockInMHz);

    JsonObject nomBrp;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(NomBrpParamName,
                                                                 NomBrpParamDesc,
                                                                 0,
                                                                 0,
                                                                 nomBrp));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(nomBrp,
                                                                   Number::minUint16(),
                                                                   Number::maxUint16()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, nomBrp));

    paramsFormat.append(nomBrp);

    JsonObject nomTseg1;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(NomTseg1ParamName,
                                                                 NomTseg1ParamName,
                                                                 0,
                                                                 0,
                                                                 nomTseg1));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(nomTseg1,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, nomTseg1));

    paramsFormat.append(nomTseg1);

    JsonObject nomTseg2;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(NomTseg2ParamName,
                                                                 NomTseg2ParamName,
                                                                 0,
                                                                 0,
                                                                 nomTseg2));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(nomTseg2,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, nomTseg2));

    paramsFormat.append(nomTseg2);

    JsonObject nomSjw;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(NomSjwParamName,
                                                                 NomSjwParamDesc,
                                                                 0,
                                                                 0,
                                                                 nomSjw));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(nomSjw,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, nomSjw));

    paramsFormat.append(nomSjw);

    JsonObject dataBrp;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(DataBrpParamName,
                                                                 DataBrpParamDesc,
                                                                 0,
                                                                 0,
                                                                 dataBrp));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(dataBrp,
                                                                   Number::minUint16(),
                                                                   Number::maxUint16()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, dataBrp));

    paramsFormat.append(dataBrp);

    JsonObject dataTseg1;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(DataTseg1ParamName,
                                                                 DataTseg1ParamDesc,
                                                                 0,
                                                                 0,
                                                                 dataTseg1));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(dataTseg1,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, dataTseg1));

    paramsFormat.append(dataTseg1);

    JsonObject dataTseg2;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(DataTseg2ParamName,
                                                                 DataTseg2ParamDesc,
                                                                 0,
                                                                 0,
                                                                 dataTseg2));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(dataTseg2,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, dataTseg2));

    paramsFormat.append(dataTseg2);

    JsonObject dataSjw;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(DataSjwParamName,
                                                                 DataSjwParamDesc,
                                                                 0,
                                                                 0,
                                                                 dataSjw));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(dataSjw,
                                                                   Number::minUint8(),
                                                                   Number::maxUint8()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, dataSjw));

    paramsFormat.append(dataSjw);

    return true;
}

bool CanFdSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(ACanSetup::validateAndSetParameters(parameters, errors));

    CanDeviceFdConfigDetails& configDetails = accessDeviceConfig().accessFdDetails();

    QString error;

    Number fClockInMHz;
    if(!BankJsonParser::getNumberValue(parameters,
                                       FClockInMHzParamName,
                                       fClockInMHz,
                                        &error))
    {
        fillErrorArray(FClockInMHzParamName, error, errors);
        return false;
    }

    configDetails.setFClockInMHz(fClockInMHz.toUint16());

    Number nomBrp;
    if(!BankJsonParser::getNumberValue(parameters,
                                       NomBrpParamName,
                                       nomBrp,
                                       &error))
    {
        fillErrorArray(NomBrpParamName, error, errors);
        return false;
    }

    configDetails.setNomBrp(nomBrp.toUint16());

    Number nomTseg1;
    if(!BankJsonParser::getNumberValue(parameters,
                                       NomTseg1ParamName,
                                       nomTseg1,
                                       &error))
    {
        fillErrorArray(NomTseg1ParamName, error, errors);
        return false;
    }

    configDetails.setNomTseg1(nomTseg1.toUint8());

    Number nomTseg2;
    if(!BankJsonParser::getNumberValue(parameters,
                                       NomTseg2ParamName,
                                       nomTseg2,
                                       &error))
    {
        fillErrorArray(NomTseg2ParamName, error, errors);
        return false;
    }

    configDetails.setNomTseg2(nomTseg2.toUint8());

    Number nomSjw;
    if(!BankJsonParser::getNumberValue(parameters,
                                       NomSjwParamName,
                                       nomSjw,
                                       &error))
    {
        fillErrorArray(NomSjwParamName, error, errors);
        return false;
    }

    configDetails.setNomSjw(nomSjw.toUint8());

    Number dataBrp;
    if(!BankJsonParser::getNumberValue(parameters,
                                       DataBrpParamName,
                                       dataBrp,
                                       &error))
    {
        fillErrorArray(DataBrpParamName, error, errors);
        return false;
    }

    configDetails.setDataBrp(dataBrp.toUint16());

    Number dataTseg1;
    if(!BankJsonParser::getNumberValue(parameters,
                                       DataTseg1ParamName,
                                       dataTseg1,
                                       &error))
    {
        fillErrorArray(DataTseg1ParamName, error, errors);
        return false;
    }

    configDetails.setDataTseg1(dataTseg1.toUint8());

    Number dataTseg2;
    if(!BankJsonParser::getNumberValue(parameters,
                                       DataTseg2ParamName,
                                       dataTseg2,
                                       &error))
    {
        fillErrorArray(DataTseg2ParamName, error, errors);
        return false;
    }

    configDetails.setDataTseg2(dataTseg2.toUint8());

    Number dataSjw;
    if(!BankJsonParser::getNumberValue(parameters,
                                       DataSjwParamName,
                                       dataSjw,
                                       &error))
    {
        fillErrorArray(DataSjwParamName, error, errors);
        return false;
    }

    configDetails.setDataSjw(dataSjw.toUint8());

    return true;
}
