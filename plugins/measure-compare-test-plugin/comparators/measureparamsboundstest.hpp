// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "ameasureboundstest.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief Test if the given measured value is contained in the boundaries set.
    @note The boundaries values are got from parameters. */
class MeasureParamsBoundsTest : public AMeasureBoundsTest
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(MeasureParamsBoundsTest)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit MeasureParamsBoundsTest(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~MeasureParamsBoundsTest() override;

    protected:
        /** @see AMeasureBoundsTest::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AMeasureBoundsTest::initParamsFormat
            @note @ref infBoundParamName : number (This is the inferior boundary for testing the
                                                   measure given)
                  @ref supBoundParamName : number (This is the superior boundary for testing the
                                                   measure given) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @brief The test is done in this method (it also generates the right business log)
            @param jsonObjectInputValues Inputs got from the sequence
            @param testSuccess This parameter contains the result of the test
            @return True if no problem occurs (if the test fails, it's not considered as an
                    error) */
        virtual bool processTest(const JsonArray &jsonObjectInputValues,
                                 bool &testSuccess) override;

    private:
        static const constexpr char *InfBoundParamName = "infBound";
        static const constexpr char *InfBoundParamDesc = "This is the inferior boundary for "
                                                         "testing the measure given";

        static const constexpr char *SupBoundParamName = "supBound";
        static const constexpr char *SupBoundParamDesc = "This is the superior boundary for "
                                                         "testing the measure given";

    private:
        Number _infBound;
        Number _supBound;
};
