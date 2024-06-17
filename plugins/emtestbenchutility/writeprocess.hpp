// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QSharedPointer>

class ACmd;
class AVisacom;
class AResponse;
class BackResponse;


/** @brief This class is used to manage the communication process with EM_Test instruments*/
class WriteProcess : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param interfaceId The communication id used to communicate with a specific instrument
            @param timeout the timeout parameter, if not set uses the default timeout of the lib
            @param parent The parent instance class */
        explicit WriteProcess(const QString &interfaceId,
                              quint16 timeout,
                              QObject *parent = nullptr);

        /** @brief Format and send the command to the instrument using visa
            @param cmd The message to send to the instrument
            @return return a boolean value. if false => An error occured */
        bool write(ACmd *cmd);

        /** @brief Format and send the command to the instrument and wait for a response
            @note The waiting time depends on the timeout value (tmo = -1 <=> infinite tmo)
            @param cmd The message to send to the instrument
            @return return a boolean value. if false => An error occured */
        bool query(ACmd *cmd);

        /** @brief Parse the response to know if reponse is IdResponse or BackResponse
            @note The waiting time depends on the timeout value (tmo = -1 <=> infinite tmo)
            @param cmd The message to send to the instrument
            @return return a boolean value. if false => An error occured */
        void manageResponse(const QString &response);

    signals:
        /** @brief Emitted when a new message is received from instrument
            @param aresponse The message coming from instrument */
        void responseReceived(const QSharedPointer<AResponse> &aresponse);

        /** @brief Emitted when an error occured during communication */
        void errorOccured();

    private slots:
        /** @brief Call when a response is coming from qtvisacomlib & Manage the response */
        void onResponse(const QByteArray &response);

    private:
        QSharedPointer<AVisacom> _visaCom;
        QString _response;
};
