// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"
#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

class HmiDisplayStringInputView;


/** @brief  The module asks the user to scan a datamatrix and returns the string returned by the
            widget. */
class ScanDataMatrix : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ScanDataMatrix)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit ScanDataMatrix(QObject *parent = nullptr);

    protected slots:
        /** @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref OutputDataName : string (The string data read from data matrix) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref ImagePathParamName : string (This is the image path to display)
                  @ref HmiDescriptionParamName : string (Hmi text instruction for input view)
                  @ref ClearFieldIfErrorParamName : boolean (If set to true and if the input isn't
                                                             correctly filled (checked by regex),
                                                             when the user enters new information if
                                                             will first erase the data already
                                                             contained in the input.)
                  @ref ValidRegexPatternParamName : string (This is the pattern used to validate the
                                                            entered input information by user. If
                                                            the regexp contains captured brackets,
                                                            the returned will be a concatenated
                                                            result of the captured elements.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Launch the view to get input info from user
            @param output This is the formatted output of the module
            @return True if no problem occurred */
        bool operate(JsonArray &output);

        /** @brief Ask for user input
            @param userInput the input got from user
            @return True if no problem occurred */
        bool askForInput(QString &userInput);

        /** @brief Parse the HMI received result and get the capture elements */
        bool parseInput(const JsonArray &hmiResultReceived, QString &parsedInput);

    private:
        static const constexpr char *ImagePathParamName = "imagePath";
        static const constexpr char *ImagePathParamDesc = "This is the image path to display";

        static const constexpr char *DefaultImagePath = ":/scan.jpg";

        static const constexpr char *OutputDataName = "data";
        static const constexpr char *OutputDataDesc = "The string data read from data matrix";

        static const constexpr char* HmiDescriptionParamName = "hmiDetectDesc";
        static const constexpr char* HmiDescriptionParamDesc = "Hmi text instruction for input "
                                                               "view";
        static const constexpr char *DefaultHmiDescription = QT_TR_NOOP("(TR) Waiting the scan of "
                                                                        "barecode");

        static const constexpr char *ClearFieldIfErrorParamName = "clearFieldIfError";
        static const constexpr char *ClearFieldIfErrorParamDesc = "If set to true and if the input "
                                                                  "isn't correctly filled (checked "
                                                                  "by regex), when the user enters "
                                                                  "new information if will first "
                                                                  "erase the data already "
                                                                  "contained in the input.";
        static const constexpr bool DefaultClearFieldIfError = true;

        static const constexpr char *ValidRegexPatternParamName = "regexPattern";
        static const constexpr char *ValidRegexPatternParamDesc = "This is the pattern used to "
                                                                  "validate the entered input "
                                                                  "information by user. If the "
                                                                  "regexp contains captured "
                                                                  "brackets, the returned will be "
                                                                  "a concatenated result of the "
                                                                  "captured elements.";
        static const constexpr char *ValidRegexPatternRegex =
          R"(^(?>(?>(?>(?>\(\?>)|[^(\r\n])*\((?>(?>\(\?>)|[^(\r\n])*\))|(?>(?>\(\?>)|[^(\r\n]))*$)";
        static const constexpr char *InvalidPatternRegExpError = "The given string isn't a regexp "
                                                                 "pattern: %1";

        QString _imagePath {DefaultImagePath};
        QString _hmiDescription{DefaultHmiDescription};
        QString _regExpPattern;
        bool _clearFieldIfError{DefaultClearFieldIfError};
};
