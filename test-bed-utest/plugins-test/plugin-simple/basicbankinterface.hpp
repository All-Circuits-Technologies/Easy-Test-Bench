// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "bankcoreshared/usebylib/ibankinterface.hpp"


class BasicBankInterface : public IBankInterface
{
    Q_OBJECT

    private:
        explicit BasicBankInterface(ICommonInterface &commonInterface, QObject *parent = nullptr);

    public:
        virtual bool getUniqueName(QString &name) const override;

        virtual bool getVersion(QString &version) const override;

        virtual bool getTopologyModulesName(QVector<QString> &modulesName) const override;

        virtual bool getSequenceModulesName(QVector<QString> &modulesName) const override;

        virtual ITopologyInstanceModule *createTopologyInstanceModule(
                                                                const QString &moduleName) override;

        virtual ISequenceInstanceModule *createSequenceInstanceModule(
                                                            const QString &moduleName,
                                                            const QString &sequenceToken) override;

    public:
        static const QSharedPointer<BasicBankInterface> &Create(ICommonInterface &commonInterface);

        static const QSharedPointer<BasicBankInterface> &Instance();

    private:
        static QSharedPointer<BasicBankInterface> _instance;

    private:
        ICommonInterface &_commonInterface;
};
