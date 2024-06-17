// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"


class SimpleSequenceInstanceModule : public ISequenceInstanceModule
{
    Q_OBJECT

    public:
        explicit SimpleSequenceInstanceModule(QObject *parent = nullptr);

    public:
        virtual QSharedPointer<IBankInterface> getBankInterface() const override;

        virtual bool getModuleName(QString &uniqueName) const override;

        virtual bool getInstanceName(QString &instanceName) const override;

        virtual bool setSequenceToken(const QString &sequenceToken) override;

        virtual bool launchModule(const JsonArray &jsonObjectInputValues) override;

        virtual bool stopModule(StopAskedType::Enum stopAskedType) override;

        virtual bool manageModulePause(bool pauseModule) override;

        virtual bool getState(SequenceStateType::Enum &currentState) const override;

        virtual bool getModuleParametersFormat(JsonArray &parameters) const override;

        virtual bool setModuleParameters(const JsonArray &parameters, JsonArray &errors) override;

        virtual bool getModuleParameters(JsonArray &parameters) const override;

        virtual bool getInputVariablesFormat(JsonArray &inVariablesFormat) const override;

        virtual bool getOutputVariablesFormat(JsonArray &outVariablesFormat) const override;

    public:
        static QString moduleName;

    private:
        bool _running = false;
};
