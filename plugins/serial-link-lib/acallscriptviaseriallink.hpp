// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "aseriallinkwriteread.hpp"
#include "types/seriallinkreceivedreplytype.hpp"

class SerialLinkIntf;


/** @brief This abstract module let user call a script through a serial line and get the result of
           it
    @note This module opens and close serial port on-purpose if not previously opened */
class ACallScriptViaSerialLink : public ASerialLinkWriteRead
{
    Q_OBJECT

    public:
        /** @brief Module constructor
            @param moduleClassName The module class name
            @param isMultiLine False if what we expect is on one line
                               True if the expected reply may be on multiple line
            @param bankLinked The bank linked to the instance module
            @param parent Qt parentship */
        explicit ACallScriptViaSerialLink(const QString &moduleClassName,
                                          bool isMultiLine,
                                          const QSharedPointer<DefaultBankInterface> &bankLinked,
                                          QObject *parent = nullptr);

        /** @brief Module constructor
            @param moduleClassName The module class name
            @param bankLinked The bank linked to the instance module
            @param parent Qt parentship */
        explicit ACallScriptViaSerialLink(const QString &moduleClassName,
                                          const QSharedPointer<DefaultBankInterface> &bankLinked,
                                          QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ACallScriptViaSerialLink() override;

    protected:
        /** @brief Overwrite default value of verbose logs activation.
                   Should be called from constructors to deactivate verbose logs by default
            @param verboseValue The default value of the verbose parameter */
        void setVerboseLogs(bool verboseValue);

        /** @copydoc ASerialLinkWriteRead::initParamsFormat
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::EndOfLine param
            @note @ref ErrorCodesMeaningParamName : string (This gives a meaning to the encountered
                                                            error codes. The format of each param
                                                            is: \"ERROR_CODE/ERROR_MEANING\")
            @note @ref TestDescriptionParamName : string (This is the test description)
            @note @ref TestedElementParamName : string (This is the tested element)
            @note @ref ScriptToCallParamName : string (This is the script to call) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ASerialLinkWriteRead::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc ASerialLinkWriteRead::manageWriteReadProcess */
        virtual bool manageWriteReadProcess(const JsonArray &jsonObjectInputValues,
                                            const QSharedPointer<SerialLinkIntf> &serialIntf,
                                            SerialLinkReceivedReplyType::Enum &receivedReply,
                                            QVector<QString> &capturedElements) override;

        /** @copydoc ASerialLinkWriteRead::setExpectedPattern */
        virtual bool setExpectedPattern(const QString &pattern) override;

        /** @brief Let the derived class parses the error code and element captured, to do extra
                   tests on them
            @param errorCode The script error code
            @param capturedElements Those are the captured elements by the expected reply regex
                                    The first element is the result of whole match,
                                    The last element is the return code of the script
            @return True if no problem occurred */
        virtual bool manageCapturedElements(quint8 errorCode,
                                            const QVector<QString> &capturedElements);

        /** @brief Before calling the script to call, this method is called and may change the
                   script
            @note By default, the method copy the value of @ref scriptToCall into
                  @ref managedScriptToCall
            @param jsonObjectInputValues The module input values
            @param scriptToCall The script to call as it has been got from parameters
            @param managedScriptToCall The script updated by the method
            @return True if no problem occurred */
        virtual bool manageScriptToCall(const JsonArray &jsonObjectInputValues,
                                        const QString &scriptToCall,
                                        QString &managedScriptToCall);

        /** @brief Get the test description */
        const QString &getTestDescription() const { return _testDescription; }

        /** @brief Get the tested element */
        const QString &getTestedElement() const { return _testedElement; }

    private:
        /** @brief Parse the error codes to extract the value and meaning
            @param errorCodes The list of the error codes and their meaning. The format of earch
                              element is "ERROR_CODE/ERROR_MEANING"
            @param parsedErrorCodes The error codes parsed
            @return True if no problem occurred */
        static bool parseErrorCodes(const QVector<QString> &errorCodes,
                                    QHash<quint8, QString> &parsedErrorCodes);

    private:
        static const constexpr char *ErrorCodesMeaningParamName = "errorCodesMeaning";
        static const constexpr char *ErrorCodesMeaningParamDesc = "This gives a meaning to the "
                                                                  "encountered error codes. The "
                                                                  "format of each param is: "
                                                                  "\"ERROR_CODE/ERROR_MEANING\"";

        static const constexpr char *TestDescriptionParamName = "testDescription";
        static const constexpr char *TestDescriptionParamDesc = "This is the test description";

        static const constexpr char *TestedElementParamName = "testedElement";
        static const constexpr char *TestedElementParamDesc = "This is the tested element";

        static const constexpr char *ScriptToCallParamName = "scriptToCall";
        static const constexpr char *ScriptToCallParamDesc = "This is the script to call";

        static const constexpr char *VerboseLogsParamName = "verboseLogs";
        static const constexpr char *VerboseLogsParamDesc = "This is Boolean used to enable or "
                                                            "disable status in logs";

        /** @brief This is the separator used to parse the error codes meaning */
        static const constexpr char ErrorCodeSeparator = '/';

        /** @brief This is the index of the code in the error code meaning string */
        static const constexpr int ErrorCodePartIdx = 0;

        /** @brief This is the index of the meaning in the error code meaning string */
        static const constexpr int ErrorCodeMeaningPartIdx = 1;

        /** @brief This is the length of the error code meaning string parts */
        static const constexpr int ErrorCodePartsLength = 2;

        /** @brief This is the base to add in each script call, to get the script error code
                   return */
        static const constexpr char *GlobalFormatScriptCall = "%1; echo \"DONERE=${?};\"";

        /** @brief This is the regex used to extract the script error code */
        static const constexpr char *ScriptCallExpectedEndScriptPattern =
            R"(DONERE=([0-9]{1,3});)";

        /** @brief This is the default tested element text used when no one has been set by the
                   user */
        static const constexpr char *TestedElementDefaultValue = "The script: %1, has been called";

        /** @brief This is minimum number of expected captured groups */
        static const constexpr int MinCapturedGroupsNb = 2;

        /** @brief This pattern is used to merge the @ref ScriptCallExpectedEndScriptPattern regex
                   pattern and the one given by the derived class */
        static const constexpr char *MergePatterns = R"(%1[\s\S]*%2)";

    private:
        QHash<quint8, QString> _errorCodesMeaning;
        QString _testDescription;
        QString _testedElement;
        QString _scriptToCall;
        bool _verboseLogs {true};
};
