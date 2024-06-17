// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QtPlugin>

#include "bankcoreshared/usebylib/iplugin.hpp"


/** @brief Qt plugin entry-point */
class SerialPluginInterface : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPLUGIN_IID FILE PLUGIN_NAME ".json")
    Q_INTERFACES(IPlugin)

    public:
        /** @brief Get the serial-link plugin bank entry-point
            @return The singleton instance of @ref SerialBankInterface
            @note The singleton is actually created and returned.
                  Calling this method more than once will generate a warning telling that singleton
                  is already created (but the singleton will be returned as expected). */
        virtual QSharedPointer<IBankInterface> accessBankInterface(
                                                        ICommonInterface &commonInterface) override;
};
