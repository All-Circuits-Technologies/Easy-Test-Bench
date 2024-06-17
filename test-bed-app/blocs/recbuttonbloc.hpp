// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abasepagebloc.hpp"


/** @brief The bloc linked to the rec button */
class RecButtonBloc : public ABasePageBloc
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit RecButtonBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~RecButtonBloc() override;

    public slots:
        /** @brief Call to simulate a click on the button given
            @param button The button to generate a click in */
        void click(QObject* button);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();
};
