// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QElapsedTimer>
#include <QString>

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"
#include "definesutility/translationsutility.hpp"
#include "hmi-helper-lib/hmidisplaysimpletestview.hpp"
#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "waitutility/waithelper.hpp"

#include "models/detectionshareddata.hpp"


/** @brief This mixin is used to add helpful method in module for detecting a board */
template<class T>
class MixinBoardDetection
{
    protected:
        /** @brief Class constructor */
        explicit MixinBoardDetection();

        /** @brief Class destructor */
        virtual ~MixinBoardDetection();

    protected:
        /** @brief Contains helpful parameters format to add to module in order to manage the
                   boad detection
            @note @ref BaseImagePathParamName : string (The path of the image to display with GUI)
            @note @ref DetectPresBoolParamName : bool (If enabled, wait to detect the board.
                                                       If disabled, wait to not detect the board
                                                       anymore)
            @note @ref HmiDescriptionParamName : string (Hmi text instruction if wait board
                                                         detection)
            @note @ref NoDetectHmiDescriptionParamName : string (Hmi text instruction if wait no
                                                                 board detection)
            @note @ref ManageBoardOnBenchParamName : bool (If enabled, it means that the board may
                                                           not be detected because an electronic
                                                           element is not working on the board.)
            @note @ref BoardOnBenchLogErrorParamName : string (This is the detail of error which
                                                               will be displayed in logs when
                                                               operator click on the board on bench
                                                               error button.)
            @note @ref TimeoutMsParamName : number (Timeout in Ms)
            @note @ref WaitBetweenTriesInMsParamName : number (This is the time to wait in
                                                               milliseconds between tries)
            @note @ref WaitForDisplayHmiInMsParamName : number (This is the time to wait in
                                                                milliseconds between displaying the
                                                                HMI. Use this when it's mostly
                                                                likely to have a compliant test
                                                                right now.)
            @param paramsFormat The parameters format to fill with new format
            @return True if no problem occurred  */
        virtual bool initDetectParamsFormat(JsonArray &paramsFormat) const;

        /** @brief Validate and set the parameters into the module
            @note First call the default validateAndSetParameters before this method
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurred */
        virtual bool validateAndSetDetectParameters(const JsonArray &parameters, JsonArray &errors);

    protected:
        /** @brief Call this method to start the board detection
            @note This method calls frequently the @ref boardDetectionProcess method
            @param sharedData The shared data to pass at each verification
            @param module The caller module
            @return True if no problem occurred */
        virtual bool detectBoard(const T& sharedData,
                                 DefaultSequenceInstanceModule &module);

        /** @brief Test if the board is detected, or not
            @note This method is called with high frequency, it's better to not reinit everything
                  in this method but to init things before and pass them in @ref sharedData
            @param sharedData The shared data given at each verification
            @param isDetected The board detection state is set in this method through this parameter
            @return True if no problem occurred */
        virtual bool boardDetectionProcess(const T& sharedData, bool &isDetected) = 0;

    private:
        static const constexpr char* BaseImagePathParamName = "baseImagePathName";
        static const constexpr char* BaseImagePathParamDesc = "The path of the image to display "
                                                              "with GUI";

        static const constexpr char* DetectPresBoolParamName = "detectPresence";
        static const constexpr char* DetectPresBoolParamDesc = "If enabled, wait to detect the "
                                                               "board. "
                                                               "If disabled, wait to not detect "
                                                               "the board anymore";
        static const constexpr bool DefaultDetectPresence = true;

        static const constexpr char* ManageBoardOnBenchParamName = "manageBoardOnBenchProblem";
        static const constexpr char* ManageBoardOnBenchParamDesc = "If enabled, it means that the "
                                                                   "board may not be detected "
                                                                   "because an electronic element "
                                                                   "is not working on the board.";
        static const constexpr bool DefaultManageBoardOnBench = false;

        static const constexpr char *BoardOnBenchLogErrorParamName = "boardOnBenchLogError";
        static const constexpr char *BoardOnBenchLogErrorParamDesc = "This is the detail of error "
                                                                     "which will be displayed in "
                                                                     "logs when operator click on "
                                                                     "the board on bench error "
                                                                     "button.";
        static const constexpr char *DefaultBoardOnBenchLogError = "A component is not working on "
                                                                   "the board.";

