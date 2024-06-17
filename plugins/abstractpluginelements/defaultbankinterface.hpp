// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "bankcoreshared/usebylib/ibankinterface.hpp"

#include <QDebug>
#include <QHash>
#include <QMetaObject>

class RegisterModuleUtilityKey;


/** @brief This class gives default implementation of the @ref IBankInterface (in order to be used
           by plugins). The default implementations of some API methods as the bank unique name or
           the bank version */
class DefaultBankInterface : public IBankInterface
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param commonInterface The common interface given by the lib
            @param parent The class parent */
        explicit DefaultBankInterface(ICommonInterface &commonInterface, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DefaultBankInterface() override = default;

    public:
        /** @see IBankInterface::getUniqueName
            @note The value is read from the pri file and the variable: PLUGIN_NAME*/
        virtual bool getUniqueName(QString &name) const override;

        /** @see IBankInterface::getVersion
            @note The value is read from the pri file and the variable: VERSION_STRING */
        virtual bool getVersion(QString &version) const override;

        /** @see IBankInterface::getTopologyModulesName
            @see DefaultBankInterface::getModuleNames */
        virtual bool getTopologyModulesName(QVector<QString> &modulesName) const override;

        /** @see IBankInterface::getSequenceModulesName
            @see DefaultBankInterface::getModuleNames */
        virtual bool getSequenceModulesName(QVector<QString> &modulesName) const override;

        /** @see IBankInterface::createTopologyInstanceModule
            @see DefaultBankInterface::createNewInstance */
        virtual ITopologyInstanceModule *createTopologyInstanceModule(
                                                                const QString &moduleName) override;

        /** @see IBankInterface::createSequenceInstanceModule
            @see DefaultBankInterface::createNewInstance*/
        virtual ISequenceInstanceModule *createSequenceInstanceModule(
                                                            const QString &moduleName,
                                                            const QString &sequenceToken) override;

        /** @brief Get the common interface with the test bed lib */
        const ICommonInterface &getCommonInterface() const { return _commonInterface; }

        /** @brief Access to the common interface with the test bed lib */
        ICommonInterface &accessCommonInterface() const { return _commonInterface; }

    private:
        /** @brief Help to differentiate between sequence and topology module */
        enum ModuleType
        {
            Sequence,
            Topology
        };

    private:
        /** @brief Get the name of modules registered thanks to the
                   @ref RegisterSequenceModuleUtility and @ref RegisterTopologyModuleUtility classes
            @param moduleType Tell if the modules name to get are sequence modules or topology
                               modules
            @param modulesName The registered modulesName
            @return True if no problem occurs */
        bool getModuleNames(ModuleType moduleType, QVector<QString> &modulesName) const;

        /** @brief Create a new instance of the object, find thanks to the name given
            @note The module has to be registered thanks to the classes:
                  @ref RegisterSequenceModuleUtility and @ref RegisterTopologyModuleUtility, in
                  order to be known and so created here.
            @param moduleName The name of the module to create
            @param moduleType Tell if the module to create is a sequence or a topology module
            @return The object created or nullptr if the module hasn't been found */
        QObject *createNewInstance(const QString &moduleName, ModuleType moduleType);

    private:
        /** @brief Get the modules registered
            @param moduleType Tell if the modules to get are sequence modules or topology
                               modules
            @return The registered modules */
        static QHash<QString, QMetaObject> getRegisteredModules(ModuleType moduleType);

    private:
        ICommonInterface &_commonInterface;
};
