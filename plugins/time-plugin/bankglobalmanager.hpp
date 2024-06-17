// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultbankinterface.hpp"

class ElapsedTimerManager;


/** @brief Bank manager callable in all the bank */
class BankGlobalManager : public DefaultBankInterface
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param commonInterface The common interface given by the lib
            @param parent The class parent */
        explicit BankGlobalManager(ICommonInterface &commonInterface, QObject *parent = nullptr);

    public:
        /** @brief Create the singleton instance of the bank global manager
            @note This method as to bel called only once
            @note Need to be called before calling instance
            @param commonInterface The common interface given by the lib
            @return The Bank global manager instance */
        static const QSharedPointer<BankGlobalManager> &Create(ICommonInterface &commonInterface);

        /** @brief Return the singleton instance of the bank global manager
            @note The method @ref BankManager::Create has to have been called once before this one
            @return The Bank global manager instance */
        static const QSharedPointer<BankGlobalManager> &Instance();

    public:
        /** @brief Accesss to the elapsed timer manager */
        ElapsedTimerManager &accessElapsedTimerManager();

        /** @brief Get the elapsed timer manager */
        const ElapsedTimerManager &getElapsedTimerManager() const;

    private:
        /** @brief Create eand init the managers linked to the global manager */
        void initManagers();

    private:
        static QSharedPointer<BankGlobalManager> _instance;

    private:
        ElapsedTimerManager *_elapsedTimerManager{nullptr};
};
