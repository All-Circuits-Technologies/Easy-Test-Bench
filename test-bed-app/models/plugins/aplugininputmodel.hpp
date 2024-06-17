// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/types/typemanaged.hpp"

#include "test-bed-lib/bank/displayuitoken.hpp"

#include "types/imagetype.hpp"
#include "types/pluginwidgettype.hpp"

class QTimer;
class TicHandler;


/** @brief Base class for plugin input models */
class APluginInputModel : public QObject
{
        Q_OBJECT

        Q_PROPERTY(PluginWidgetType::Enum widgetType READ getWidgetType CONSTANT)
        Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
        Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
        Q_PROPERTY(QString imagePath READ getImagePath NOTIFY imagePathChanged)
        Q_PROPERTY(ImageType::Enum imageType READ getImageType NOTIFY imageTypeChanged)
        Q_PROPERTY(QVariant valueSet READ getValueSet WRITE setValueSet NOTIFY valueSetChanged)
        Q_PROPERTY(QVariant defaultValue READ getDefaultValue NOTIFY defaultValueChanged)
        Q_PROPERTY(bool timerEnabled READ isTimerEnabled NOTIFY timerEnabledChanged)
        Q_PROPERTY(bool required READ isRequired NOTIFY requiredChanged)
        Q_PROPERTY(bool hide READ isHide NOTIFY hideChanged)

        Q_PROPERTY(bool displayed READ isDisplayed WRITE setDisplayed NOTIFY displayedChanged);
        Q_PROPERTY(int timeoutLeftInS READ getTimeoutLeft NOTIFY timeoutLeftChanged);

    private:
        /** @brief The plugin input state */
        enum PluginInputState {
            Created,
            Init,
            Displayed,
            Closed
        };

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        APluginInputModel(QObject *parent = nullptr);

        /** @brief Class constructor
            @param typeManaged The type of the value to manage its input
            @param uiToken The Display UI Token linked to this plugin input model
            @param parent An optional parent QObject */
        APluginInputModel(PluginWidgetType::Enum widgetType,
                          TypeManaged::Enum typeManaged,
                          const QSharedPointer<DisplayUiToken> &uiToken,
                          QObject *parent = nullptr);

        /** @brief Copy constructor
            @param copyModel The model to copy */
        APluginInputModel(const APluginInputModel &copyModel);

        /** @brief Class destructor */
        virtual ~APluginInputModel() override;

    public:
        /** @brief Get input name */
        const QString &getName() const { return _name; }

        /** @brief Get input description */
        const QString &getDescription() const { return _description; }

        /** @brief Get input value set */
        const QVariant &getValueSet() const { return _valueSet; }

        /** @brief Set input value set */
        void setValueSet(const QVariant &valueSet);

        /** @brief Get input value type */
        TypeManaged::Enum getValueType() const { return _valueType; }

        /** @brief Get widget type */
        PluginWidgetType::Enum getWidgetType() const { return _widgetType; }

        /** @brief Get input default value */
        const QVariant &getDefaultValue() const { return _defaultValue; }

        /** @brief Get input time enabled state */
        bool isTimerEnabled() const { return _timerEnabled; }

        /** @brief Get input image path */
        const QString &getImagePath() const { return _imagePath; }

        /** @brief Get input image type */
        ImageType::Enum getImageType() const { return _imageType; }

        /** @brief Get input global timer*/
        QTimer *getGlobalTimer() { return _globalTimer; }

        /** @brief Returns true if the model is required */
        bool isRequired() const { return _required; }

        /** @brief Returns true if the model is hidden */
        bool isHide() const { return _hide; }

        /** @brief Say if the input model is displayed */
        void setDisplayed(bool displayed);

        /** @brief Returns true if the input model is displayed */
        bool isDisplayed() const;

        /** @brief Get the left time of the timeout */
        int getTimeoutLeft() const;

