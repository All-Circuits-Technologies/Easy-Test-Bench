// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "acmd.hpp"


/** @brief This class is used to manage start pulse command*/
class StartCmd : public ACmd
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit StartCmd(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~StartCmd() override;

    public:
        /** @see ACmd::getParameters */
        virtual bool getParameters(QVector<quint16> &params) override;

    private:
        static const constexpr char *startCmd = "AA";
};
