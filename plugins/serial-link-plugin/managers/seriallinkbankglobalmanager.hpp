// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultbankinterface.hpp"

class AutoAnswerSerialManager;


/** @brief This is the global manager of the serial link module */
class SerialLinkBankGlobalManager : public DefaultBankInterface
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param commonInterface The common interface
            @param parent The parent instance */
        explicit SerialLinkBankGlobalManager(ICommonInterface &commonInterface,
                                             QObject *parent = nullptr);

    public:
        /** @brief The class destructor */
        virtual ~SerialLinkBankGlobalManager() override;

    public:
        /** @brief Initialize the manager */
        void initManager();

        /** @brief Access the auto answer manager
            @return The auto answer manager */
        AutoAnswerSerialManager &accessAutoAnswerManager() const { return *_autoAnswerManager; }

        /** @brief Get the auto answer manager
            @return The auto answer manager */
        const AutoAnswerSerialManager &getAutoAnswerManager() const { return *_autoAnswerManager; }

    public:
        /** @brief Create the singleton instance of the bank global manager
            @note This method as to be called only once
            @note Need to be called before calling instance
            @param commonInterface The common interface given by the lib
            @return The Bank global manager instance */
        static const QSharedPointer<SerialLinkBankGlobalManager> &Create(
            ICommonInterface &commonInterface);

        /** @brief Return the singleton instance of the bank global manager
            @note The method @ref SerialLinkBankGlobalManager::Create has to have been called once
                  before this one
            @return The Bank global manager instance */
        static const QSharedPointer<SerialLinkBankGlobalManager> &Instance();

    private:
        static QSharedPointer<SerialLinkBankGlobalManager> _instance;

    private:
        AutoAnswerSerialManager *_autoAnswerManager{nullptr};
};
