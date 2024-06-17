// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "usebyplugin/icommoninterface.hpp"


class FakeCommonInterface  : public ICommonInterface
{
    Q_OBJECT

    public:
        FakeCommonInterface(QObject * parent = nullptr);

        virtual ~FakeCommonInterface() override;

        virtual bool getRoutesLinkedToTopologyModule(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    JsonArray &pinsLinks) const override;
        virtual bool activateRoute(const JsonObject &pinsLink) override;

        virtual bool displayParameterInput(const JsonObject &elemToDisplay,
                                           QString &uuidToken) override;

        virtual bool updateDisplay(const QString &uuidToken,
                                   const JsonObject &elemToUpdate) override;
        virtual bool cancelDisplay(const QString &uuidToken) override;

        virtual bool readSetting(const QString &settingStrKey, QVariant &value) const override;

        virtual const ISequenceLogManager *getSequenceLogManager(
                                                const QString &sequenceUniqueToken) const override;

        virtual bool getSequenceDirectory(const QString &seqToken, QDir &seqDir) const override;

        virtual bool getPermStoreValue(const QString &storeKey, QString &storeValue) const override;

        virtual bool setPermStoreValue(const QString &storeKey, const QString &storeValue) override;

        virtual bool getProjectCacheValue(const QString &seqToken,
                                          const QString &cacheKey,
                                          QVariant &cacheValue) const override;

        virtual bool setProjectCacheValue(const QString &seqToken,
                                          const QString &cacheKey,
                                          const QVariant &cacheValue) override;
};