        static const constexpr char *BoardOnBenchLogErrorBase = "The board is on the bench but "
                                                                "it's not detected. What's might "
                                                                "cause the problem: %1.";

        static const constexpr char *TimeoutMsParamName = "detectTimeoutInMs";
        static const constexpr char *TimeoutMsParamDesc = "Timeout in Ms; set -1 to disable the "
                                                          "timeout";
        static const constexpr int DefaultTimeoutInMs = -1;

        static const constexpr char *WaitBetweenTriesInMsParamName = "waitBetweenTriesInMs";
        static const constexpr char *WaitBetweenTriesInMsParamDesc = "This is the time to wait in "
                                                                     "milliseconds between tries";
        static const constexpr int DefaultWaitBetweenTriesInMs = 1000;

        static const constexpr char *WaitForDisplayHmiInMsParamName = "waitForDisplayHmiInMs";
        static const constexpr char *WaitForDisplayHmiInMsParamDesc = "This is the time to wait in "
                                                                      "milliseconds between "
                                                                      "displaying the HMI. Use "
                                                                      "this when it's mostly "
                                                                      "likely to have a compliant "
                                                                      "test right now.";
        static const constexpr int DefaultWaitForDisplayHmiInMs = -1;


        static const constexpr char* HmiDescriptionParamName = "hmiDetectDesc";
        static const constexpr char* HmiDescriptionParamDesc = "Hmi text instruction if wait board "
                                                               "detection";

        static const constexpr char* NoDetectHmiDescriptionParamName = "hmiNoDetectDesc";
        static const constexpr char* NoDetectHmiDescriptionParamDesc = "Hmi text instruction if "
                                                                       "wait no board detection";

        static const constexpr char* HmiName = "WaitForBoard";
        static const constexpr char* DefaultHmiDescription = QT_TRANSLATE_NOOP(
            "MixinBoardDetection", "(TR) Put the board on the test bed.");
        static const constexpr char* HmiArgBoardOnBench = QT_TRANSLATE_NOOP(
            "MixinBoardDetection", "(TR) Put the board on the test bed.\n\n"
                                   "If you have put the board on the bench and this view is still "
                                   "displayed, click on button: '%1'.");
        static const constexpr char* DefaultNoDetectHmiDescription = QT_TRANSLATE_NOOP(
            "MixinBoardDetection", "(TR) Take the board out of the test bed.");

        static const constexpr char* BoardOnBenchButtonLabel = QT_TRANSLATE_NOOP(
            "MixinBoardDetection", "(TR) Error: on bench");
        static const constexpr char  BoardOnBenchKeyShorcut = 'e';

    private:
        int _timeoutInMs{DefaultTimeoutInMs};
        int _waitBetweenTriesInMs{DefaultWaitBetweenTriesInMs};
        int _waitForDisplayHmiInMs{DefaultWaitForDisplayHmiInMs};

        QString _imagePath;
        bool _isDetectingPresence{DefaultDetectPresence};

        bool _manageBoardNotDetecting{DefaultManageBoardOnBench};
        QString _boardOnBenchLogError;

        QString _hmiDescription;
        QString _noDetectHmiDescription;
};

template<class T>
MixinBoardDetection<T>::MixinBoardDetection()
{
    static_assert(std::is_base_of<DetectionSharedData, T>::value,
                  "The linked class is not based on ADetectionSharedData");
}

template<class T>
MixinBoardDetection<T>::~MixinBoardDetection()
{
}

