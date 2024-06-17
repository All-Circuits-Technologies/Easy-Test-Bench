// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "hmidisplayhelper.hpp"

#include <QHash>

class QFileInfo;


/** @brief Display a simple view with description, an image and buttons */
class HmiDisplaySimpleTestView : public HmiDisplayHelper
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param commonItf The common interface to use in order to ask the display of a GUI
            @param parent The parent class */
        explicit HmiDisplaySimpleTestView(ICommonInterface& commonItf, QObject *parent = nullptr);

    public:
        /** @brief List of buttons which can be displayed in the view */
        enum ButtonEnum : quint8
        {
            NoButton        = 0x00, //!< @brief Display no button
            OkButton        = 0x01, //!< @brief Display a green success button
            NotOkButton     = 0x02, //!< @brief Display a red fail button
            CancelButton    = 0x04  //!< @brief Display a red fail button
        };

    public:
        /** @brief Display a simple test view
            @attention The @ref setButtonName method has to be called before this one
            @param name The name of the view
            @param description The description to display in the view
            @param buttons Flags which has to contain ButtonEnum and allow to choose the buttons to
                           display
            @param imagePath The path of the image to display, if empty no image will be searched
                             and so displayed
            @param tokenToGetInSequenceDir The sequence token which is used to find the sequence
                                           directory and search the image from it.
                                           If the parameter is empty, we consider that the previous
                                           image path is an absolute path and it's not relative to
                                           the sequence dir.
            @param defaultButton The default button to choose if no choice is made by user
            @return True if no problem occurred */
        bool displayHmi(const QString &name,
                        const QString &description,
                        quint8 buttons = OkButton | NotOkButton,
                        const QString &imagePath = QString(),
                        const QString &tokenToGetInSequenceDir = QString(),
                        ButtonEnum defaultButton = CancelButton);

        /** @brief Update a simple test view currently displaying
            @attention The @ref setButtonName method has to be called before this one
            @param description The description to display in the view
            @param buttons Flags which has to contain ButtonEnum and allow to choose the buttons to
                           display
            @param imagePath The path of the image to display, if empty no image will be searched
                             and so displayed
            @param tokenToGetInSequenceDir The sequence token which is used to find the sequence
                                           directory and search the image from it.
                                           If the parameter is empty, we consider that the previous
                                           image path is an absolute path and it's not relative to
                                           the sequence dir.
            @param defaultButton The default button to choose if no choice is made by user
            @return True if no problem occurred */
        bool updateHmi(const QString &description = QString(),
                       quint8 buttons = OkButton | NotOkButton,
                       const QString &imagePath = QString(),
                       const QString &tokenToGetInSequenceDir = QString(),
                       ButtonEnum defaultButton = CancelButton);

        /** @brief Change the button label name displayed
            @attention To work, this method has to be called before the
                       @ref formatAndsetElemToDisplay method
            @param button The button to change its name
            @param labelName The new button label name to display
            @param keyShortCut The new button shorcut */
        void setButtonName(ButtonEnum button, const QString &labelName, char keyShortCut);

        /** @brief Get the button clicked
            @return The button clicked by user, if the user hasn't clicked any button, this method
                    returns @ref ButtonEnum::NoButton */
        ButtonEnum getButtonClicked() const { return _buttonClicked; }

    protected:
        /** @brief Set the buttons managed by the class
            @note This method won't change the element to display. This info is only used to parse
                  results */
        void setButtons(quint8 buttons) { _buttons = buttons; }

        /** @brief formatButtons
            @param buttons Flags which has to contain ButtonEnum and allow to choose the buttons to
                           display
            @param defaultButton The default button to choose if no choice is made by user
            @param guiToDisplay The objects to display in view
            @return True if no problem occurred */
        bool formatButtons(quint8 buttons, ButtonEnum defaultButton, JsonObject &guiToDisplay);

    protected:
        /** @brief Parse the display result and say if the HMI ended in succes or not
            @param valuesSet The values got from HMI
            @return True if the HMI ended in success */
        virtual bool parseDisplayResult(const JsonArray &valuesSet) override;

    protected:
        static const constexpr char* okButtonKey = "okKey";
        static const constexpr char* okButtonLabel = QT_TR_NOOP("(TR) Ok");
        static const constexpr char  okKeyShorcut = 'o';
        static const constexpr char* okFontColor = "#FFFFFF";
        static const constexpr char* okBackgroundColor = "#43A047";

        static const constexpr char* notOkButtonKey = "notOkKey";
        static const constexpr char* notOkButtonLabel = QT_TR_NOOP("(TR) Not ok");
        static const constexpr char  notOkKeyShorcut = 'n';
        static const constexpr char* notOkFontColor = "#FFFFFF";
        static const constexpr char* notOkBackgroundColor = "#E53935";

        static const constexpr char* cancelButtonKey = "cancelKey";
        static const constexpr char* cancelButtonLabel = QT_TR_NOOP("(TR) Cancel");
        static const constexpr char  cancelKeyShorcut = 'c';
        static const constexpr char* cancelFontColor = "#FFFFFF";
        static const constexpr char* cancelBackgroundColor = "#E53935";

    private:
        /** @brief Contains the button info */
        struct ButtonInfo
        {
            QString label;
            char shortcut;
        };

    private:
        ButtonEnum _buttonClicked{NoButton};
        quint8 _buttons{0};

        QHash<ButtonEnum, ButtonInfo> _buttonsInfoOverriden;

        QString _description;
};
