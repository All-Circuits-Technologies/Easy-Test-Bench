// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief This class is used to manage command to send to EM_Test instruments*/
class ACmd : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param header The header of the command specifying the pulse to generate
            @param parent The parent instance class */
        explicit ACmd(const QString &header, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ACmd() override;

    public:
        /** @brief format the command to the specific Em_Test instruments format
            @note This function generates a cmd with header, parameters & checksum
            @param formatedCommand the formated command as qByteArray
            @return true = command formated correctly | false = command not formated*/
        virtual bool formatCommand(QByteArray &formatedCommand);

    protected:
        /** @brief Get the parameters define in the child class
            @note This function must be overriding by child class to define the parameters to get
            @return formated command as qByteArray */
        virtual bool getParameters(QVector<quint16> &params) = 0;

    private:
        static const constexpr char *comma = ",";
        static const constexpr char *semicolon = ";";
        static const constexpr char *linefeed = "\n";

    private:
        QString _header;
};
