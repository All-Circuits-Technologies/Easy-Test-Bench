// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "displayerrorresults.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "hmi-helper-lib/hmidisplaysimpletestview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "waitutility/waithelper.hpp"


DisplayErrorResults::DisplayErrorResults(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

DisplayErrorResults::~DisplayErrorResults()
{
}

void DisplayErrorResults::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!displayErrorResults(jsonObjectInputValues))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred when "
                                                                    "displaying error results.")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, {});
}

bool DisplayErrorResults::displayErrorResults(const JsonArray &jsonObjectInputValues)
{
    QVector<QString> errorsToDisplay;
    RETURN_IF_FALSE(BankJsonParser::getArrayStringValue(jsonObjectInputValues,
                                                        InputErrorsToDisplayName,
                                                        errorsToDisplay));

    HmiDisplaySimpleTestView hmiDisplay{accessCommIntf()};

    QString errorsStr;

    QVector<QString>::const_iterator citer = errorsToDisplay.cend();
    int counter = 0;

    while((citer != errorsToDisplay.cbegin()) && (counter < _errorsNbToDisplay))
    {
        --citer;
        ++counter;

        errorsStr.prepend(QString("  * %1 \n").arg(*citer));
    }

    QString hmiTextDisplayed = tr(HmiDescription);

    if(errorsStr.isEmpty())
    {
        hmiTextDisplayed = hmiTextDisplayed.arg(HmiDescNoError);
    }
    else
    {
        hmiTextDisplayed = hmiTextDisplayed.arg(tr(HmiDescWithErrors, "", _errorsNbToDisplay)
                                                    .arg(errorsStr));
    }

    hmiDisplay.setButtonName(HmiDisplaySimpleTestView::CancelButton,
                             tr(BoardRemovedButton),
                             BoardRemovedShort);

    RETURN_IF_FALSE(hmiDisplay.displayHmi(HmiName,
                                          hmiTextDisplayed,
                                          HmiDisplaySimpleTestView::CancelButton,
                                          _imagePath,
                                          getSequenceToken()));

    bool userAskedCancellation = false;

    WaitHelper::pseudoWait([this, &hmiDisplay, &userAskedCancellation]() {
        if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
        {
            hmiDisplay.cancelDisplaying();
            userAskedCancellation = true;
            return true;
        }

        if(hmiDisplay.getDisplayStatus() > HmiDisplayStatus::Displaying)
        {
            return true;
        }

        return false;
    });

    if(userAskedCancellation)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) Error displaying results "
                                                                    "has been cancelled by user")));
        return false;
    }

    return true;
}

bool DisplayErrorResults::initParamsFormat(JsonArray &paramFormat) const
{
    JsonArray parameters;

    JsonObject imagePathParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(BaseImagePathParamName,
                                                                 BaseImagePathParamDesc,
                                                                 imagePathParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, imagePathParam));
    parameters.append(imagePathParam);

    JsonObject errorNbParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(ErrorsNbToDisplayParamName,
                                                                 ErrorsNbToDisplayParamDesc,
                                                                 0,
                                                                 0,
                                                                 errorNbParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
        errorNbParam,
        Number(0),
        Number::maxInt32(),
        Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, errorNbParam));

    parameters.append(errorNbParam);

    paramFormat = parameters;

    return true;
}

bool DisplayErrorResults::initInputsFormat(JsonArray &inputFormat) const
{
    JsonObject errorsToDisplay;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(InputErrorsToDisplayName,
                                                              errorsToDisplay,
                                                              InputErrorsToDisplayDesc));
    inputFormat.append(errorsToDisplay);

    return true;
}

bool DisplayErrorResults::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getStringValue(parameters, BaseImagePathParamName, _imagePath, &error))
    {
        fillErrorArray(BaseImagePathParamName, error, errors);
        return false;
    }

    Number errorsNb;
    if(!BankJsonParser::getNumberValue(parameters, ErrorsNbToDisplayParamName, errorsNb, &error))
    {
        fillErrorArray(ErrorsNbToDisplayParamName, error, errors);
        return false;
    }

    // The number limits are tests in the validate module parameters
    _errorsNbToDisplay = errorsNb.toInt32();

    return true;
}
