// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "hmidisplaysimpletestview.hpp"


/** @brief Display a simple infinite wait view, with infinite GIF, a description and an optional
           cancel button */
class HmiDisplayInfiniteWaitView : public HmiDisplaySimpleTestView
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param commonItf The common interface to use in order to ask the display of a GUI
            @param parent The parent class */
        explicit HmiDisplayInfiniteWaitView(ICommonInterface& commonItf, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~HmiDisplayInfiniteWaitView() override;

    public:
        /** @brief Display an infinite HMI
            @param name The name of the view
            @param description The description to display in the view
            @param displayCancelButton True to display a cancel button
            @return True if no problem occurred */
        bool displayInfiniteHmi(const QString &name,
                                const QString &description,
                                bool displayCancelButton = true);

    private:
        /** @brief Default infinite gif image */
        static const constexpr char *defaultImg = ":/infinitewaitanimation-2.gif";
};
