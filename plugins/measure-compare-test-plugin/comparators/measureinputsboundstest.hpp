// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "ameasureboundstest.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Test if the given measured value is contained in the boundaries set.
    @note The boundaries values are got from inputs. */
class MeasureInputsBoundsTest : public AMeasureBoundsTest
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(MeasureInputsBoundsTest)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit MeasureInputsBoundsTest(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~MeasureInputsBoundsTest() override;

    protected:
        /** @brief The test is done in this method (it also generates the right business log)
            @param jsonObjectInputValues Inputs got from the sequence
            @param testSuccess This parameter contains the result of the test
            @return True if no problem occurs (if the test fails, it's not considered as an
                    error) */
        virtual bool processTest(const JsonArray &jsonObjectInputValues,
                                 bool &testSuccess) override;

        /** @see AMeasureBoundsTest::initInputsFormat
            @note @ref inputInfBoundName : number (This is the inferior boundary for testing
                                                   the measure given)
                  @ref inputSupBoundName : number (This is the superior boundary for testing
                                                   the measure given) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    private:
        static const constexpr char *InputInfBoundName = "infBound";
        static const constexpr char *InputInfBoundDesc = "This is the inferior boundary for "
                                                         "testing the measure given";

        static const constexpr char *InputSupBoundName = "supBound";
        static const constexpr char *InputSupBoundDesc = "This is the superior boundary for "
                                                         "testing the measure given";
};
