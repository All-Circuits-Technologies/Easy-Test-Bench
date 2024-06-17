// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acansetup.hpp"

#include <QDebug>

#include "qtpeakcanlib/src/models/candeviceconfig.hpp"
#include "qtpeakcanlib/src/candevice/candeviceintf.hpp"
#include "qtpeakcanlib/src/canmanager.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"

#include "peak-can-lib/canmodulesutility.hpp"
#include "peak-can-lib/peakcanlibconstants.hpp"

#include "peakcanbankglobalmanager.hpp"
#include "peakcanconstants.hpp"


ACanSetup::ACanSetup(const QString &moduleClassName,
                     bool hasFlexibleDataRateDefaultValue,
                     QObject *parent)
    : DefaultSequenceInstanceModule(moduleClassName,
                                    PeakCanBankGlobalManager::Instance(),
                                    parent),
    _deviceConfig(hasFlexibleDataRateDefaultValue)
{
}

ACanSetup::~ACanSetup()
{
}

bool ACanSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject busOffAutoReset;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(BusOffAutoResetParamName,
                                                               BusOffAutoResetParamDesc,
                                                               busOffAutoReset));

    paramsFormat.append(busOffAutoReset);

    return true;
}

bool ACanSetup::addCommonParamsFormat(JsonArray &paramsFormat,
                                      bool addFlexibleDataRateParam) const
{
    if(addFlexibleDataRateParam)
    {
        JsonObject flexibleDataRateObj;

        RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(FlexibleDataRateParamName,
                                                                   FlexibleDataRateParamDesc,
                                                                   flexibleDataRateObj));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
            _deviceConfig.isCanFd(),
            flexibleDataRateObj));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, flexibleDataRateObj));

        paramsFormat.append(flexibleDataRateObj);
    }

    return true;
}

bool ACanSetup::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputPeakCanIntf;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
                                PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
                                inputPeakCanIntf,
                                PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfDesc));

    inputsFormat.append(inputPeakCanIntf);
    return true;
}

bool ACanSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    bool hasFlexibleDataRate = _deviceConfig.isCanFd();
    if(!BankJsonParser::getBoolValue(parameters,
                                     FlexibleDataRateParamName,
                                     hasFlexibleDataRate,
                                     &error,
                                     true))
    {
        fillErrorArray(FlexibleDataRateParamName, error, errors);
        return false;
    }

    _deviceConfig.setCanFd(hasFlexibleDataRate);

    bool busOffAutoResetParam = DefaultBusOffAutoResetParam;
    if(!BankJsonParser::getBoolValue(parameters,
                                     BusOffAutoResetParamName,
                                     busOffAutoResetParam,
                                     &error,
                                     true))
    {
        fillErrorArray(BusOffAutoResetParamName, error, errors);
        return false;
    }

    _busOffAutoResetParam = busOffAutoResetParam;

    return true;
}

void ACanSetup::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!setupCan(jsonObjectInputValues))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to setup a can "
                                                                    "interface for "
                                                                    "communication")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, {});
}

bool ACanSetup::setupCan(const JsonArray &jsonObjectInputValues)
{
    QString strCanBusIntf;
    PCanBusItf::Enum pCanBusIntf;
    RETURN_IF_FALSE(CanModulesUtility::extractAndParseCanBusIntf(jsonObjectInputValues,
                                                                 strCanBusIntf,
                                                                 pCanBusIntf));

    _deviceConfig.setCanBusItf(pCanBusIntf);

    _canIntf = CanManager::getInstance().createOrGetCanIntf(_deviceConfig);

    if(_canIntf.isNull())
    {
        qWarning() << "A problem occurred when tried to create and get the can Interface";
        return false;
    }

    bool autoReset = true;
    if(!_canIntf->getParamBusOffAutoReset(autoReset))
    {
        qWarning() << "A problem occurred when tried to get the current BUS off auto reset param "
                   << "value";
        return false;
    }

    if(autoReset != _busOffAutoResetParam &&
        !_canIntf->setParamBusOffAutoReset(_busOffAutoResetParam))
    {
        qWarning() << "A problem occurred when tried to set the current BUS off auto reset param "
                   << "value: " << _busOffAutoResetParam;
        return false;
    }

    return true;
}
