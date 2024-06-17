// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QSharedPointer>

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"

class ADisplayUserInterface;
class DisplayUiToken;
class DbManager;
class DbPluginPermStorageAsker;


/** @brief Bank common interface implementation */
class BankCommonInterface : public ICommonInterface
{
    Q_OBJECT

    private:
        /** @brief This structure is used with the @ref getRoutesLinkedToTopologyModule method
                   in order to return values through threads */
        struct GetRoutesLinkedResults
        {
            bool success;
            JsonArray pinsLinks;
        };

        /** @brief This structure is used with the @ref getProjectCacheValueImpl method
                   in order to return values through threads */
        struct GetProjectCache
        {
            bool success;
            QVariant cacheValue;
        };

    public:
        /** @brief Class constructor
            @param userInterface The user interface object created in the application to display
                                 widgets in it, if null, the @ref displayParameterInput method
                                 will always return false
            @param dbManager The database manager to use in order to manage the permanent storage
            @param parent The parent class */
        explicit BankCommonInterface(ADisplayUserInterface *userInterface,
                                     DbManager &dbManager,
                                     QObject *parent = nullptr);

    public:
        /** @see ICommonInterface::getRoutesLinkedToTopologyModule
            @note The method is synchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method
                     but to use @ref getRoutesLinkedToTopologyModuleImpl method to do it.
            @note The method is thread safe */
        virtual bool getRoutesLinkedToTopologyModule(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    JsonArray &pinsLinks) const override;

        /** @see ICommonInterface::activateRoute
            @note The method is synchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method
                     but to use @ref activateRouteImpl method to do it.
            @note The method is thread safe */
        virtual bool activateRoute(const JsonObject &pinsLink) override;

        /** @see ICommonInterface::displayParameterInput
            @note The method is asynchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method,
                     but to use the @ref ADisplayUserInterface object to do what you want to do
            @note The method is thread safe */
        virtual bool displayParameterInput(const JsonObject &elemToDisplay,
                                           QString &uuidToken) override;

        /** @see ICommonInterface::updateDisplay
            @note The method is asynchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method,
                     but to use the @ref ADisplayUserInterface object to do what you want to do
            @note The method is thread safe */
        virtual bool updateDisplay(const QString &uuidToken,
                                   const JsonObject &elemToUpdate) override;

        /** @see ICommonInterface::getPermStoreValue */
        virtual bool getPermStoreValue(const QString &storeKey, QString &storeValue) const override;

        /** @see ICommonInterface::getPermStoreValue */
        virtual bool setPermStoreValue(const QString &storeKey, const QString &storeValue) override;

        /** @see ICommonInterface::getProjectCacheValue
            @note The method is synchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method
                     but to use @ref getProjectCacheValueImpl method to do it.
            @note The method is thread safe */
        virtual bool getProjectCacheValue(const QString &seqToken,
                                          const QString &cacheKey,
                                          QVariant &cacheValue) const override;

        /** @see ICommonInterface::setProjectCacheValue
            @note The method is synchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method
                     but to use @ref setProjectCacheValueImpl method to do it.
            @note The method is thread safe */
        virtual bool setProjectCacheValue(const QString &seqToken,
                                          const QString &cacheKey,
                                          const QVariant &cacheValue) override;

        /** @see ICommonInterface::readSetting
            @note The method is thread safe */
        virtual bool readSetting(const QString &settingStrKey, QVariant &value) const override;

        /** @brief ICommonInterface::getSequenceLogManager
            @note The method is synchronous and acts as if all is called in the same thread
            @warning If you derive this class it's strongly not recommended to override this method,
                     but to use the @ref getSequenceLogManagerImpl method to do it
            @note The method is thread safe */
        virtual const ISequenceLogManager *getSequenceLogManager(
                                                const QString &sequenceUniqueToken) const override;

        /** @see ICommonInterface::getSequenceDirectory
            @note The method is thread safe */
        virtual bool getSequenceDirectory(const QString &seqToken, QDir &seqDir) const override;

        /** @brief The bank elements can call this method to cancel the display in the HMI
            @param uuidToken token unique identifier of the GUI to be closed
            @return True if no errors occurs */
        virtual bool cancelDisplay(const QString &uuidToken) override;

    public slots:
        /** @brief Received when the token is finished. Remove the token from the QHash
            @param success True if no error occurred in the UI displaying
            @param uuidToken uuid of the finished token
            @param valuesSet The values set in the UI displaying */
        void onTokenFinished(bool success, const QString &uuidToken, const JsonArray &valuesSet);

    protected:
        /** @brief Contains the implementation of the @ref getRoutesLinkedToTopologyModule
            @see ICommonInterface::getRoutesLinkedToTopologyModule */
        virtual bool getRoutesLinkedToTopologyModuleImpl(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    JsonArray &pinsLinks) const;

        /** @brief Contains the implementation of the @ref activateRouteImpl
            @see ICommonInterface::activateRoute */
        virtual bool activateRouteImpl(const JsonObject &pinsLink);


        /** @brief Contains the implementation of the @ref getProjectCacheValue
            @see ICommonInterface::getProjectCacheValue */
        virtual bool getProjectCacheValueImpl(const QString &seqToken,
                                              const QString &cacheKey,
                                              QVariant &cacheValue) const;

        /** @brief Contains the implementation of the @ref setProjectCacheValue
            @see ICommonInterface::setProjectCacheValue */
        virtual bool setProjectCacheValueImpl(const QString &seqToken,
                                              const QString &cacheKey,
                                              const QVariant &cacheValue);

        /** @brief Contains the implementation of the @ref getSequenceLogManager
            @see ICommonInterface::getSequenceLogManager */
        virtual const ISequenceLogManager *getSequenceLogManagerImpl(
                                                        const QString &sequenceUniqueToken) const;

    private:
        /** @brief This method allowes to call @ref getRoutesLinkedToTopologyModuleImpl method from
                   an another thread and get all the return values from it
            @see ICommonInterface::getRoutesLinkedToTopologyModule
            @return The method results */
        GetRoutesLinkedResults getRoutesLinkedToTopologyModulePriv(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    const JsonArray &pinsLinks) const;

        /** @brief This method allowes to call @ref getProjectCacheValueImpl method from
                   an another thread and get all the return values from it
            @see ICommonInterface::getProjectCacheValue
            @return The method results */
        GetProjectCache getProjectCacheValuePriv(const QString &seqToken,
                                                 const QString &cacheKey) const;

    private:
        ADisplayUserInterface *_userInterface{nullptr};
        QHash<QString,QSharedPointer<DisplayUiToken>> _tokensList;

        DbPluginPermStorageAsker *_dbPluginCacheAsker{nullptr};
};
