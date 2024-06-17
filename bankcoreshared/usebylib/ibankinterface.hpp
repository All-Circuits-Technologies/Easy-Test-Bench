// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "logsutility/logmsgtype.hpp"
#include "testbedcore/testbedglobal.hpp"

#include <QSharedPointer>

class ICommonInterface;
class ISequenceInstanceModule;
class ITopologyInstanceModule;


/** @brief Main interface with a Bank */
class TESTBEDPLUGIN_DLLSPEC IBankInterface : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent Class parent */
        explicit IBankInterface(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~IBankInterface() override = default;

    public:
        /** @brief Get the bank unique name
            @param name The bank unique name
            @return True if no problem occurs */
        virtual bool getUniqueName(QString &name) const = 0;

        /** @brief Get the bank version
            @param version The bank version
            @return True If no problem occurs */
        virtual bool getVersion(QString &version) const = 0;

        /** @brief Get the name of all the bank topology modules
            @param modulesNames The topology module names
            @return True If no problem occurs */
        virtual bool getTopologyModulesName(QVector<QString> &modulesName) const = 0;

        /** @brief Get the name of all the bank sequence modules
            @param modulesNames The sequence module names
            @return True If no problem occurs */
        virtual bool getSequenceModulesName(QVector<QString> &modulesName) const = 0;

        /** @brief Create a topology instance module
            @note This method creates an unique instance
            @warning The method creates a class instance with no owner, the responsibility of the
                     deletion is given to the caller.
            @param moduleName The name of module to create instance
            @return The topology instance module created */
        virtual ITopologyInstanceModule *createTopologyInstanceModule(
                                                                    const QString &moduleName) = 0;

        /** @brief Create a sequence instance module
            @note This method creates an unique instance
            @warning The method creates a class instance with no owner, the responsibility of the
                     deletion is given to the caller.
            @param moduleName The name of module to create instance
            @param sequenceToken The token of the sequence where the module is created and where it
                                 will run into
            @return The sequence instance module created */
        virtual ISequenceInstanceModule *createSequenceInstanceModule(
                                                                const QString &moduleName,
                                                                const QString &sequenceToken) = 0;

    signals:
        /** @brief Emitted when a new log is generated from the bank
            @param logType The log type
            @param logMsg The log message */
        void newLog(LogMsgType::Enum logType, const QString &logMsg);
};
