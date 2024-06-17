// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "detectboard.hpp"

#include "testbedcore/testbedglobal.hpp"


DetectBoard::DetectBoard(QObject *parent) :
    AJLinkIntfFromParams(staticMetaObject.className(), parent),
    MixinBoardDetection<DetectJLinkSharedData>{}
{
}

DetectBoard::~DetectBoard()
{
}

bool DetectBoard::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AJLinkIntfFromParams::initParamsFormat(paramsFormat));
    return initDetectParamsFormat(paramsFormat);
}

bool DetectBoard::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AJLinkIntfFromParams::validateAndSetParameters(parameters, errors));
    return validateAndSetDetectParameters(parameters, errors);
}

bool DetectBoard::mainJLinkProcess(const JsonArray &/*jsonObjectInputValues*/,
                                   const QString &jLinkPgmPath,
                                   JsonArray &/*output*/)
{
    const DetectJLinkSharedData sharedData(jLinkPgmPath);

    return detectBoard(sharedData, *this);
}

bool DetectBoard::boardDetectionProcess(const DetectJLinkSharedData &sharedData, bool &isDetected)
{
    return callCmdsProcess({ JLinkCmdEoeEnable,
                            JLinkCmdConnect,
                            QString(JLinkCmdMr).arg(1),
                            JLinkCmdCloseAndQuit },
                           sharedData.getJLinkPgmPath(),
                           DefaultTimeoutLoopInMs,
                           false,
                           &isDetected);
}
