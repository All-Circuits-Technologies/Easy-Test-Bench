// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "test-bed-lib/bank/adisplayuserinterface.hpp"

#include "models/plugins/aplugininputmodel.hpp"


/** @brie interface between the plugins and the HMI */
class AppInterfaceManager : public ADisplayUserInterface
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent object */
        explicit AppInterfaceManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AppInterfaceManager() override;

    public:
        /** @brief Get input models */
        const QVector<APluginInputModel *> &getInputModels() const { return _pluginsData; }

    protected slots:
        /** @see ADisplayUserInterface::displayWidgetImpl */
        virtual void displayWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                       const JsonObject &elemToDisplay) override;

        /** @see ADisplayUserInterface::updateWidgetImpl */
        virtual void updateWidgetImpl(const QSharedPointer<DisplayUiToken> &uiToken,
                                      const JsonObject &elemToUpdate) override;

    private slots:
        /** @brief Called when the pungin input model is removed */
        void onPluginInputModelRemoved();

    signals:
        /** @brief Emitted when a plugin input need to be displayed */
        void displayPluginView(APluginInputModel *inputModel);

    private:
        /** @brief Create an input model and add it to the view
            @param uiToken The uiToken to create the view from
            @param elemToDisplay The element to use in order to create the view */
        bool createInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                              const JsonObject &elemToDisplay);

    private:
        QVector<APluginInputModel *> _pluginsData;
};