template<class T>
bool MixinBoardDetection<T>::initDetectParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject detectBoardPresence;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(DetectPresBoolParamName,
                                                               DetectPresBoolParamDesc,
                                                               detectBoardPresence));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultDetectPresence,
                                                                   detectBoardPresence));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, detectBoardPresence));

    paramsFormat.append(detectBoardPresence);

    JsonObject imagePathParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(BaseImagePathParamName,
                                                                 BaseImagePathParamDesc,
                                                                 imagePathParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, imagePathParam));

    paramsFormat.append(imagePathParam);

    JsonObject hmiDescriptionParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(HmiDescriptionParamName,
                                                                 HmiDescriptionParamDesc,
                                                                 hmiDescriptionParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
        TR_WITH_CONTEXT("MixinBoardDetection", DefaultHmiDescription),
        hmiDescriptionParam));

    paramsFormat.append(hmiDescriptionParam);

    JsonObject noDetectHmiDescriptionParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(NoDetectHmiDescriptionParamName,
                                                                 NoDetectHmiDescriptionParamDesc,
                                                                 noDetectHmiDescriptionParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
        TR_WITH_CONTEXT("MixinBoardDetection", DefaultNoDetectHmiDescription),
        noDetectHmiDescriptionParam));

    paramsFormat.append(noDetectHmiDescriptionParam);

    JsonObject manageBoardOnBenchProblem;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(ManageBoardOnBenchParamName,
                                                               ManageBoardOnBenchParamDesc,
                                                               manageBoardOnBenchProblem));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultManageBoardOnBench,
                                                                   manageBoardOnBenchProblem));

    paramsFormat.append(manageBoardOnBenchProblem);

    JsonObject boardOnBenchLogError;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(BoardOnBenchLogErrorParamName,
                                                                 BoardOnBenchLogErrorParamDesc,
                                                                 boardOnBenchLogError));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
        TR_WITH_CONTEXT("MixinBoardDetection", DefaultBoardOnBenchLogError),
        boardOnBenchLogError));

    paramsFormat.append(boardOnBenchLogError);

    JsonObject timeoutObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(TimeoutMsParamName,
                                                                 TimeoutMsParamDesc,
                                                                 0,
                                                                 0,
                                                                 timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Second, -3),
                                                             timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(timeoutObj,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(DefaultTimeoutInMs),
        timeoutObj));

    paramsFormat.append(timeoutObj);

    JsonObject waitBetweenTriesObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(WaitBetweenTriesInMsParamName,
                                                                 WaitBetweenTriesInMsParamDesc,
                                                                 0,
                                                                 0,
                                                                 waitBetweenTriesObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Second, -3),
                                                             waitBetweenTriesObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(waitBetweenTriesObj,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(DefaultWaitBetweenTriesInMs),
        waitBetweenTriesObj));

    paramsFormat.append(waitBetweenTriesObj);

    JsonObject waitBeforeDisplayingHmiObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(WaitForDisplayHmiInMsParamName,
                                                                 WaitForDisplayHmiInMsParamDesc,
                                                                 0,
                                                                 0,
                                                                 waitBeforeDisplayingHmiObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Second, -3),
                                                             waitBeforeDisplayingHmiObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(waitBeforeDisplayingHmiObj,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(DefaultWaitForDisplayHmiInMs),
        waitBeforeDisplayingHmiObj));

    paramsFormat.append(waitBeforeDisplayingHmiObj);

    return true;
}

