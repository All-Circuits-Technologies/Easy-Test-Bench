// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "serial-link-lib/acallscriptviaseriallink.hpp"
#include "detection-helper-lib/modules/mixinboarddetection.hpp"

#include "serial-link-lib/types/seriallinkreceivedreplytype.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

#include "models/serialdetectshareddata.hpp"


/** @brief This module lets the user calls a script through a serial link to
 *         confirm element connection */
class DetectFromScriptLogs : public ACallScriptViaSerialLink,
                             public MixinBoardDetection<SerialDetectSharedData>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(DetectFromScriptLogs)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit DetectFromScriptLogs(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DetectFromScriptLogs() override;

    protected:
        /** @copydoc ACallScriptViaSerialLink::initParamsFormat
            @note The following elements are always added. To see the optional elements see
                  @ref addParametersFormat
            @note @ref ExpectedValuePatternParamName : string (This is the pattern of regular
                                                         expression to extract the value from the
                                                         script logs.
                                                         To extract properly the value, you have to
                                                         set only one capturing group with
                                                         parenthesis \"()\".)
            @note @ref ExpectedValueParamName : string (This is the value expected in return
                                                        from the script to detect the element) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACallScriptViaSerialLink::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc ACallScriptViaSerialLink::manageWriteReadProcess */
        virtual bool manageWriteReadProcess(const JsonArray &jsonObjectInputValues,
                                            const QSharedPointer<SerialLinkIntf> &serialIntf,
                                            SerialLinkReceivedReplyType::Enum &receivedReply,
                                            QVector<QString> &capturedElements) override;

        /** @copydoc MixinBoardDetection::boardDetectionProcess */
        virtual bool boardDetectionProcess(const SerialDetectSharedData& sharedData,
                                           bool &isDetected) override;

        /** @brief Let the derived class parses the element captured, to get element detection value
            @param capturedElements Those are the captured elements by the expected reply regex
                                    The first element is the result of whole match,
                                    The last element is the return code of the script
            @param detectionStatus The value of the gpio */
        bool manageDetectElements(const QVector<QString> &capturedElements, bool *detectionStatus);

    private:
        static const constexpr char *ExpectedValuePatternParamName = "scriptResultPattern";

        /** @brief Index of the script return value in the captured list */
        static const constexpr int ExpectedValueIdxInCapturedList = 1;

        /** @brief This is the expected captured list length:
         *                           - 0: script content
         *                           - 1: script echo value
         *                           - 2: script error code with $? */
        static const constexpr int ExpectedCapturedListLength = 3;

        static const constexpr char* ExpectedValueParamName = "expectedValue";
        static const constexpr char* ExpectedValueParamDesc = "the value expected in return from "
                                                              "the script to detect the element";
    private:
        QString _expectedValue;
};
