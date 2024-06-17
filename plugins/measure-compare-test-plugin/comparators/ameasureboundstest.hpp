// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

class Number;


/** @brief Abstract class to test if a given measure is included in boundaries
    @note If the measure isn't included in the boundaries and:
            - if the fatalError param is equal to true, this will raise an error and stop the
              sequence
            - if the fatalError param is equal to false, this will output normaly and return false
              in the output variable */
class AMeasureBoundsTest : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit AMeasureBoundsTest(const QString &moduleClassName,
                                    QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AMeasureBoundsTest() override = default;

    protected slots:
        /** @brief Called in an asynchronous way by the launchModule method
            @param jsonObjectInputValues The input values given by the test bed lib */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref elementTestedParamName : string (Description of the element tested)
                  @ref testDescriptionParamName : string (The description of the test)
                  @ref fatalErrorParamName : boolean (If the test fails, this param says if we
                                                      have to stop the sequence or not) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputMeasureDoneName : number (This is the measure to test with
                                                      boundaries) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputSuccessName : boolean (Tell if the test has been successful) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @brief Validate and set the parameters into the module
            @see ISequenceInstanceModule::setModuleParameters
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurs */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief The test is done in this method (it also generates the right business log)
            @param jsonObjectInputValues Inputs got from the sequence
            @param testSuccess This parameter contains the result of the test
            @return True if no problem occurs (if the test fails, it's not considered as an
                    error) */
        virtual bool processTest(const JsonArray &jsonObjectInputValues, bool &testSuccess) = 0;

        /** @brief Compare the measure done with the bounds given
            @param jsonObjectInputValues The input values which contains the measure done
            @param infBound The inferior bound to compare the measure with
            @param supBound The superior bound to compare the measure with
            @param testSuccess True if the measure done is contained between the bounds
            @return True if no problem occurs */
        bool compare(const JsonArray &jsonObjectInputValues,
                     const Number &infBound,
                     const Number &supBound,
                     bool &testSuccess);

    private:
        static const constexpr char *ElementTestedParamName = "elementTested";
        static const constexpr char *ElementTestedParamDesc = "Description of the element tested";

        static const constexpr char *TestDescriptionParamName = "testDescription";
        static const constexpr char *TestDescriptionParamDesc = "The description of the test";

        static const constexpr char *FatalErrorParamName = "fatalError";
        static const constexpr char *FatalErrorParamDesc = "If the test fails, this param says if "
                                                           "we have to stop the sequence or not";

        static const constexpr char *InfBoundaryIncludedParamName = "isInfBoundaryIncluded";
        static const constexpr char *InfBoundaryIncludedParamDesc = "If true, the inferior "
                                                                    "boundary given is included in "
                                                                    "test, same as: inf <= x. "
                                                                    "If false, the inferior "
                                                                    "boundary isn't included in "
                                                                    "test, same as: inf < x.";
        static const constexpr bool DefaultInfBoundaryIncludedValue = true;

        static const constexpr char *SupBoundaryIncludedParamName = "isSupBoundaryIncluded";
        static const constexpr char *SupBoundaryIncludedParamDesc = "If true, the superior "
                                                                    "boundary given is included in "
                                                                    "test, same as: x <= sup. "
                                                                    "If false, the superior "
                                                                    "boundary isn't included in "
                                                                    "test, same as: x < supr.";
        static const constexpr bool DefaultSupBoundaryIncludedValue = true;

        static const constexpr char *InputMeasureDoneName = "measureDone";
        static const constexpr char *InputMeasureDoneDesc = "This is the measure to test with "
                                                            "boundaries.";

        static const constexpr char *OutputSuccessName = "success";
        static const constexpr char *OutputSuccessDesc = "Tell if the test has been successful";

        static const constexpr bool DefaultFatalErrorValue = true;

    private:
        QString _elementTested;
        QString _testDescription;
        bool _fatalError{DefaultFatalErrorValue};
        bool _isInfBoundaryIncluded { DefaultInfBoundaryIncludedValue };
        bool _isSupBoundaryIncluded { DefaultSupBoundaryIncludedValue };
};