template<class T>
bool MixinBoardDetection<T>::validateAndSetDetectParameters(const JsonArray &parameters,
                                                            JsonArray &errors)
{
    QString parserError;

    _isDetectingPresence = DefaultDetectPresence;
    if(!BankJsonParser::getBoolValue(parameters,
                                     DetectPresBoolParamName,
                                     _isDetectingPresence,
                                     &parserError,
                                     true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(DetectPresBoolParamName, parserError, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       BaseImagePathParamName,
                                       _imagePath,
                                       &parserError))
    {
        DefaultSequenceInstanceModule::fillErrorArray(BaseImagePathParamName, parserError, errors);
        return false;
    }

    _manageBoardNotDetecting = DefaultManageBoardOnBench;
    if(!BankJsonParser::getBoolValue(parameters,
                                      ManageBoardOnBenchParamName,
                                      _manageBoardNotDetecting,
                                      &parserError,
                                      true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(ManageBoardOnBenchParamName,
                                                      parserError,
                                                      errors);
        return false;
    }

    if(_manageBoardNotDetecting && _isDetectingPresence)
    {
        _hmiDescription = TR_WITH_CONTEXT("MixinBoardDetection", HmiArgBoardOnBench)
                              .arg(TR_WITH_CONTEXT("MixinBoardDetection",
                                                   BoardOnBenchButtonLabel));
    }
    else
    {
        _hmiDescription = TR_WITH_CONTEXT("MixinBoardDetection", DefaultHmiDescription);
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       HmiDescriptionParamName,
                                       _hmiDescription,
                                       &parserError,
                                       true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(HmiDescriptionParamName, parserError, errors);
        return false;
    }

    _noDetectHmiDescription = TR_WITH_CONTEXT("MixinBoardDetection", DefaultNoDetectHmiDescription);
    if(!BankJsonParser::getStringValue(parameters,
                                       NoDetectHmiDescriptionParamName,
                                       _noDetectHmiDescription,
                                       &parserError,
                                       true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(NoDetectHmiDescriptionParamName,
                                                      parserError,
                                                      errors);
        return false;
    }

    _boardOnBenchLogError = DefaultBoardOnBenchLogError;
    if(!BankJsonParser::getStringValue(parameters,
                                       BoardOnBenchLogErrorParamName,
                                       _boardOnBenchLogError,
                                       &parserError,
                                       true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(BoardOnBenchLogErrorParamName,
                                                      parserError,
                                                      errors);
        return false;
    }

    Number timeoutInMs;
    if(!BankJsonParser::getNumberValue(parameters,
                                       TimeoutMsParamName,
                                       timeoutInMs,
                                       &parserError,
                                       true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(TimeoutMsParamName, parserError, errors);
        return false;
    }

    // The integer boundaries are already tested before
    if(timeoutInMs.isValid())
    {
        _timeoutInMs = timeoutInMs.toInt32();
    }
    else
    {
        _timeoutInMs = DefaultTimeoutInMs;
    }

    Number waitBetweenTriesInMs;
    if(!BankJsonParser::getNumberValue(parameters,
                                        WaitBetweenTriesInMsParamName,
                                        waitBetweenTriesInMs,
                                        &parserError,
                                        true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(WaitBetweenTriesInMsParamName,
                                                      parserError,
                                                      errors);
        return false;
    }

    // The integer boundaries are already tested before
    if(waitBetweenTriesInMs.isValid())
    {
        _waitBetweenTriesInMs = waitBetweenTriesInMs.toInt32();
    }
    else
    {
        _waitBetweenTriesInMs = DefaultWaitBetweenTriesInMs;
    }

    Number waitForDisplayHmiInMs;
    if(!BankJsonParser::getNumberValue(parameters,
                                        WaitForDisplayHmiInMsParamName,
                                        waitForDisplayHmiInMs,
                                        &parserError,
                                        true))
    {
        DefaultSequenceInstanceModule::fillErrorArray(WaitForDisplayHmiInMsParamName,
                                                      parserError,
                                                      errors);
        return false;
    }

    // The integer boundaries are already tested before
    if(waitForDisplayHmiInMs.isValid())
    {
        _waitForDisplayHmiInMs = waitForDisplayHmiInMs.toInt32();
    }
    else
    {
        _waitForDisplayHmiInMs = DefaultWaitForDisplayHmiInMs;
    }

    return true;
}

template<class T>
bool MixinBoardDetection<T>::detectBoard(const T& sharedData,
                                         DefaultSequenceInstanceModule &module)
{
    module.onNewSeqLogs(SequenceLogData(LogMsgType::Info,
                                        SequenceLogCategory::BusinessScope,
                                        _isDetectingPresence ? "Wait for detecting the board" :
                                            "Wait to no more detect the board"));

    HmiDisplaySimpleTestView hmiDisplay(module.accessCommIntf());
    QElapsedTimer timeToWaitToDisplay;
    timeToWaitToDisplay.start();

    auto displayHmiFunc = [this, &hmiDisplay, &module, &timeToWaitToDisplay]()
    {
        if(hmiDisplay.getDisplayStatus() >= HmiDisplayStatus::Displaying ||
            (_waitForDisplayHmiInMs > 0 &&
             !timeToWaitToDisplay.hasExpired(_waitForDisplayHmiInMs)))
        {
            // Nothing to do yet or to do more
            return true;
        }

        timeToWaitToDisplay.invalidate();

        quint8 buttons = HmiDisplaySimpleTestView::CancelButton;
        HmiDisplaySimpleTestView::ButtonEnum defaultButton = HmiDisplaySimpleTestView::CancelButton;
        if(_isDetectingPresence && _manageBoardNotDetecting)
        {
            buttons |= HmiDisplaySimpleTestView::NotOkButton;
            defaultButton = HmiDisplaySimpleTestView::NotOkButton;
            hmiDisplay.setButtonName(HmiDisplaySimpleTestView::NotOkButton,
                                     TR_WITH_CONTEXT("MixinBoardDetection",
                                                     BoardOnBenchButtonLabel),
                                     BoardOnBenchKeyShorcut);
        }

        return hmiDisplay.displayHmi(HmiName,
                                     _isDetectingPresence ? _hmiDescription :
                                         _noDetectHmiDescription,
                                     buttons,
                                     _imagePath,
                                     module.getSequenceToken(),
                                     defaultButton);
    };

    RETURN_IF_FALSE(displayHmiFunc());

    bool cancelledOrErrorOccurred = false;

    auto waitTestFunc = [this, &sharedData, &module, &hmiDisplay, &displayHmiFunc,
                         &cancelledOrErrorOccurred]()
    {
        if(!displayHmiFunc())
        {
            cancelledOrErrorOccurred = true;
            return true;
        }

        if(!PauseAndStopHelper::managePauseAndStop(module.getSequenceState()) ||
            (hmiDisplay.getDisplayStatus() > HmiDisplayStatus::Displaying))
        {
            cancelledOrErrorOccurred = true;
            hmiDisplay.cancelDisplaying();

            HmiDisplaySimpleTestView::ButtonEnum buttonClicked = hmiDisplay.getButtonClicked();

            if(buttonClicked == HmiDisplaySimpleTestView::CancelButton)
            {
                module.onNewSeqLogs(SequenceLogFormatter::formatABusinessProblem(
                    TR_WITH_CONTEXT("MixinBoardDetection",
                                    "Detection has been cancelled by user")));
            }
            else if(_manageBoardNotDetecting &&
                       buttonClicked == HmiDisplaySimpleTestView::NotOkButton)
            {
                module.onNewSeqLogs(SequenceLogFormatter::formatABusinessProblem(
                    QString(BoardOnBenchLogErrorBase).arg(_boardOnBenchLogError)));
            }

            return true;
        }

        bool isDetected = false;
        if(!boardDetectionProcess(sharedData, isDetected))
        {
            cancelledOrErrorOccurred = true;
            return true;
        }

        bool waitingIsEnded = (_isDetectingPresence && isDetected) ||
                              (!_isDetectingPresence && !isDetected);

        if(!waitingIsEnded && _waitBetweenTriesInMs > 0)
        {
            WaitHelper::pseudoSleep(_waitBetweenTriesInMs);
        }

        return waitingIsEnded;
    };

    WaitHelper::pseudoWait(waitTestFunc, _timeoutInMs);

    if(cancelledOrErrorOccurred)
    {
        module.onNewSeqLogs(SequenceLogFormatter::formatABusinessProblem(
            _isDetectingPresence ?
                TR_WITH_CONTEXT("MixinBoardDetection",
                                "A problem occured while trying to detect the board") :
                TR_WITH_CONTEXT("MixinBoardDetection",
                                "A problem occured while waiting to no more detect the board")));
        return false;
    }

    module.onNewSeqLogs(SequenceLogData(LogMsgType::Info,
                                        SequenceLogCategory::BusinessScope,
                                        _isDetectingPresence ?
                                            TR_WITH_CONTEXT("MixinBoardDetection",
                                                            "The board is detected") :
                                            TR_WITH_CONTEXT("MixinBoardDetection",
                                                            "The board is no more detected")));

    return true;
}
