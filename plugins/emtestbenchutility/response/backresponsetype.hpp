// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of the back response value */
class BackResponseType : public QObject
{
    Q_OBJECT

    public:
        enum Enum: quint16
        {
            RR00 = 0,   // The Test procedure was stopped correctly
            RR01 = 1,   // One single pulse was triggered
            RR02 = 2,   // The simulator is ready to be discharged
            RR05 = 5,   // Fail 1
            RR06 = 6,   // Fail 2
            RR07 = 7,   // Continue execution after Fail 2 (RR 06)
            RR10 = 10,  // Data transmission error, too less or too much parameters
            RR11 = 11,  // Start not possible: Test On key not pushed or safety circuit not closed
            RR13 = 13,  // No CNA connected, the preselected coupling mode cannot be realised
            RR14 = 14,  // Automatic limitation of transmitted data
            RR15 = 15,  // Checksum error
            RR17 = 17,  // Overvolatge / Overtemperature
            RR20 = 20,  // Not correctable limitation error
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Back Response
            @param backResponse The type to get the string representation from
            @return The string representation */
        static QString toString(Enum backResponse);

        /** @brief Return the error signification
            @param Enum The backReponse received
            @return The string signification of the error */
        static QString reponseToString(Enum backResponse);

        /** @brief Allows to register the meta type of the BackResponseCategory::Enum */
        static void registerMetaType();

    private:
        //Info Response
        static const constexpr char *RR_00 = "The Test procedure was stopped correctly";
        static const constexpr char *RR_01 = "One single pulse was triggered";
        static const constexpr char *RR_02 = "The simulator is ready to be discharged";
        static const constexpr char *RR_07 = "Continue execution after Fail 2 (RR 06)";
        static const constexpr char *RR_13 = "No CNA connected, the preselected coupling mode "
                                             "cannot be realised";
        static const constexpr char *RR_14 = "Automatic limitation of transmitted data";

        //Back Response
        static const constexpr char *RR_05 = "Fail 1";
        static const constexpr char *RR_06 = "Fail 2";
        static const constexpr char *RR_10 = "Data transmission error, too less or too much "
                                             "parameters";
        static const constexpr char *RR_11 = "Start not possible: Test On key not pushed or safety "
                                             "circuit not closed";
        static const constexpr char *RR_15 = "Checksum error";
        static const constexpr char *RR_17 = "Overvolatge / Overtemperature";
        static const constexpr char *RR_20 = "Not correctable limitation error";
        static const constexpr char *Unknown_Msg = "Unknown BackMessage !!";
};
