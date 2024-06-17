<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Plugin and test bed lib API documentation

## Table of contents

- [Plugin and test bed lib API documentation](#plugin-and-test-bed-lib-api-documentation)
  - [Table of contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Class diagram](#class-diagram)
  - [Default classes](#default-classes)

## Introduction

This file contains the definition of the interface between the plugins and test bed lib

## Class diagram

```mermaid
classDiagram
    class ICommonInterface {
        <<interface>>
        +getRoutesLinkedToTopologyModule(QString bankUniqueName,\nQString uniqueTopologyModuleName,\nQString uniqueTopologyInstanceModuleName,\nJsonArray &pinsLinks) bool*
        +activateRoute(JsonObject pinsLinks) bool*
        +displayParameterInput(JsonObject elemToDisplay, QString uuidToken) bool*
        +updateDisplay(QString uuidToken, JsonObject elemToUpdate) bool*
        +readSetting(QString settingStrKey, QVariant &value) bool*
        +getPermStoreValue(QString storeKey, QString &storeValue) bool*
        +setPermStoreValue(QString storeKey, QString storeValue) bool*
        +getProjectCacheValue(QString seqToken,\nQString cacheKey,\nQString &cacheValue) bool*
        +setProjectCacheValue(QString seqToken,\nQString cacheKey,\nQString cacheValue) bool*
        +getSequenceLogManager(QString sequenceUniqueToken) ISequenceLogManager**
        +getSequenceDirectory(QString seqToken, QDir &seqDir) bool*
        +cancelDisplay(QString uuidToken) bool*

        +~signal~ guiResult(bool success, QString uuidToken, JsonArray valueSet)
    }
    style ICommonInterface fill:#747239,color:#fff

    class ISequenceLogManager {
        <<interface>>
        +getAllTheSequenceLogs(QVector~SequenceLogData~ &sequenceLogs,\nSequenceLogCategory categoryFilter,\nLogMsgType criticityLevel,\nqint64 fromTimestampMSecsSinceEpoch) bool*

        +~signal~ newLogAdded(SequenceLogData newLog)
    }
    style ISequenceLogManager fill:#747239,color:#fff

    class IPlugin {
        +accessBankInterface(ICommonInterface &commonInterface) QSharedPointer~IBankInterface~*
    }
    style IPlugin fill:#747239,color:#fff

    class IBankInterface {
        +getUniqueName(QString &name) bool*
        +getVersion(QString &version) bool*
        +getTopologyModulesName(QVector~QString~ &modulesName) bool*
        +getSequenceModulesName(QVector~QString~ &modulesName) bool*
        +createTopologyInstanceModule(QString moduleName) ITopologyInstanceModule**
        +createSequenceInstanceModule(QString moduleName,\nQString sequenceToken) ISequenceInstanceModule**

        +~signal~ newLog(LogMsgType logType, QString logMsg)
    }
    style IBankInterface fill:#747239,color:#fff

    class ISequenceInstanceModule {
        +getBankInterface() QSharedPointer~IBankInterface~*
        +getModuleName(QString &uniqueName) bool*
        +getInstanceName(QString &instanceName) bool*
        +setSequenceToken(QString sequenceToken) bool*
        +launchModule(JsonArray jsonObjectInputValues) bool*
        +stopModule(StopAskedType stopAskedType) bool*
        +manageModulePause(bool pauseModule) bool*
        +getState(SequenceStateType currentState) bool*
        +getModuleParametersFormat(JsonArray &parameters) bool*
        +setModuleParameters(JsonArray parameters, JsonArray &errors) bool*
        +getModuleParameters(JsonArray &parameters) bool*
        +getInputVariablesFormat(JsonArray &inVariablesFormat) bool*
        +getOutputVariablesFormat(JsonArray &outVariablesFormat) bool*
        +~signal~ newState(SequenceStateType currentState, SequenceStateType previousState)
        +~signal~ moduleFinished(bool success, JsonArray jsonObjectOutputValues)
        +~signal~ newLog(SequenceLogData log)
    }
    style ISequenceInstanceModule fill:#747239,color:#fff

    class ITopologyInstanceModule
    style ITopologyInstanceModule fill:#747239,color:#fff

    ICommonInterface ..> ISequenceLogManager : uses
    IPlugin ..> IBankInterface : uses
    IBankInterface ..> ITopologyInstanceModule : creates
    IBankInterface ..> ISequenceInstanceModule : creates
    IBankInterface <.. ISequenceInstanceModule : uses
```

## Default classes

TODO: add here the explanation of how the default classes implements the interface
