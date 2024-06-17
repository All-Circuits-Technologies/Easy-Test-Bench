// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module allowes to create and fire an elapsed timer */
class FireElapsedTimerModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FireElapsedTimerModule)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit FireElapsedTimerModule(QObject *parent = nullptr);
    protected:
        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputElapsedTimerName : string (The token of the elapsed timer created and
                                                        fire) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        static const constexpr char *outputElapsedTimerName = "elapsedTimerToken";
        static const constexpr char *outputElapsedTimerDesc = "Create and start an elapsed timer "
                                                              "token";
};
