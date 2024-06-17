// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "aplugininputbloc.hpp"

#include "models/plugins/radiolistbuttonsinputmodel.hpp"


/** @brief Backend class linked with radio list buttons input view */
class RadioListButtonsInputBloc : public APluginInputBloc
{
        Q_OBJECT
        Q_PROPERTY(RadioListButtonsInputModel *castedModel
                   READ getRadioListButtonsInputModel
                   NOTIFY castedModelChanged)

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        RadioListButtonsInputBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~RadioListButtonsInputBloc() override;

    public slots:
        /** @brief Called when a button is selected
            @param btnKey The key of the selected button */
        void onBtnSelected(const QString &btnKey);

    public:
        /** @brief Get the input model */
        RadioListButtonsInputModel *getRadioListButtonsInputModel() const;

    private slots:
        /** @brief Called when the model is updated */
        void onModelUpdated();

    signals:
        /** @brief Emitted when the model has been modified */
        void castedModelChanged(RadioListButtonsInputModel *castedModel);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();
};
