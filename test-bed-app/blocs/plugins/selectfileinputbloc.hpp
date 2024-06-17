// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "aplugininputbloc.hpp"

#include "models/plugins/selectfileinputmodel.hpp"

#include <QUrl>


/** @brief Backend class linked with select file input view */
class SelectFileInputBloc : public APluginInputBloc
{
        Q_OBJECT
        Q_PROPERTY(SelectFileInputModel *castedModel
                   READ getSelectFileInputModel
                   NOTIFY castedModelChanged)
        Q_PROPERTY(QUrl folderPath READ getFolderPath NOTIFY folderPathUpdated)
        Q_PROPERTY(bool validBtnEnabled READ isValidBtnEnabled NOTIFY validBtnEnabledChanged)
        Q_PROPERTY(QString errorTxt READ getErrorTxt NOTIFY errorTxtChanged)

    public:
        /** @brief Class constructor
            @param parent An optional parent QObject */
        SelectFileInputBloc(QObject *parent = nullptr);

    public:
        /** @brief Get the input model */
        SelectFileInputModel *getSelectFileInputModel() const;

        /** @brief Say if the valid button is enabled */
        bool isValidBtnEnabled() const { return _validBtnEnabled; }

        /** @brief Get the error text, if an error occurred */
        const QString &getErrorTxt() const { return _errorTxt; }

        /** @brief Get the folder path */
        const QUrl &getFolderPath() const { return _folderPath; }

        /** @brief Validate the file path and set the error label depending of the error discovered
            @param value The value entered in the file path field */
        Q_INVOKABLE void onValidateButtonCLicked(const QString &value);

    public slots:
        /** @brief Called when a file is selected from the file explorer
            @param fileUrl The file path of the selected file */
        void onFileSelected(const QUrl &fileUrl);

    private:
        /** @brief Validate the file path and set the error label depending of the error discovered
            @param value The file path value to validate
            @return True if no problem occurs */
        QString validateFilePath(const QUrl &value, const SelectFileInputModel &castedModel);

        /** @brief Manage the validation of the file path value and enable/disable the validate
                   button depending of the validation
            @note This method calls @ref validateFilePath
            @param value The file path value to validate
            @return True if no problem occurs */
        bool manageValidation(const QUrl &value);

        /** @brief Set the error text, if an error occurred */
        void setErrorTxt(const QString &errorTxt);

        /** @brief Set the valid button to enable or disable */
        void setValidBtnEnabled(bool enabled);

        /** @brief Set the folder path */
        void setFolderPath(const QUrl &folderPath);

    private slots:
        /** @brief Called when the model is updated */
        void onModelUpdated();

    signals:
        /** @brief emitted to display an error message when the selected file is invalid
            @param errorMessage The message to display */
        void selectedFileError(const QString &errorMessage);

        /** @brief emitted to update validate button enable state
            @param enabled The enable state */
        void enableValidateButton(bool enabled);

        /** @brief emitted to set the file path chosen on the string input
            @param filePath The filePath to set */
        void setFilePathText(const QString &filePath);

        /** @brief Emitted when the casted model has been changed
            @param castedModel The casted model updated */
        void castedModelChanged(SelectFileInputModel *castedModel);

        /** @brief Emitted when the valid button enable state has changed
            @param enabled True if a valid button has been enabled */
        void validBtnEnabledChanged(bool enabled);

        /** @brief Emitted when the error text has changed
            @param errorText The updated error text */
        void errorTxtChanged(const QString &errorTxt);

        /** @brief Emitted when the folder path has changed
            @param folderPath The updated folder path */
        void folderPathUpdated(const QUrl &folderPath);

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    private:
        static const constexpr char *anyFiles = QT_TR_NOOP("(TR) Any files (*)");

        static const constexpr char *noFileSelectedError = QT_TR_NOOP("(TR) No file selected");

        static const constexpr char *fileDoesntExistError = QT_TR_NOOP("(TR) The file doesn't "
                                                                       "exist");

        static const constexpr char *notADirError = QT_TR_NOOP("(TR) The element chosen isn't a "
                                                               "folder");

        static const constexpr char *notAFileError = QT_TR_NOOP("(TR) The element chosen isn't a "
                                                                "file");

    private:
        bool _validBtnEnabled{false};
        QString _errorTxt{""};
        QUrl _folderPath;
};
