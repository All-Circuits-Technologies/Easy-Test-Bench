// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

class HmiDisplayFileBrowserView;
class ICommonInterface;
class QDir;


/** @brief Abstract module to browse file or folder */
class AFileBrowserModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent class instance */
        explicit AFileBrowserModule(const QString &moduleClassName,
                                    QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AFileBrowserModule() override = default;

    public:
        /** @see AUpdateHexFile::initParamsFormat
            @note @ref selectDirParamName : boolean (True if you want to select a directory,
                                                     instead of a file)
            @note @ref fileExtensionsParamName : string (The file extensions filter, the format has
                                                         to be like this: Text (*.txt))
            @note @ref fileDialogDirectoryParamName : string (If given, the file dialog will open
                                                              on this directory (if no previous
                                                              value has been given)) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AUpdateHexFile::initOutputsFormat
            @note @ref outputFilePathSelectedName : string (The file path selected) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief Get common interface */
        ICommonInterface &getCommonItf() const { return _commonItf; }

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Get the default value for the element path
            @param jsonObjectInputValues The input module values
            @param defaultValue The default value got
            @return True if no problem occured */
        virtual bool getDefaultValue(const JsonArray &jsonObjectInputValues,
                                     QString &defaultValue) = 0;

        /** @brief Save the element path chosen
            @param value The path chosen
            @return True if no problem occured */
        virtual bool saveChosenValue(const QString &value) = 0;

    private slots:
        /** @brief Call when the process is ended, either a problem occured, or we successfuly got
                   the file path
            @param success True if the sequence has successfuly ended */
        void onProcessEnded(bool success);

        /** @brief Called when the stop has been asked by user */
        void onStopAsked();

    private:
        /** @brief Manage the displaying of the file browser
            @note The method manages the displaying of the browser file view, when the view is no
                  more displayed, the @ref manageProcessEnd is called
            @param jsonObjectInputValues The module input values to parse */
        bool processDisplay(const JsonArray &jsonObjectInputValues);

        /** @brief Manage connections with the @ref HmiDisplayFileBrowserView instance
            @param hmiDisplay The hmi display to manage the connection with
            @param toConnect True to connect signals and slots, False to disconnect */
        void manageHmiConnection(HmiDisplayFileBrowserView &hmiDisplay, bool toConnect);

        /** @brief Manage the process end, the hmi display return value, the saving in database,
                   etc...
            @param success Represents the value returned by the hmi display (if the displaying has
                           succeeded or not)
            @param filePath The file path got from hmi display
            @return True if no problem occurred */
        bool manageProcessEnd(bool success, QString &filePath);

    private:
        static const constexpr char *SelectDirParamName = "selectDir";
        static const constexpr char *SelectDirParamDesc = "True if you want to select a directory, "
                                                          "instead of a file";

        static const constexpr char *FileExtensionsParamName = "fileExtensions";
        static const constexpr char *FileExtensionsParamDesc = "The file extensions filter, the "
                                                               "format has to be like this: "
                                                               "Text (*.txt)";

        static const constexpr char *FileDialogDirectoryParamName = "fileDialogDirectory";
        static const constexpr char *FileDialogDirectoryParamDesc = "If given, the file dialog "
                                                                    "will open on this directory "
                                                                    "(if no previous value has "
                                                                    "been given)";

        static const constexpr char *OutputFilePathSelectedName = "filePathSelected";
        static const constexpr char *OutputFilePathSelectedDesc = "The file path selected";

        static const constexpr char *FileExtensionsRegExp =
                                                R"(.* \(((\s?(\*(\.[a-zA-Z0-9]+)+)\s?)+|\*)\))";

        static const constexpr char *HmiDisplayFileName = "Browse file";
        static const constexpr char *HmiDisplayFileDesc = QT_TR_NOOP("(TR) Display a browser file "
                                                                     "to select a file");
        static const constexpr char *HmiDisplayDirName = "Browse directory";
        static const constexpr char *HmiDisplayDirDesc = QT_TR_NOOP("(TR) Display a browser folder "
                                                                    "to select a directory");

        static const QRegExp ValidateFileExtensions;

    private:
        bool _selectDirectory{false};
        QDir *_fileDialogDirectory{nullptr};
        QVector<QString> _filesExtensions;
        HmiDisplayFileBrowserView *_hmiDisplay{nullptr};
        ICommonInterface &_commonItf;
};
