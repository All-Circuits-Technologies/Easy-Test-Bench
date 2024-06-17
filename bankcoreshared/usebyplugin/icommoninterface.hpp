// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QDir>

#include "jsonutility/jsontypedefs.hpp"
#include "usebyplugin/isequencelogmanager.hpp"

/** @brief Interface for Bank classes to access methods from the main lib */
class ICommonInterface : public QObject
{
    Q_OBJECT

protected:
    /** @brief Class constructor
        @param parent Class parent */
    explicit ICommonInterface(QObject *parent = nullptr);

    /** @brief Class destructor */
    virtual ~ICommonInterface() override = default;

public:
    /** @brief Get all the routes linked to a given topology module. All the routes, where the
               module is:
                   - The target
                   - The targeting
                   - An intermediary
        @note The json array returned is as follow:
            [
                {
                    "fromPin": { "instance": "my-fakebox", "pin": "SW3" },
                    "toPin":   { "instance": "my-keithley", "pin": "Hi" },
                    "path": [ { "instance": "matrix1", "fromPin": "row7", "toPin": "col1" } ]
                }
            ]
        @param bankUniqueName The bank name where the topology module is
        @param uniqueTopologyModuleName The topology module name
        @param uniqueTopologyInstanceModuleName The unique name which identify the topology
                                                instance module
        @param pinsLinks The routes where the module is involved
        @return True If no problem occurs */
    virtual bool getRoutesLinkedToTopologyModule(
        const QString &bankUniqueName,
        const QString &uniqueTopologyModuleName,
        const QString &uniqueTopologyInstanceModuleName,
        JsonArray &pinsLinks) const = 0;

    /** @brief Activate a route thanks to the route description given
        @param pinsLink The description of the route to activate
        @return True If no problem occurs */
    virtual bool activateRoute(const JsonObject &pinsLink) = 0;

    /** @brief The bank elements can call this method to display a form in the HMI, in order to
               get a param value
        @note The format of the json object to give is identical as the one defined in the
               method: @ref ISequenceInstanceModule::getModuleParametersFormat
        @param elemToDisplay The elements to display
        @param uuidToken Unique identifier of the token
        @return True If no problem occurred */
    virtual bool displayParameterInput(const JsonObject &elemToDisplay, QString &uuidToken) = 0;

    /** @brief The bank elements can call this method to update a displaying form in the HMI
        @note The format of the json object to give is identical as the one defined in the
               method: @ref ISequenceInstanceModule::getModuleParametersFormat
        @param uuidToken Unique identifier of the currently displaying HMI
        @param elemToUpdate The elements to update in the displaying view (not all the elements
                            have to be resent, only those which are changing)
        @return True If no problem occurred */
    virtual bool updateDisplay(const QString &uuidToken, const JsonObject &elemToUpdate) = 0;

    /** @brief Read a setting from the application property
        @param settingStrKey The key of the setting
        @param value The value of the setting wanted
        @return False if a problem occurs or if the setting key is unknown */
    virtual bool readSetting(const QString &settingStrKey, QVariant &value) const = 0;

    /** @brief Get a value stored in the permanent storage of the application
        @param storeKey The key of the stored value
        @param storeValue The value got thanks to the key
        @return True if no problem occurred */
    virtual bool getPermStoreValue(const QString &storeKey, QString &storeValue) const = 0;

    /** @brief Set a value in the permanent storage of the application
        @param storeKey The key of the value to store
        @param storeValue The value to set in the permanent storage
        @return True if no problem occurred */
    virtual bool setPermStoreValue(const QString &storeKey, const QString &storeValue) = 0;

    /** @brief Get a value stored in the current project cache
        @note The cache lives with the project, when it's deleted, the cache is lost
        @note The cache is shared with all the other sequences of the project
        @param seqToken The token of the sequence to get the cache project linked
        @param cacheKey The key of the value to get
        @param cacheValue The value to get
        @return True if no problem occurred */
    virtual bool getProjectCacheValue(const QString &seqToken,
                                      const QString &cacheKey,
                                      QVariant &cacheValue) const = 0;

    /** @brief Set a value in the current project cache
        @note The cache lives with the project, when it's deleted, the cache is lost
        @note The cache is shared with all the other sequences of the project
        @param seqToken The token of the sequence to get the cache project linked
        @param cacheKey The key of the value to set
        @param cacheValue The value to set
        @return True if no problem occurred */
    virtual bool setProjectCacheValue(const QString &seqToken,
                                      const QString &cacheKey,
                                      const QVariant &cacheValue) = 0;

    /** @brief Get the sequence log manager attached to the sequence targetted by its token
        @warning The pointer is managed by the testbed lib, do not delete it
        @note The object is in the same thread of the plugin
        @param sequenceUniqueToken The token of the sequence targetted
        @return The log manager of the sequence or nullptr if the sequence hasn't been found by
                its token (or a problem occurs) */
    virtual const ISequenceLogManager *getSequenceLogManager(
        const QString &sequenceUniqueToken) const = 0;

    /** @brief Get the folder of the sequence targeted by its token
        @note The directory returned exists (it's tested in the method) or an error is returned
        @param seqToken The token of the sequence to get its folder
        @param seqDir The directory of the sequence
        @return True if no problem occurs */
    virtual bool getSequenceDirectory(const QString &seqToken, QDir &seqDir) const = 0;

    /** @brief The bank elements can call this method to cancel the display in the HMI
        @param uuidToken Unique identifier of the GUI's token
        @return True if no error occurs */
    virtual bool cancelDisplay(const QString &uuidToken) = 0;

signals:
    /** @brief Emitted when the token is finished
        @note This signal is emitted for each launched GUI; therefore, the slot connected to
              this signal has to verify the uuidToken in order to know if the signal is for it.
        @param success True if no error occurred in the UI displaying
        @param uuidToken Unique identifier of the GUI's token
        @param valuesSet The values set in the UI displaying */
    void guiResult(bool success, const QString &uuidToken, const JsonArray &valuesSet);
};
