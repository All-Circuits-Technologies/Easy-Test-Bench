// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultbankinterface.hpp"


/** @brief Default global bank manager callable in all the bank
    @note This Global manager can be used when there is no need to add a new manager and you just
          want an usable simple bank interface with singleton. */
class DefaultBankGlobalManager : public DefaultBankInterface
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param commonInterface The common interface given by the lib
            @param parent The class parent */
        explicit DefaultBankGlobalManager(ICommonInterface &commonInterface,
                                          QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~DefaultBankGlobalManager() override = default;

    public:
        /** @brief Create the singleton instance of the bank manager
            @note This method as to be called only once
            @note Need to be called before calling instance
            @param commonInterface The common interface given by the lib
            @return The Bank manager instance */
        static const QSharedPointer<DefaultBankGlobalManager> &Create(
                                                                ICommonInterface &commonInterface);

        /** @brief Return the singleton instance of the bank manager
            @note The method @ref DefaultBankGlobalManager::Create has to have been called once
                  before this one
            @return The Bank manager instance */
        static const QSharedPointer<DefaultBankGlobalManager> &Instance();

    private:
        static QSharedPointer<DefaultBankGlobalManager> _instance;
};
