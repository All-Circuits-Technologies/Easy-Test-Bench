// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Abstract bloc
    @note Blocs are controllers for QML views, they are created in the QML views as model and then
          used to access to the linked models.
          They manage the refresh of the models (and so of the views) */
class AbstractBloc : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The optional qObject parent */
        explicit AbstractBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AbstractBloc() override;

    signals:
        /** @brief Emit when bloc is initiliazed
            @note As the view is initialized before the bloc,
                  this signal allows to warn that the bloc information is available **/
        void blocInitialized();

    protected slots:
        /** @brief Called after the instance constuction (the call is added on the eventloop)
            @note This can contain all the elements/data to initialize in order to update the
                  view */
        virtual void initBloc() = 0;
};
