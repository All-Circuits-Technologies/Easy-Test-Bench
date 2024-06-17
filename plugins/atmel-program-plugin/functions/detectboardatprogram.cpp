// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "detectboardatprogram.hpp"

#include <QCoreApplication>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "hmi-helper-lib/hmidisplaysimpletestview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "types/atprogramcmds.hpp"


DetectBoardAtProgram::DetectBoardAtProgram(QObject *parent) :
    AAtmelProgramItf(staticMetaObject.className(), parent),
    MixinBoardDetection<DetectAtmelSharedData>{}
{
}

DetectBoardAtProgram::~DetectBoardAtProgram()
{
}

void DetectBoardAtProgram::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool success = detectBoardProcess(jsonObjectInputValues);

    emit moduleFinished(success, {});
}

bool DetectBoardAtProgram::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AAtmelProgramItf::initParamsFormat(paramsFormat));
    return initDetectParamsFormat(paramsFormat);
}

bool DetectBoardAtProgram::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AAtmelProgramItf::validateAndSetParameters(parameters, errors));
    return validateAndSetDetectParameters(parameters, errors);
}

bool DetectBoardAtProgram::boardDetectionProcess(const DetectAtmelSharedData &sharedData,
                                                 bool &isDetected)
{
    return callProcess(sharedData.getAtPgmPath(),
                       {
                           AtProgramCmds::toString(AtProgramCmds::Info),
                           AtPgmVoltageArg
                       },
                       true,
                       DefaultTimeoutLoopInMs,
                       &isDetected);
}

bool DetectBoardAtProgram::detectBoardProcess(const JsonArray &jsonObjectInputValues)
{
    QString atPgmPath;
    RETURN_IF_FALSE(parseAtProgramPath(jsonObjectInputValues, atPgmPath));

    const DetectAtmelSharedData sharedData(atPgmPath);

    return detectBoard(sharedData, *this);
}
