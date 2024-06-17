// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "emtestbenchutility/command/acmd.hpp"


/** @brief Command to request the device IDN  */
class IdnCmd : public ACmd
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent object */
        explicit IdnCmd(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~IdnCmd() override;

    public:
        /** @see ACmd::getParameters */
        virtual bool getParameters(QVector<quint16> &params) override;

    public:
        static const constexpr char *vdsIdnCmd  = "DC";
};