        /** @brief Get the token linked to the UI */
        const QSharedPointer<DisplayUiToken> &getUiToken() const { return _uiToken; }

        /** @brief Called when the sequence is in pause or to resume it */
        void onPauseOrResume(bool resume);

    public:
        /** @brief Init the model with the object given
            @param elemDescription The elements to initialize the model
            @return True if no problem occurred */
        bool initModel(const JsonObject &elemDescription);

        /** @brief Update the model with the object given
            @param elemDescription The elements to update the model
            @return True if no problem occurred */
        bool updateModel(const JsonObject &elemDescription);

    public slots:
        /** @brief Call to finish the view and return the value to the plugins */
        void finishView();

    private slots:
        /** @brief Called when the view is finished or cancelled */
        void onFinishedOrCancelled();

        /** @brief Called when the view is displayed
            @param displayed True when the view is displayed */
        void onViewDisplayed(bool displayed);

        /** @brief Called when a tic is triggered */
        void onTicTriggered();

        /** @brief Called when the global timeout has raised */
        void onGlobalTimeout();

    signals:
        /** @brief Emitted when imagePath changed*/
        void imagePathChanged(const QString &imagePath);

        /** @brief Emitted when imageType changed*/
        void imageTypeChanged(ImageType::Enum imageType);

        /** @brief Emitted when description changed*/
        void descriptionChanged(const QString &description);

        /** @brief Emitted when name changed*/
        void nameChanged(const QString &name);

        /** @brief Emitted when valueSet changed*/
        void valueSetChanged(const QVariant &valueSet);

        /** @brief Emitted when timerEnbaled changed*/
        void timerEnabledChanged(bool enabled);

        /** @brief Emitted when the required value has changed */
        void requiredChanged(bool required);

        /** @brief Emitted when the hide value has changed */
        void hideChanged(bool hide);

        /** @brief Emitted when the displayed value has changed */
        void displayedChanged(bool displayedChanged);

        /** @brief Emitted when the default value has changed */
        void defaultValueChanged(const QVariant &defaultValue);

        /** @brief Emitted when the timeout left value has changed */
        void timeoutLeftChanged(int timeoutLeft);

        /** @brief Emitted to close item */
        void closeItem();

    protected:
        /** @brief Parse the JsonObject description, get the informations and set the values on
                   the object
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurs */
        virtual bool parseAndSetAttributes(const JsonObject &elemDescription, bool update = false);

        /** @brief Set input default value*/
        void setDefaultValue(const QVariant &defaultValue);

    private:
        /** @brief Set input image path */
        void setImagePath(const QString &imagePath);

        /** @brief Set input image type */
        void setImageType(ImageType::Enum imageType);

        /** @brief Set input description */
        void setDescription(const QString &description);

        /** @brief Set input name */
        void setName(const QString &name);

        /** @brief Set input timer enabled state */
        void setTimerEnabled(bool timerEnabled);

        /** @brief Set the element as required */
        void setRequired(bool required);

        /** @brief Set the element as hide */
        void setHide(bool hide);

        /** @brief Manage the global timer
            @param start Start or stop the global timer*/
        void manageGlobalTimer(bool start);

    public:
        /** @brief Register QML type linked to this model */
        static void registerBaseQmlType();

        /** @brief Register Meta type linked to this model */
        static void registerMetaType();

    private:
        const PluginWidgetType::Enum _widgetType;
        const TypeManaged::Enum _valueType;

        QString _imagePath;
        ImageType::Enum _imageType;
        QString _name;
        QString _description;
        bool _hide{false};
        bool _required{false};

        QVariant _defaultValue;
        QVariant _valueSet;

        bool _timerEnabled{false};
        bool _timerStarted{false};
        QTimer *_globalTimer{nullptr};
        const TicHandler *_ticHandler{nullptr};

        PluginInputState _itemState{PluginInputState::Created};
        QSharedPointer<DisplayUiToken> _uiToken;
};
