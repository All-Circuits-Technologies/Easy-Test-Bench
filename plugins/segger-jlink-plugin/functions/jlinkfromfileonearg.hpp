// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkintffromfile.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief The module replaces one argument and its value in a targetted JLinkFile. */
class JLinkFromFileOneArg : public AJLinkIntfFromFile
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(JLinkFromFileOneArg)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit JLinkFromFileOneArg(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~JLinkFromFileOneArg() override;
};
