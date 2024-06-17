// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "hmidisplayhelper.hpp"

class QDir;


/** @brief Helpful class to display a file browser */
class HmiDisplayFileBrowserView : public HmiDisplayHelper
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param commonItf The common interface to use in order to ask the display of a GUI
            @param parent The parent class */
        explicit HmiDisplayFileBrowserView(ICommonInterface& commonItf, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~HmiDisplayFileBrowserView() override;

    public:
        /** @brief Format and set the element to display
            @note To call before @ref HmiDisplayHelper::displayHmi
            @param sequenceToken The sequence token linked to the current module sequence
            @param name The name of the view
            @param description The description to display in the view
            @param filesExtensions The files extensions managed by the dialog view
            @param selectDir True to select a directory, false to select a file
            @param defaultPathValue The default path to set as default value in the view
            @param currentDirectory The current directory to display the file browser from
            @return True if no problem occurs */
        bool displayHmi(const QString &sequenceToken,
                        const QString &name,
                        const QString &description,
                        const QVector<QString> &filesExtensions = QVector<QString>(),
                        bool selectDir = false,
                        const QString &defaultPathValue = QString(),
                        const QDir *currentDirectory = nullptr);

    protected:
        /** @see HmiDisplayHelper::parseDisplayResult
            @note Verify if the element returned exists and if a dir was asked, it's a dir and if
                  a file was asked, it's a file */
        virtual bool parseDisplayResult(const JsonArray &valuesSet) override;

    private:
        bool _selectDir{false};
};
