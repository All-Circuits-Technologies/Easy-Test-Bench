// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "acmd.hpp"


/** @brief This class is used to manage terminate communication command*/
class TerminateCmd : public ACmd
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit TerminateCmd(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~TerminateCmd() override;

    public:
        /** @see ACmd::getParameters */
        virtual bool getParameters(QVector<quint16> &params) override;

    private:
        static const constexpr char *terminateCmd = "AR";
};
