// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkintffromparams.hpp"
#include "detection-helper-lib/modules/mixinboarddetection.hpp"

#include <QElapsedTimer>

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

#include "models/detectjlinkshareddata.hpp"

class HmiDisplaySimpleTestView;


/** @brief The module tries to detect (or not) a board connecting to a JLink probe. */
class DetectBoard : public AJLinkIntfFromParams,
                    public MixinBoardDetection<DetectJLinkSharedData>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(DetectBoard)

    public:
        /** @brief Class constructor
            @param parent The class parent */
        Q_INVOKABLE explicit DetectBoard(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DetectBoard() override;

    protected:
        /** @copydoc AJLinkIntfFromParams::initParamsFormat
            @note @ref BaseImagePathParamName : string (The path of the image to display with GUI)
                  @ref DetectPresBoolParamName : boolean (If enabled, wait to detect the board. If
                                                          disabled, wait to not detect the board
                                                          any more) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc AJLinkIntfFromParams::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc AJLinkIntfFromParams::mainJLinkProcess */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) override;

        /** @copydoc MixinBoardDetection::boardDetectionProcess */
        virtual bool boardDetectionProcess(const DetectJLinkSharedData& sharedData,
                                           bool &isDetected) override;

    private:
        static const constexpr int DefaultTimeoutLoopInMs = -1;
};
