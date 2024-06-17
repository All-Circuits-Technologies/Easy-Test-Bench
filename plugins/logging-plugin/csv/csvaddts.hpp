// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "acsvadd.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module appends the current date time to the string list given, which is the
           representation of a CSV line.
    @note You can specify the expected string format of the date time to write.
    @note If `datetimeStrFormat` isn't empty, it will be used instead of `datetimeEnumFormat`. */
class CsvAddTs : public ACsvAdd
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CsvAddTs)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit CsvAddTs(QObject *parent = nullptr);

    protected:
        /** @see ACsvAdd::append */
        virtual bool append(const JsonArray &jsonObjectInputValues,
                            QVector<QString> &csvLine) override;

        /** @see ACsvAdd::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                          JsonArray &errors) override;

        /** @brief @see ACsvAdd::initParamsFormat
            @note @ref timeSpecParamName : array<string> (This describe the time spec to use
                                                          in order to create the timestamp)
            @note @ref datetimeFormatEnumParamName : array<string> (This contains the format (enum
                                                                    value) to apply to the
                                                                    datetime)
            @note @ref datetimeFormatStrParamName : string (This contains the format
                                                            (pattern) to apply to the
                                                            datetime) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

    private:
        static const constexpr char *TimeSpecParamName = "timeSpec";
        static const constexpr char *TimeSpecParamDesc = "This describes the time spec to use "
                                                         "in order to create the timestamp";

        static const constexpr char *DatetimeFormatEnumParamName = "datetimeEnumFormat";
        static const constexpr char *DatetimeFormatEnumParamDesc = "This contains the format (enum "
                                                                   "value) to apply to the "
                                                                   "datetime";

        static const constexpr char *DatetimeFormatStrParamName = "datetimeStrFormat";
        static const constexpr char *DatetimeFormatStrParamDesc = "This contains the format "
                                                                  "(pattern) to apply to the "
                                                                  "datetime";

        static const constexpr Qt::TimeSpec TimeSpecDefaultValue = Qt::UTC;

        static const constexpr Qt::DateFormat DatetimeFormatDefaultValue = Qt::ISODateWithMs;

    private:
        Qt::TimeSpec _timeSpec{ TimeSpecDefaultValue };
        Qt::DateFormat _datetimeFormat{ DatetimeFormatDefaultValue };
        QString _datetimeFormatPattern;
};
