// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/aatmelprogramitf.hpp"
#include "detection-helper-lib/modules/mixinboarddetection.hpp"

#include <QElapsedTimer>

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

#include "models/detectatmelshareddata.hpp"


/** @brief This module is used to detect the tested board through the atmel probe (or detect if
           none is present).
    @note The module also displays a pop-up in the application to inform the user of what he has to
          do:
            - if we want to detect the board, we ask the user to put the board in test bench,
            - if we want to not detect the board, we ask the user to take the board out the test
              bench. */
class DetectBoardAtProgram : public AAtmelProgramItf,
                             public MixinBoardDetection<DetectAtmelSharedData>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(DetectBoardAtProgram)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit DetectBoardAtProgram(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DetectBoardAtProgram() override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note The module display a view to tell the user that we are waiting for the board */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @see AAtmelProgramItf::initParamsFormat
            @note @ref imagePathParamName : string (The path of the image to display in the GUI.)
            @note @ref detectPresenceParamName : bool (If true, the module waits to detect a bord.
                                                       If false, the module waits to be no more
                                                            connected to a board.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AAtmelProgramItf::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc MixinBoardDetection::boardDetectionProcess */
        virtual bool boardDetectionProcess(const DetectAtmelSharedData& sharedData,
                                           bool &isDetected) override;

    private:
        /** @brief Process the detection feature
            @note displays a modal view to inform the user of what we expect of him
            @param jsonObjectInputValues The input object values to parse
            @return True if no problem occurs */
        bool detectBoardProcess(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr int DefaultTimeoutLoopInMs = -1;
};
