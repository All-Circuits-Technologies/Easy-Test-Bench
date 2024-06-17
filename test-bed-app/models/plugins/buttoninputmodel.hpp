// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QColor>

#include "jsonutility/jsontypedefs.hpp"

#include "models/plugins/aplugininputmodel.hpp"

using namespace std::placeholders;


/** @brief Display button input model */
class ButtonInputModel : public APluginInputModel
{
    Q_OBJECT
    Q_PROPERTY(QString key READ getKey NOTIFY keyChanged)
    Q_PROPERTY(QString label READ getLabel NOTIFY labelChanged)
    Q_PROPERTY(QString keyShortcut READ getKeyShortcut NOTIFY keyShortcutChanged)
    Q_PROPERTY(QColor fontColor READ getFontColor NOTIFY fontColorChanged)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor NOTIFY backgroundColorChanged)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit ButtonInputModel(QObject *parent = nullptr);

        /** @brief Class constructor
            @param uiToken The Display UI Token linked to this plugin input model
            @param parent The parent widget */
        explicit ButtonInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                  QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ButtonInputModel() override;

    public:
        /** @brief Get the key linked to the button */
        const QString &getKey() const { return _key; }

        /** @brief Get the button label */
        const QString &getLabel() const { return _label; }

        /** @brief Get the key shorcut linked to the button */
        const QString &getKeyShortcut() const { return _keyShortcut; }

        /** @brief Get the font color of the button */
        const QColor &getFontColor() const { return _fontColor; }

        /** @brief Get the background color of the button */
        const QColor &getBackgroundColor() const { return _backgroundColor; }

        /** @brief Say if the button is checked by default */
        void setDefaultChecked(bool checked);

    protected:
        /** @brief Parse the JsonObject description, get the informations and set the values on
                   the object
            @param elemDescription The JsonObject description to parse in order to build the widget
            @param update True if the elemDescription given is for update the model or for creating
                          a new view
            @return True if no problem occurs */
        virtual bool parseAndSetAttributes(const JsonObject &elemDescription,
                                           bool update = false) override;

    signals:
        /** @brief Emitted when the button key is changed
            @param key The new button key */
        void keyChanged(const QString &key);

        /** @brief Emitted when the button label is changed
            @param label The button label*/
        void labelChanged(const QString &label);

        /** @brief Emitted when the key shortcut has changed
            @param keyShortcut The button key shortcut */
        void keyShortcutChanged(const QString &keyShortcut);

        /** @brief Emitted when the font color has changed
            @param fontColro The new font color */
        void fontColorChanged(const QColor &fontColor);

        /** @brief Emitted when the background color has changed
            @param backgroundColor The new background color */
        void backgroundColorChanged(const QColor &backgroundColor);

    private:
        /** @brief Set the button key
            @param key The button key */
        void setKey(const QString &key);

        /** @brief Set the button label
            @param label The button label */
        void setLabel(const QString &label);

        /** @brief Set the key shorcut
            @param keyShorcut The button key shortcut */
        void setKeyShortcut(const QString &keyShortcut);

        /** @brief Set the font color
            @param fontColor The button font color */
        void setFontColor(const QColor &fontColor);

        /** @brief Set the background color
            @param backgroundColor The button background color */
        void setBackgroundColor(const QColor &backgroundColor);

        /** @brief Help to get the color from an element description and apply with the given setter
            @note This method calls @ref manageColorParseAndSetImpl
            @param elemDescription The element description
            @param attributeName The name of the attribute where to find the color in the element
                                 description
            @param currentColor The current color linked to the value we try to get
            @param fn The setter to apply the parsed color
            @return True if no error occurred */
        template<typename FnClass>
        bool manageColorParseAndSet(const JsonObject &elemDescription,
                                    const QString &attributeName,
                                    const QColor &currentColor,
                                    void (FnClass::*fn)(const QColor &));

        /** @brief Help to get the color from an element description and apply with the given setter
            @note This method is called by @ref manageColorParseAndSet
            @param elemDescription The element description
            @param attributeName The name of the attribute where to find the color in the element
                                 description
            @param currentColor The current color linked to the value we try to get
            @param setter The setter to apply the parsed color
            @return True if no error occurred */
        bool manageColorParseAndSetImpl(const JsonObject &elemDescription,
                                        const QString &attributeName,
                                        const QColor &currentColor,
                                        const std::function<void (const QColor &)> &setter);

    public:
        /** @brief Register QML type linked to this model */
        static void registerQmlType();

    private:
        QString _key;
        QString _label;
        QString _keyShortcut;
        QColor _fontColor;
        QColor _backgroundColor;
};

template<typename FnClass>
bool ButtonInputModel::manageColorParseAndSet(const JsonObject &elemDescription,
                                              const QString &attributeName,
                                              const QColor &currentColor,
                                              void (FnClass::*fn)(const QColor &))
{
    static_assert(std::is_base_of<ButtonInputModel, FnClass>::value,
                  "ButtonInputModel must implement given method");
    auto setter = std::bind(fn, this, _1);
    return manageColorParseAndSetImpl(elemDescription, attributeName, currentColor, setter);
}
