// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This class is used to manage BackResponse coming from EM_Test instruments*/
class AResponse : public QObject
{
    Q_OBJECT

    public:
        /** @brief Enum used to define the type of the response */
        enum ResponseType:quint16
        {
            IdResponse = 1,
            BackResponse = 2,
            Unknown = std::numeric_limits<quint16>::max()
        };

        Q_ENUM(ResponseType)

    public:
        /** @brief Class constructor
            @param response The response from instruments
            @param parent The parent instance class */
        explicit AResponse(const QString &response, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AResponse() override;

    public:
        /** @brief format the command to the specific Em_Test instruments format
            @note This function generates a cmd with header, parameters & checksum
            @param formatedCommand the formated command as qByteArray
            @return true = command formated correctly | false = command not formated*/
        virtual bool parseResponse(const QString &response) = 0;

    public:
        /** @brief Return the response type
            @note ResponseType can be IdResponse | InfoResponse | ErrorResponse | Unknown
            @return The response type */
        static ResponseType getResponseType(const QString &response);

    private:
        static const constexpr char *Back_Response = "RR";
        static const constexpr char *VDS200 = "VDS200";
        static const constexpr char *MPG200 = "MPG200";
        static const constexpr char *LD200  = "LD200";

    public:
        ResponseType responseType = {Unknown};
};
