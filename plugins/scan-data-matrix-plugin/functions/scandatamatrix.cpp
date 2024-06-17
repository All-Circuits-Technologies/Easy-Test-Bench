// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "scandatamatrix.hpp"

#include <QDir>
#include <QRegularExpression>
#include <QTimer>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplaystringinputview.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "waitutility/waithelper.hpp"


ScanDataMatrix::ScanDataMatrix(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

bool ScanDataMatrix::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject ouputStringScanned;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(OutputDataName,
                                                         ouputStringScanned,
                                                         OutputDataDesc));

    outputsFormat.append(ouputStringScanned);

    return true;
}

bool ScanDataMatrix::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject imagePathParam;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ImagePathParamName,
                                                                 ImagePathParamDesc,
                                                                 imagePathParam));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(DefaultImagePath,
                                                                     imagePathParam));

    paramsFormat.append(imagePathParam);

    JsonObject hmiDescParam;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(HmiDescriptionParamName,
                                                                 HmiDescriptionParamDesc,
                                                                 hmiDescParam));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(DefaultHmiDescription,
                                                                     hmiDescParam));

    paramsFormat.append(hmiDescParam);

    JsonObject regExpPatternParam;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ValidRegexPatternParamName,
                                                                 ValidRegexPatternParamDesc,
                                                                 regExpPatternParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamPatternRegex(ValidRegexPatternRegex,
                                                               regExpPatternParam));

    paramsFormat.append(regExpPatternParam);

    JsonObject clearFieldIfErrorParam;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(ClearFieldIfErrorParamName,
                                                               ClearFieldIfErrorParamDesc,
                                                               clearFieldIfErrorParam));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultClearFieldIfError,
                                                                   clearFieldIfErrorParam));

    paramsFormat.append(clearFieldIfErrorParam);

    return true;
}

bool ScanDataMatrix::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    QString imagePath;
    if(!BankJsonParser::getStringValue(parameters, ImagePathParamName, imagePath, &error, true))
    {
        fillErrorArray(ImagePathParamName, error, errors);
        return false;
    }

    if(imagePath.isEmpty())
    {
        _imagePath = DefaultImagePath;
    }
    else
    {
        QDir seqDir;

        if (!DefaultBankGlobalManager::Instance()->accessCommonInterface()
                                                          .getSequenceDirectory(getSequenceToken(),
                                                                                seqDir))
        {
            error = "Impossible to get the directory path of the sequence with sequennce token";
            fillErrorArray(ImagePathParamName, error, errors);
            qWarning() << error << ": " << getSequenceToken();
            return false;
        }

        QFileInfo imageInfo(seqDir, imagePath);

        if(!imageInfo.exists() || !imageInfo.isFile())
        {
            qWarning() << "Image not found: " << imagePath << ", use default image";
            _imagePath = DefaultImagePath;
        }
        else
        {
            _imagePath = imageInfo.absoluteFilePath();
        }
    }

    _hmiDescription = DefaultHmiDescription;
    if(!BankJsonParser::getStringValue(parameters,
                                       HmiDescriptionParamName,
                                       _hmiDescription,
                                       &error,
                                       true))
    {
        fillErrorArray(HmiDescriptionParamName, error, errors);
        return false;
    }

    QString regExpPattern;
    if(!BankJsonParser::getStringValue(parameters,
                                       ValidRegexPatternParamName,
                                       regExpPattern,
                                       &error,
                                       true))
    {
        fillErrorArray(ValidRegexPatternParamName, error, errors);
        return false;
    }

    QRegularExpression regExp(regExpPattern);

    if(!regExp.isValid())
    {
        fillErrorArray(ValidRegexPatternParamName,
                       QString(InvalidPatternRegExpError).arg(regExpPattern),
                       errors);
        return false;
    }

    _regExpPattern = regExpPattern;

    _clearFieldIfError = DefaultClearFieldIfError;
    if(!BankJsonParser::getBoolValue(parameters,
                                     ClearFieldIfErrorParamName,
                                     _clearFieldIfError,
                                     &error,
                                     true))
    {
        fillErrorArray(ClearFieldIfErrorParamName, error, errors);
        return false;
    }

    return true;
}

void ScanDataMatrix::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    JsonArray outputs;
    if(!operate(outputs))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying scan data matrix")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, outputs);
}

bool ScanDataMatrix::operate(JsonArray &output)
{
    QString userInput;
    RETURN_IF_FALSE(askForInput(userInput));

    return BankJsonParser::setStringValue(OutputDataName, userInput, output);
}

bool ScanDataMatrix::askForInput(QString &userInput)
{
    HmiDisplayStringInputView hmiDisplay = HmiDisplayStringInputView(accessCommIntf());

    bool cancelledOrErrorOccurred = false;
    auto waitTestFunc = [this, &hmiDisplay, &cancelledOrErrorOccurred]() {
        HmiDisplayStatus::Enum displayStatus = hmiDisplay.getDisplayStatus();

        if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()) ||
                displayStatus == HmiDisplayStatus::Error)
        {
            cancelledOrErrorOccurred = true;
            hmiDisplay.cancelDisplaying();
            return true;
        }

        return (hmiDisplay.getDisplayStatus() == HmiDisplayStatus::Success);
    };

    hmiDisplay.setClearFieldIfError(_clearFieldIfError);
    RETURN_IF_FALSE(hmiDisplay.displayHmi(OutputDataName,
                                          tr(_hmiDescription.toLatin1()),
                                          _imagePath,
                                          1,
                                          HmiDisplayStringInputView::DefaultMaxLength,
                                          _regExpPattern));


    WaitHelper::pseudoWait(waitTestFunc);

    if(cancelledOrErrorOccurred)
    {
        // An error occurred
        return false;
    }

    return parseInput(hmiDisplay.getResultReceived(), userInput);
}

bool ScanDataMatrix::parseInput(const JsonArray &hmiResultReceived, QString &parsedInput)
{
    QString rawInput;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(hmiResultReceived, OutputDataName, rawInput));

    if(_regExpPattern.isEmpty())
    {
        // There is nothing to get
        parsedInput = rawInput;
        return true;
    }

    QRegularExpression captureRegexp(_regExpPattern);

    QRegularExpressionMatch match = captureRegexp.match(rawInput);

    int lastCapturedIdx = match.lastCapturedIndex();

    if(lastCapturedIdx == 0)
    {
        // That means, no capture group was expected; therefore we return all
        parsedInput = rawInput;
        return true;
    }

    parsedInput = "";
    for(int idx = 1; idx <= lastCapturedIdx; ++idx)
    {
        parsedInput += match.captured(idx);
    }

    return true;
}
