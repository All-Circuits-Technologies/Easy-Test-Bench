// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultbankinterface.hpp"

#include <QHash>

class AutoAnswerPeakCanManager;
class PCanIntfSetupHandle;


/** @brief This Bank global manager to use in this plugin
    @note It also contains the handle registered by setup module, to access common information */
class PeakCanBankGlobalManager : public DefaultBankInterface
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param commonInterface The common interface of the caller lib
            @param parent The parent instance class */
        explicit PeakCanBankGlobalManager(ICommonInterface &commonInterface,
                                          QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~PeakCanBankGlobalManager() override;

    public:
        /** @brief Initialize the manager */
        void initManager();

        /** @brief Access the auto answer manager
            @return The auto answer manager */
        AutoAnswerPeakCanManager &accessAutoAnswerManager() const { return *_autoAnswerManager; }

        /** @brief Get the auto answer manager
            @return The auto answer manager */
        const AutoAnswerPeakCanManager &getAutoAnswerManager() const { return *_autoAnswerManager; }

    public:
        /** @brief Create the singleton instance of the bank global manager
            @note This method as to be called only once
            @note Need to be called before calling instance
            @param commonInterface The common interface given by the lib
            @return The Bank global manager instance */
        static const QSharedPointer<PeakCanBankGlobalManager> &Create(
                                                                ICommonInterface &commonInterface);

        /** @brief Return the singleton instance of the bank global manager
            @note The method @ref BankManager::Create has to have been called once before this one
            @return The Bank global manager instance */
        static const QSharedPointer<PeakCanBankGlobalManager> &Instance();

    private:
        static QSharedPointer<PeakCanBankGlobalManager> _instance;

    private:
        AutoAnswerPeakCanManager *_autoAnswerManager{nullptr};
};
