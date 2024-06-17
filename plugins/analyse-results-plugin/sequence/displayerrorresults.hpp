// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"

class HmiDisplaySimpleTestView;


/** @brief Display the last error results and wait the user to remove the board from the test bed
    @note We wait the user to explicitly remove its board from the test bench. */
class DisplayErrorResults : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(DisplayErrorResults)

    public:
        /** @brief Class constructor
            @param parent Parent class constructor */
        Q_INVOKABLE explicit DisplayErrorResults(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DisplayErrorResults() override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note Display an HMI to manage user cancellation if needed */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief Get the parameters format
            @param paramFormat The parameters format
            @return True if no problem occurs */
        virtual bool initParamsFormat(JsonArray &paramFormat) const override;

        /** @brief Get the inputs format
            @param inputFormat The inputs format
            @return True if no problem occurs */
        virtual bool initInputsFormat(JsonArray &inputFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters
            @note @ref errorsNbToDisplayParamName : number (Errors number to display)
                  @ref baseImagePathParamName : string (The path of the image to display) */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Display to user the errors given
            @note If no errors have been given or if the module parameter @ErrorsNbToDisplayName
                  has a value equals to 0, nothing more than the main messsage is displayed
            @note The module parameter @ErrorsNbToDisplayName allowes to set a limit to the elements
                  number to display
            @param jsonObjectInputValues The errors to display in the view
            @return True if no error occurs */
        bool displayErrorResults(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char* HmiName = "DisplayErrors";
        static const constexpr char* HmiDescription = QT_TR_NOOP("(TR) At least one error occured "
                                                                 "while testing the board."
                                                                 "%1"
                                                                 "The board has to be put away.");
        static const constexpr char* HmiDescWithErrors = QT_TR_N_NOOP(
                                                                "(TR) Display the last %n error(s):"
                                                                "\n%1");
        static const constexpr char* HmiDescNoError = "\n";

        static const constexpr char* BoardRemovedButton = QT_TR_NOOP("(TR) Board removed");
        static const constexpr char  BoardRemovedShort = 'b';

        static const constexpr char* ErrorsNbToDisplayParamName = "errorsNbToDisplay";
        static const constexpr char* ErrorsNbToDisplayParamDesc = "Number of error logs to display";

        static const constexpr char* BaseImagePathParamName = "baseImagePathName";
        static const constexpr char* BaseImagePathParamDesc = "The path of the image to display "
                                                              "with GUI";

        static const constexpr char* InputErrorsToDisplayName = "inputErrorsToDisplay";
        static const constexpr char* InputErrorsToDisplayDesc = "The errors to display in HMI";

    private:
        QString _imagePath;
        int _errorsNbToDisplay{0};
};
