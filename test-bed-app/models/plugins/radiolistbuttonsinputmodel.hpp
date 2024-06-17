// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/plugins/aplugininputmodel.hpp"

#include "models/genericlistmodel.hpp"
#include "models/plugins/buttoninputmodel.hpp"

#include <QHash>
#include <QQuickItem>


/** @brief Model class linked with radio list buttons input views */
class RadioListButtonsInputModel : public APluginInputModel
{
    Q_OBJECT
    Q_PROPERTY(GenericListModel<ButtonInputModel> *buttons READ getButtonsList
                                                           NOTIFY buttonsListChanged)
    Q_PROPERTY(QVector<ButtonInputModel*> buttonsList READ getButtonsItemList
                                                      NOTIFY buttonsListChanged)

    public:
        /** @brief Class constructor
            @param parent Parent class */
        explicit RadioListButtonsInputModel(QObject *parent = nullptr);

        /** @brief Class constructor
            @param uiToken The Display UI Token linked to this plugin input model
            @param parent The parent widget */
        explicit RadioListButtonsInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                            QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~RadioListButtonsInputModel() override;

    public:
        /** @brief Getter to get the buttons list */
        GenericListModel<ButtonInputModel> *getButtonsList() const { return _buttons; }

        /** @brief Getter to get the buttons item list */
        QVector<ButtonInputModel*> getButtonsItemList() const { return _buttons->getItemList(); }

    signals:
        /** @brief Emitted when the buttons list is changed */
        void buttonsListChanged();

    protected:
        /** @brief Parse the JsonObject description, get the informations and set the values on
                   the object
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurred */
        virtual bool parseAndSetAttributes(const JsonObject &elemDescription,
                                           bool update = false) override;

    private:
        /** @brief Manage the buttons parsing
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param defaultKey The default key to call if the user makes no choice
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurred */
        bool manageButtonsParsing(const JsonObject &elemDescription,
                                  const QString & defaultKey,
                                  bool update);

    public:
        /** @brief Register QML type linked to this model */
        static void registerQmlType();

    private:
        GenericListModel<ButtonInputModel> *_buttons{nullptr};
};
