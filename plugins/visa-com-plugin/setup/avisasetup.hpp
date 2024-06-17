// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

class AVisacom;


/** @brief This module is helpful to setup a VISA communication with an instrument */
class AVisaSetup : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit AVisaSetup(const QString &moduleClassName,
                            QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AVisaSetup() override = default;

    protected:

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputInterfaceIdName : string (The unique id of the interface used for
                                                       communicating with the targeted
                                                       instrument) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @brief Create and setup the wanted interface
            @return The VISA com interface created */
        virtual QSharedPointer<AVisacom> createAndSetupItf() = 0;


    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        static const constexpr char *outputInterfaceIdName = "interfaceId";
        static const constexpr char *outputInterfaceIdDesc = "The unique id of the interface used "
                                                             "for communicating with the targeted "
                                                             "instrument";

    private:
        QSharedPointer<AVisacom> _visaCom;
};
