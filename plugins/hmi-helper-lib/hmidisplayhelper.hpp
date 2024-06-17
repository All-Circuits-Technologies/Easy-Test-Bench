// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "hmidisplaystatus.hpp"
#include "jsonutility/jsontypedefs.hpp"

class ICommonInterface;


/** @brief Useful class to ask the display of an HMI
    @note The goal of the class is to make the HMI building easy */
class HmiDisplayHelper : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param commonItf The common interface to use in order to ask the display of a GUI
            @param parent The parent class */
        explicit HmiDisplayHelper(ICommonInterface& commonItf, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~HmiDisplayHelper() override;

    public slots:
        /** @brief Call to cancel the current displaying of the HMI
            @note If the HMI is not displayed, the method does nothing and returns true
            @note If the HMI is cancelled, the GUI result will be on error
            @return True if no problem occurred */
        bool cancelDisplaying();

    public:
        /** @brief Update HMI decription
            @note This only works if the HMI is already displaying
            @param description The description to update on the view
            @return True if no problem occurred */
        bool updateHmiDescription(const QString& description);

        /** @brief Update HMI decription and image
            @note This only works if the HMI is already displaying
            @param description The description to update on the view
            @param imagePath The path of the image
            @param tokenToGetInSequenceDir The sequence token which is used to find the sequence
                                           directory and search the image from it.
                                           If the parameter is empty, we consider that the previous
                                           image path is an absolute path and it's not relative to
                                           the sequence dir.
            @return True if no problem occurred */
        bool updateHmiDescAndImage(const QString &description,
                                   const QString &imagePath,
                                   const QString &tokenToGetInSequenceDir = QString());

        /** @brief Get the current HMI display status */
        HmiDisplayStatus::Enum getDisplayStatus() const { return _displayStatus; }

        /** @brief Get the result received from the HMI
            @attention This getter is only usefull if the current display status is equal to
                       HmiDisplayStatus::Success (because the result has been got from HMI) */
        const JsonArray &getResultReceived() const { return _resultReceived; }

    protected:
        /** @brief Access to the common interface linked to the main view  */
        ICommonInterface &accessCommonItf() { return _commonItf; }

        /** @brief This method is called in order to parsed the display results in derived class
            @param valuesSet The values got from the HMI displaying
            @return True if no problem occurred */
        virtual bool parseDisplayResult(const JsonArray &valuesSet);

        /** @brief Ask to display an HMI
            @param name The name of the view to display
            @param elemToDisplay The elements of the view to display
            @return True if no problem occurred */
        bool displayHmiImpl(const QString &name, const JsonObject &elemToDisplay);

        /** @brief Update an existing HMI
            @note There is no need to resend all the elements here, only the elements you want to
                  update
            @note Because the name of the view is used to find it, it can't be updated
            @param elemToUpdate The elements to update on the views
            @return True if no problem occurred */
        bool updateHmiImpl(const JsonObject &elemToUpdate);

        /** @brief Get the name of the displayed view */
        const QString &getName() const { return _name; }

        /** @brief Format the image informations and set it in the JsonObject given
            @note If the image path is empty, nothing is added and the method returns true
            @param imagePath The path of the image
            @param tokenToGetInSequenceDir The sequence token which is used to find the sequence
                                           directory and search the image from it.
                                           If the parameter is empty, we consider that the previous
                                           image path is an absolute path and it's not relative to
                                           the sequence dir.
            @param guiToDisplay The objects to display in view */
        bool formatImage(const QString &imagePath,
                         const QString &tokenToGetInSequenceDir,
                         JsonObject &guiToDisplay);

    private slots:
        /** @brief Called on GUI result
            @note The common interface emits a signal for each finished HMI. To know if the result
                  is the one expected, compare the unique uuidToken with the one given at HMI
                  displaying
            @param success True if the HMI ends in success
            @param uuidToken The HMI unique token
            @param valueSet The result of the HMI */
        void onGuiResult(bool success, const QString &uuidToken, const JsonArray &valueSet);

    signals:
        /** @brief Emitted when the HMI displaying has ended
            @note If the GUI finished in success, you can get the result with the method
                  @ref getResultReceived */
        void finished(bool success);

    private:
        /** @brief Set the display status and manage the finished signal
            @param newStatus The new display status */
        void setDisplayStatus(HmiDisplayStatus::Enum newStatus);

    private:
        ICommonInterface &_commonItf;
        QString _uuidToken;
        HmiDisplayStatus::Enum _displayStatus{HmiDisplayStatus::Init};
        JsonArray _resultReceived;
        QString _name;
};
