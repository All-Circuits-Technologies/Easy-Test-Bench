// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "serial-link-lib/acallscriptviaseriallink.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief This module lets the user calls a script through a serial link and gets measured
           information from the script logs */
class CompareMeasureFromScriptLogs : public ACallScriptViaSerialLink
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CompareMeasureFromScriptLogs)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit CompareMeasureFromScriptLogs(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CompareMeasureFromScriptLogs() override;

    protected:
        /** @copydoc ACallScriptViaSerialLink::initParamsFormat
            @note The following elements are always added. To see the optional elements see
                  @ref addParametersFormat
            @note @ref MeasurePatternParamName : string (This is the pattern of regular expression
                                                         to extract the measure value from the
                                                         script logs.
                                                         To extract properly the value, you have to
                                                         set only one capturing group with
                                                         parenthesis \"()\".)
            @note @ref InfToleranceBoundParamName : number (This is the inferior tolerance bound to
                                                            compare with.)
            @note @ref SupToleranceBoundParamName : number (This is the superior tolerance bound to
                                                            compare with.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACallScriptViaSerialLink::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc ACallScriptViaSerialLink::manageCapturedElements */
        virtual bool manageCapturedElements(quint8 errorCode,
                                            const QVector<QString> &capturedElements) override;

    private:
        static const constexpr char *MeasurePatternParamName = "measurePattern";

        static const constexpr char *InfToleranceBoundParamName = "infToleranceBound";
        static const constexpr char *InfToleranceBoundParamDesc = "This is the inferior tolerance "
                                                                  "bound to compare with.";

        static const constexpr char *SupToleranceBoundParamName = "supToleranceBound";
        static const constexpr char *SupToleranceBoundParamDesc = "This is the superior tolerance "
                                                                  "bound to compare with.";

        /** @brief Index of the measured return value in the captured list */
        static const constexpr int MeasuredIdxInCapturedList = 1;

        /** @brief This is the expected captured list length */
        static const constexpr int ExpectedCapturedListLength = 3;

    private:
        Number _infTolerance;
        Number _supTolerance;
};
