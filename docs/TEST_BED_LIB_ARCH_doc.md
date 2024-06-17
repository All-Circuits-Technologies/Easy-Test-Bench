<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Test bed lib architecture documentation

## Table of contents

- [Test bed lib architecture documentation](#test-bed-lib-architecture-documentation)
  - [Table of contents](#table-of-contents)
  - [Preamble](#preamble)
  - [Architecture](#architecture)
    - [Main classes](#main-classes)
    - [Plugins management](#plugins-management)
    - [Settings management](#settings-management)
    - [Database management](#database-management)
    - [Authentication management](#authentication-management)
    - [Permission management](#permission-management)
    - [Errors management](#errors-management)
    - [File loading management](#file-loading-management)
    - [Project management](#project-management)
      - [Project manager](#project-manager)
      - [Nodes sequences](#nodes-sequences)
      - [References nodes](#references-nodes)
      - [Sequences nodes](#sequences-nodes)
      - [Decisional transitions](#decisional-transitions)

## Preamble

This file contains the test bed lib architecture documentation.

`banks` and `plugins` are synonyms here. We created the name `banks` to not mistake

## Architecture

### Main classes

This shows the top and entry classes of the Test Bed core lib.

```mermaid
classDiagram
    direction LR
    class GlobalManager{
        +create(ADisplayUserInterface *displayUserInterface)$
        +instance() GlobalManager&$
    }

    class ProjectManager {
        <<singleton>>
    }
    style ProjectManager fill:#167034,color:#fff

    class BankManager {
        <<singleton>>
    }
    style BankManager fill:#8F6965,color:#fff

    class SettingsManager {
        <<singleton>>
    }
    style SettingsManager fill:#3D65A6,color:#fff

    class DbManager {
        <<singleton>>
    }
    style DbManager fill:#BD5646,color:#fff

    class PermissionManager {
        <<singleton>>
    }
    style PermissionManager fill:#B8449B,color:#fff

    class AuthenticationManager {
        <<singleton>>
    }
    style AuthenticationManager fill:#8755D4,color:#fff

    class LogsManager {
        <<singleton>>
    }
    note for LogsManager "Contained in the actlibs/logsutility"
    style LogsManager fill:#FFFA7C,color:#000

    class TicManager {
        <<singleton>>
    }
    note for TicManager "Contained in the actlibs/ticutility"
    style TicManager fill:#FFFA7C,color:#000

    class YamlToJsonManager {
        <<singleton>>
    }
    note for YamlToJsonManager "Contained in the actlibs/yamlutility"
    style YamlToJsonManager fill:#FFFA7C,color:#000

    GlobalManager "0..1" *-- "1" ProjectManager
    GlobalManager "0..1" *-- "1" BankManager
    GlobalManager "0..1" *-- "1" LogsManager
    GlobalManager "0..1" *-- "1" SettingsManager
    GlobalManager "0..1" *-- "1" DbManager
    GlobalManager "0..1" *-- "1" PermissionManager
    GlobalManager "0..1" *-- "1" AuthenticationManager
    GlobalManager "0..1" *-- "1" TicManager
    GlobalManager "0..1" *-- "1" YamlToJsonManager
```

### Plugins management

First see the interface between the `plugins` and `test bed lib`:
[plugin core api](PLUGIN_CORE_API_doc.md)

```mermaid
classDiagram
    direction TB
    class BankManager {
        <<singleton>>
        -QHash~QString, BankData~ _bankInterfaces

        +getBankNames() QVector~QString~
        +getBankInterface(QString name) QSharedPointer~IBankInterface~
        +accessCommonInterface(QString name) BankCommonInterface&
        +reloadBanks() bool
    }
    style BankManager fill:#8F6965,color:#fff

    class BankData {
        <<structure>>
        +QSharedPointer~IBankInterface~ interface
        +QPLuginLoader* pluginLoader
    }
    style BankData fill:#8F6965,color:#fff

    class BankCommonInterface {
        +~slots~ onTokenFinished(bool success, QString uuidToken, JsonArray valuesSet)
    }
    style BankCommonInterface fill:#8F6965,color:#fff

    class ICommonInterface {
        <<interface>>
    }
    style ICommonInterface fill:#747239,color:#fff

    class DbPluginPermStorageAsker
    style DbPluginPermStorageAsker fill:#BD5646,color:#fff

    class ADisplayUserInterface

    class DisplayUiToken

    class SequenceLogManager
    style SequenceLogManager fill:#167034,color:#fff

    BankManager "1" --> "*" BankData
    BankManager "1" *--> "1" BankCommonInterface
    BankCommonInterface --|> ICommonInterface
    BankCommonInterface "1" *--> "1" DbPluginPermStorageAsker: _dbPluginCacheAsker
    BankCommonInterface "1" --> "1" ADisplayUserInterface: _userInterface
    BankCommonInterface "1" --> "*" DisplayUiToken: _tokensList
    BankCommonInterface "1" ..> "*" SequenceLogManager
```

### Settings management

This describes the classes used to manage the library settings.

```mermaid
classDiagram
    class SettingsManager {
        <<singleton>>
        -QHash~KnownSettings, QVariant~ _cache
        -QSettings* _settingsFile
        -QReadWriteLock* _readWriteLock

        +load() bool
        +clear()
        +read(KnownSettings setting, QVariant &value) bool
        +read(KnownSettings setting, QString &value) bool
        +write(KnownSettings setting, QVariant value, bool writeToIniFile) bool
    }
    style SettingsManager fill:#3D65A6,color:#fff

    class KnownSettings {
        <<enumeration>>
        PluginPath
        ProjectFileDefaultPath
        LogAppFolderPath
        LogBusinessFolderPath
        LogFileNamePattern
        LogConsoleCriticity
        LogFileSavingCriticity
        TranslateFilesFolderPath
        DatabaseFilePath
        YqProgramPath
        YamlWorkerDirPath
        UnknownSetting

        +getSettingDetails(KnownSettings value) KnownSettingDetails$
    }
    style KnownSettings fill:#3D65A6,color:#fff

    class KnownSettingDetails {
        +QString name
        +QString optionLetterName
        +QString optionDescription
        +QString optionValueName
        +QVariant constantDefaultValue
    }
    style KnownSettingDetails fill:#3D65A6,color:#fff

    KnownSettings ..> KnownSettingDetails: uses
    SettingsManager ..> KnownSettings: uses
```

### Database management

This is the ERB diagram of the library internal database.

```mermaid
erDiagram
    libOptions {
        TEXT dbVersion "DEFAULT 0"
        INTEGER guestModeAllowed "DEFAULT 1; between 0 and 1"
        INTEGER lastModified "prevent updating with old info"
    }

    projects {
        INTEGER id PK
        TEXT name
        TEXT path
        INTEGER lastOpened "DEFAULT 0"
        INTEGER lastModified "prevent updating with old info"
    }

    users {
        INTEGER id PK
        TEXT firstName
        TEXT lastName
        TEXT userName
        TEXT password
        TEXT salt
        TEXT roles
        INTEGER lastModified "prevent updating with old info"
    }

    roles {
        INTEGER id PK
        TEXT name UK
        INTEGER lastModified "prevent updating with old info"
    }

    permissions {
        INTEGER id PK
        TEXT name UK
        TEXT description
        INTEGER lastModified "prevent updating with old info"
    }

    permissionRoles {
        INTEGER id PK
        TEXT permissionName FK
        TEXT roleName FK
        INTEGER lastModified "prevent updating with old info"
    }

    pluginPermanentStorage {
        INTEGER id PK
        TEXT key
        TEXT value
        TEXT note
        INTEGER lastModified "prevent updating with old info"
    }

    roles ||--o{ permissionRoles : roleName
    permissions ||--o{ permissionRoles : permissionName
```

Those are the classes used to manage the access in the local database (CRUD).

```mermaid
classDiagram
    class DbManager {
        <<singleton>>
        +QSharedPointer~LibOptions~ _libOptions
        +DbThread* _dbThread

        +initDbManager(QString path) bool
        +select(QString queryOperation,\nQVector~QSharedPointer~T~~ dbObjects) bool
        +select(QVector~QSharedPointer~T~~ dbObjects) bool
        +selectOne(QString queryOperation, QSharedPointer~T~ dbObject) bool
        +selectOne(qint64 id, QSharedPointer~T~ dbObject) bool
        +insert(QSharedPointer~T~ dbObject) bool
        +remove(QSharedPointer~T~ dbObject) bool
        +removeAll() bool
        +update(QSharedPointer~QObject~ dbObject,\nQVector~QString~ propertiesToNotUpdate) bool
        +getLibOptions() QSharedPointer~LibOptions~
    }
    style DbManager fill:#BD5646,color:#fff

    class DbSqlQueryHandler {
        +QSqlQuery* _sqlQuery
    }
    style DbSqlQueryHandler fill:#BD5646,color:#fff

    class DbThread {
        +initDbManager(QString path) bool
        +exec(QString sqlQuery) bool
        +exec(QString sqlQuery,\nQList~QString~ placeholderNames,\nQList~QVariant~ values) bool
        +execSelect(QString sqlQuery,\nQScopedPointer~DbSqlQueryHandler~ query) bool
        +executeSqlScriptFile(QString filePath) bool
    }
    style DbThread fill:#BD5646,color:#fff

    class BaseThread
    style BaseThread fill:#FFFA7C,color:#000

    class DbExecuter {
        +initDbManager(QString path) bool
        +exec(QString sqlQuery) bool
        +exec(QString sqlQuery,\nQList~QString~ placeholderNames,\nQList~QVariant~ values) bool
        +execSelect(QString sqlQuery,\nQScopedPointer~DbSqlQueryHandler~ query) bool
        +executeSqlScriptFile(QString filePath) bool
    }
    style DbExecuter fill:#BD5646,color:#fff

    class DbHandlerManager {
        +setInactiveTimeoutInMs(int inactiveTimeout,\nQString connectionName)

        +~slot~ incrementHandlerNb(DbHandlerKey key,\nQString connectionName)
        +~slot~ decrementHandlerNb(DbHandlerKey key,\nQString connectionName)
        +~slot~ pingDb(QString connectionName)
    }
    style DbHandlerManager fill:#BD5646,color:#fff

    class DbHandlerKey
    style DbHandlerKey fill:#BD5646,color:#fff

    class DbHandler {
        +QSqlDatabase _db
        +QString _connectionName

        +accessDb() QSqlDatabase
    }
    style DbHandler fill:#BD5646,color:#fff


    class LibOptions
    style LibOptions fill:#BD5646,color:#fff

    DbThread --|> BaseThread
    DbManager "1" *--> "1" DbThread : _dbThread
    DbThread "1" *--> "1" DbExecuter : _dbExecuter
    DbExecuter "1" *--> "1" DbHandlerManager : _handlerManager
    DbManager "1" o--> "1" LibOptions
    DbManager ..> DbSqlQueryHandler : uses
    DbThread ..> DbSqlQueryHandler : uses
    DbExecuter ..> DbSqlQueryHandler : uses
    DbHandlerManager ..> DbHandlerKey : uses
    DbHandler "*" o--> "1" DbHandlerManager
    DbHandlerKey ..> DbHandler : friend
```

This diagram contains:
- The classes which represent the database entities
- The "Askers" classes which easify the DB requests

```mermaid
classDiagram
    direction LR
    class DbManager {
        <<singleton>>
    }
    style DbManager fill:#BD5646,color:#fff

    class DbLibOptionsAsker {
        +getLibOptions(QSharedPointer~LibOptions~ libOptions) bool
        +updateLibOptions(QSharedPointer~LibOptions~ libOptions) bool
    }
    style DbLibOptionsAsker fill:#BD5646,color:#fff

    class DbPermissionAsker {
        +getAllUserPermissions(QSharedPointer~user~ user,\nQVector~QSharedPointer~Permission~~ permissions) bool
    }
    style DbPermissionAsker fill:#BD5646,color:#fff

    class DbPluginPermStorageAsker {
        +getValue(QString key,\nQSharedPointer~PluginPermStorageValue~ &storageValue) bool
        +insertValue(QSharedPointer~PluginPermStorageValue~ storageValue) bool
        +modifyValue(QSharedPointer~PluginPermStorageValue~ storageValue) bool
        +setValue(QString key, QString value) bool
        +getAllCacheValue(QVector~QSharedPointer~PluginPermStorageValue~~\nstorageValues) bool
        +clearStorage() bool
    }
    style DbPluginPermStorageAsker fill:#BD5646,color:#fff

    class DbProjectAsker {
        +getRecentProjects(QVector~QSharedPointer~Project~~ &projects) bool
        +updateProject(QSharedPointer~Project~ project) bool
        +addProject(QSharedPointer~Project~ project) bool
        +deleteProject(QSharedPointer~Project~ project) bool
    }
    style DbProjectAsker fill:#BD5646,color:#fff

    class DbUserAsker {
        +addUser(QSharedPointer~User~ user) bool
        +getUserByUsername(QString username,\nQSharedPointer~User~ &user) bool
        +deleteUser(QSharedPointer~User~ user) bool
        +updateUser(QSharedPointer~User~ user) bool
        +getAllUsers(QVector~QSharedPointer~User~~ &users) bool
    }
    style DbUserAsker fill:#BD5646,color:#fff

    class DbObject {
        +qint64 id
        +qint64 lastModified
    }
    style DbObject fill:#BD5646,color:#fff

    class LibOptions {
        -QVersionNumber _dbVersion
        +bool guestModeAllowed

        +getDbVersion() QString
        +setDbVersion(QString value)
    }
    style LibOptions fill:#BD5646,color:#fff

    class Permission {
        +QString name
        +QString description
    }
    style Permission fill:#BD5646,color:#fff

    class PluginPermStorageValue {
        +QString key
        +QString value
        +QString note
    }
    style PluginPermStorageValue fill:#BD5646,color:#fff

    class Project {
        +QString name
        +QString path
        +qint64 lastOpened
    }
    style Project fill:#BD5646,color:#fff

    class Role {
        +QString name
    }
    style Role fill:#BD5646,color:#fff

    class User {
        +QString firstName
        +QString lastName
        +QString userName
        +QString password
        +QString salt
        +QVector~QString~ roles
    }
    style User fill:#BD5646,color:#fff

    DbLibOptionsAsker "1" o--> "1" DbManager
    DbLibOptionsAsker ..> LibOptions : uses
    DbPermissionAsker "1" o--> "1" DbManager
    DbPermissionAsker ..> User : uses
    DbPermissionAsker ..> Permission : uses
    DbPluginPermStorageAsker ..> PluginPermStorageValue : uses
    DbPluginPermStorageAsker "1" o--> "1" DbManager
    DbProjectAsker ..> Project : uses
    DbProjectAsker "1" o--> "1" DbManager
    DbUserAsker ..> User : uses
    DbUserAsker "1" o--> "1" DbManager
    DbManager "1" o--> "1" LibOptions

    LibOptions --|> DbObject
    Permission --|> DbObject
    PluginPermStorageValue --|> DbObject
    Project --|> DbObject
    Role --|> DbObject
    User --|> DbObject
```

### Authentication management

```mermaid
classDiagram
    class AuthenticationManager {
        <<singleton>>
        -QSharedPointer~User~ _currentUser

        +loginDefaultUser() bool
        +login(QString username, QString clearTextPassword, bool &idsAreCorrect) bool
        +logout()
        +getCurrentUser() QSharedPointer~User~

        +saltAndHashPassword(QString clearTextPassword, QString salt) QString$

        +~signal~ userConnected(QSharedPointer~User~ user)
        +~signal~ userDisconnected()
    }
    style AuthenticationManager fill:#8755D4,color:#fff

    class User
    style User fill:#BD5646,color:#fff

    class DbUserAsker
    style DbUserAsker fill:#BD5646,color:#fff

    AuthenticationManager "1" *--> "1" DbUserAsker : _dbUserAsker
    AuthenticationManager "1" o--> "0..1" User : _currentUser
```

### Permission management

```mermaid
classDiagram
    class PermissionManager {
        -QVector~QSharedPointer~Permission~~ _userPermissions

        +hasPermission(QString permissionName) bool
        +loadUserPermissions(QSharedPointer~User~ user) bool

        +~slot~ onUserConnected(QSharedPointer~User~ user)
        +~slot~ onUserDisconnected()

        +~signal~ loadingStatusChanged(LoadingStatus loadingStatus)
    }
    style PermissionManager fill:#B8449B,color:#fff

    class LoadUserPermissionsThread {
        -QSharedPointer~User~ _user
        -QVector~QSharedPointer~Permission~~ _userPermissions

        +~signal~ loadingStatusChanged(LoadingStatus loadingStatus)
    }
    style LoadUserPermissionsThread fill:#B8449B,color:#fff

    class LoadingStatus {
        <<enumeration>>
        Started
        Failed
        Finished
    }
    style LoadingStatus fill:#B8449B,color:#fff

    class Permission
    style Permission fill:#BD5646,color:#fff

    class DbPermissionAsker
    style DbPermissionAsker fill:#BD5646,color:#fff

    class User
    style User fill:#BD5646,color:#fff

    PermissionManager "1" o--> "*" Permission : _userPermissions
    PermissionManager "1" *--> "1" DbPermissionAsker : _dbPermissionAsker
    LoadUserPermissionsThread "1" o--> "1" User : _user
    LoadUserPermissionsThread "1" o--> "1" DbPermissionAsker : _dbPermissionAsker
    LoadUserPermissionsThread "1" o--> "*" Permission : _userPermissions
    LoadingStatus <.. PermissionManager : uses
    LoadingStatus <.. LoadUserPermissionsThread : uses

    QThread <|-- LoadUserPermissionsThread
```

### Errors management

```mermaid
classDiagram
    class SequenceLoadingErrorHelper
    style SequenceLoadingErrorHelper fill:#167034,color:#fff

    class FileLoadingErrorHelper
    style FileLoadingErrorHelper fill:#00A8A5,color:#000

    class SequenceError {
        FailedNodesSequence
        FailedUnknownNodesSequence
        FailedChildNode
        FailedUnknownChildNode
    }
    style SequenceError fill:#167034,color:#fff

    class SequenceLoadingErrorHandler
    style SequenceLoadingErrorHandler fill:#167034,color:#fff

    class FileLoadingErrorHandler
    style FileLoadingErrorHandler fill:#00A8A5,color:#000

    class ErrorDescriptor
    style ErrorDescriptor fill:#00A8A5,color:#000

    FileLoadingErrorHelper <|-- SequenceLoadingErrorHelper
    FileLoadingErrorHandler <|-- SequenceLoadingErrorHandler
    SequenceLoadingErrorHandler ..>  ErrorDescriptor : uses
    SequenceLoadingErrorHandler ..>  SequenceError : uses
    SequenceLoadingErrorHelper ..>  SequenceError : uses
```

### File loading management

```mermaid
classDiagram
    direction TB

    class AJsonFile {
        -QFile *_file
        -QString _aliasFile

        +getFile() QFile&
        +getAbsoluteFilePath() QString
        +getAbsoluteDirPath() QString
    }
    style AJsonFile fill:#167034,color:#fff

    class ProjectFile {
        -QString _name
        -ProjectCache *_projectCache
        -QMap~QString#44; QSharedPointer~SequenceFile~~ _sequenceFiles

        +getName() QString
        +getSequenceFiles() QMap~QString#44; QSharedPointer~SequenceFile~~
        +setName(QString name)
        +addSequenceFile(QString fileName,\nQSharedPointer~SequenceFile~ sequence)
        +loadAndParseProjectFile(QString filePath,\nFileloadingErrorHandler *errorHandler) bool
        +accessCache() ProjectCache&
        +getCache() ProjectCache
    }
    style ProjectFile fill:#167034,color:#fff

    class ProjectCache {
        -QHash~QString, QVariant~ _projectCache

        +getValue(QString key) QVariant
        +setValue(QString key, QVariant value)
    }
    style ProjectCache fill:#167034,color:#fff

    class SequenceFile {
        -QVector~QSharedPointer~NodesSequence~~ _nodesSequence
        -QHash~SequenceThread*, SequenceThreadState~ _sequenceThreadList
        -bool _sequenceDataLoaded
        -QJsonArray _jsonArray

        +initAndSetSequenceFile(QFile *sequenceFile,\nFileloadingErrorHandler *errorHandler) bool
        +getSequenceThreadsNumber() int
        +accessSequenceThreads() QVector~SequenceThread*~
        +findFirstLoadedNodesSequence() QSharedPointer~NodesSequence~
        +getNodesSequence(QString sequenceUniqueToken)\nQSharedPointer~NodesSequence~
        +containsNodesSequence(QString sequenceUniqueToken) bool
        +loadSequenceData(InclusionNodeKey InclusionNodeKey,\nSequenceLoadingErrorHandler *errorHandler) bool
        +loadSequenceDataAndLaunchThreads(int nbrInstances,\nSequenceLoadingErrorHandler *errorHandler) bool
        +isSequenceDataLoaded() bool
        +areAllThreadsRunning() bool
        +areAllNodesSequenceLoaded() bool
        +accessJsonArray() QJsonArray&
        +stopAndClearThreads()

        +~signal~ allThreadsRunning()
    }
    style SequenceFile fill:#167034,color:#fff

    class SequenceThreadState {
        <<enumeration>>
        Initializing
        Running
        Stopped
    }
    style SequenceThreadState fill:#167034,color:#fff

    class SequenceThread
    style SequenceThread fill:#167034,color:#fff

    class NodesSequence
    style NodesSequence fill:#167034,color:#fff

    class InclusionNodeKey
    style InclusionNodeKey fill:#167034,color:#fff

    class AJsonParser {
        -QString _error
        +getError() QString
    }
    style AJsonParser fill:#167034,color:#fff

    class ProjectFileJsonParser {
        +loadAndParseProjectFile(QIODevice &projectDevice,\nProjectFile &projectFile,\nFileLoadingErrorHandler *errorHandler) bool$
    }
    style ProjectFileJsonParser fill:#167034,color:#fff

    class SequenceFileJsonParser {
        +loadSequenceFile(QIODevice &sequenceDevice,\nQJsonArray &jsonArray) bool$
    }
    style SequenceFileJsonParser fill:#167034,color:#fff

    AJsonFile <|-- ProjectFile
    AJsonFile <|-- SequenceFile
    ProjectFile "1" *--> "1" ProjectCache : _projectCache
    SequenceFile ..> SequenceThreadState : uses
    SequenceFile ..> InclusionNodeKey : uses
    SequenceFile "1" *--> "*" SequenceThread
    SequenceFile "1" o--> "*" NodesSequence
    ProjectFile "1" o--> "*" SequenceFile
    ProjectFileJsonParser --|> AJsonParser
    SequenceFileJsonParser --|> AJsonParser
    ProjectFile <.. ProjectFileJsonParser : uses
    SequenceFile ..> SequenceFileJsonParser : uses
```

### Project management

#### Project manager

```mermaid
classDiagram
    class ProjectManager {
        -QVector~QSharedPointer~ProjectFile~~ _loadedProjects
        -QVector~QSharedPointer~Project~~ _recentProjects
        -bool _recentProjectLoaded

        +loadProject(QString projectFilePath, FileLoadingErrorHandler *errorHanler) bool
        +loadProject(QSharedPointer<Project> project, FileLoadingErrorHandler *errorHandler) bool
        +unloadProject(QSharedPointer<ProjectFile> project) bool
        +accessProjects() QVector~QSharedPointer~ProjectFile~~
        +getProjects() QVector~QSharedPointer~ProjectFile~~
        +tryToGetLinkedProjectFile(QString sequenceToken) QSharedPointer~ProjectFile~
        +tryToAccessLinkedProjectFile(QString sequenceToken) QSharedPointer~ProjectFile~
        +tryToGetLinkedSequenceFile(QString sequenceToken) QSharedPointer~SequenceFile~
        +tryToGetLinkedNodesSequence(QString sequenceToken) QSharedPointer~NodesSequence~
        +accessRecentProjects() QVector~QSharedPointer~Project~~
        +forgetProject(QSharedPointer~Project~ project) bool

        +~slot~ unloadFirstProject() bool
        +~slot~ unloadAllProjects() bool

        +~signal~ recentProjectListUpdated()
    }
    style ProjectManager fill:#167034,color:#fff

    class Project
    style Project fill:#BD5646,color:#fff

    class ProjectFile
    style ProjectFile fill:#167034,color:#fff

    class SequenceFile
    style SequenceFile fill:#167034,color:#fff

    class NodesSequence
    style NodesSequence fill:#167034,color:#fff

    class SequenceThread
    style SequenceThread fill:#167034,color:#fff

    ProjectManager "1" o--> "*" Project : _recentProjects
    ProjectManager "1" o--> "*" ProjectFile : _loadedProjects
    ProjectManager ..> SequenceFile : uses
    ProjectManager ..> NodesSequence : uses
    SequenceFile "1" o--> "*" NodesSequence
    ProjectFile "1" o--> "*" SequenceFile
    SequenceFile "1" *--> "*" SequenceThread
```

#### Nodes sequences

```mermaid
classDiagram
    class SequenceThread {
        -NodesSequenceManager* _nodesSeqManager

        +getLinkedNodesSequence() QSharedPointer<NodesSequence>

        +~slot~ setSequenceData(QJsonArray jsonArray,\nSequenceLoadingErrorHandler *errorHandler) bool
        +~slot~ startSequence() bool
        +~slot~ stopSequence(StopAskedType stopAskedType) bool
        +~slot~ manageSequencePause(bool setToPause) bool
        +~slot~ cleanNodesSequence()
        +~slot~ cleanAndLoadNodesSequence(\nSequenceLoadingErrorHandler *errorHandler) bool
        +~slot~ prepareAndQuit()

        +~signal~ newLog(SequenceLogData newLog)
        +~signal~ nodesSequenceLoaded(bool success)
        +~signal~ nodeSeqFinished(bool success)
    }
    style SequenceThread fill:#167034,color:#fff

    class ANode
    style ANode fill:#167034,color:#fff

    class NodesSequenceManager {
        -QSharedPointer<NodesSequence> _nodesSequence
        -QJsonArray _nodesSequenceInitializer

        +~slot~ setSequenceData(QJsonArray jsonArray,\nSequenceLoadingErrorHandler *errorHandler)
        +~slot~ manageSequencePause(bool setToPause) bool
        +~slot~ startSequence() bool
        +~slot~ stopSequence(StopAskedType stopAskedType)
        +~slot~ cleanNodesSequence()
        +~slot~ cleanAndLoadNodesSequence(\nSequenceLoadingErrorHandler *errorHandler) bool

        +~signal~ nodesSequenceLoaded(bool success)
    }
    style NodesSequenceManager fill:#167034,color:#fff

    class NodesSequence {
        -QString _uniqueToken
        -QSharedPointer~QStateMachine~ _stateMachine
        -ReferencesHolder _referencesHolder
        -QString _errorString
        -QSharedPointer~ANode~ _guiltyNode
        -SequenceStateHelper _sequenceState
        -SequenceLogManager _sequenceLogManager
        -QHash~QString#44; QSharedPointer~ASequenceNode~~ _sequenceNodes
        -QHash~QString#44; QSharedPointer~AReferenceNode~~ _referenceNodes

        +startSequence() bool
        +stopSequence(StopAskedType stopAskedType) bool
        +managePauseAndResume(bool setToPause) bool
        +isSequenceRunning() bool
        +getSequenceStringError() QString
        +getGuiltyNode() QSharedPointer~ANode~

        +addSequenceNode(QSharedPointer~ASequenceNode~ node)
        +removeSequenceNode(QSharedPointer~ASequenceNode~ node)
        +addReferenceNode(QSharedPointer~AReferenceNode~ referenceNode)
        +removeReferenceNode(QSharedPointer~AReferenceNode~ referenceNode)
        +getSequenceNode(QString uniqueName) QSharedPointer~ASequenceNode~
        +getSequenceNodesName() QVector~QString~
        +getReferenceNode(QString uniqueName) QSharedPointer~AReferenceNode~
        +getVariableNode(QString uniqueName) QSharedPointer~VariableNode~
        +unloadAll()

        +findAChildNodesSequence(QString uniqueToken)\nQSharedPointer~NodesSequence~

        +~signal~ finished(bool success)
        +~signal~ hasToDie(QSharedPointer~ANode~ nodeToDetachWith)

        +createNodesSequence() QSharedPointer~NodesSequence~$
    }
    style NodesSequence fill:#167034,color:#fff

    class ReferencesHolder
    style ReferencesHolder fill:#167034,color:#fff

    class SequenceLogManager
    style SequenceLogManager fill:#167034,color:#fff

    class ASequenceNode
    style ASequenceNode fill:#167034,color:#fff

    class AReferenceNode
    style AReferenceNode fill:#167034,color:#fff

    class StopAskedType {
        <<enumeration>>
    }
    style StopAskedType fill:#00A8A5,color:#000

    class SequenceStateHelper
    style SequenceStateHelper fill:#00A8A5,color:#000

    NodesSequenceManager ..> StopAskedType : uses
    NodesSequence ..> StopAskedType : uses
    SequenceThread ..> StopAskedType : uses
    NodesSequence "1" *--> "1" SequenceStateHelper : _sequenceState
    NodesSequence "1" *--> "1" ReferencesHolder : _referencesHolder
    NodesSequence "1" *--> "1" SequenceLogManager : _sequenceLogManager
    NodesSequence "1" o--> "*" ASequenceNode : _sequenceNodes
    NodesSequence "1" o--> "*" AReferenceNode : _referenceNodes
    NodesSequenceManager "1" o--> "1" NodesSequence : _nodesSequence
    ANode <|-- NodesSequence
    SequenceThread "1" *--> "1" NodesSequenceManager : _nodesSeqManager
```

#### References nodes

```mermaid
classDiagram
    class ReferencesHolder {
        -ReferencesHolderType _referenceHolderType
        -QVector~InNodePort~ _inputs
        -QVector~OutNodePort~ _outputs

        +addInput(InNodePort inputNode)
        +addOutput(OutNodePort outputNode)
    }
    style ReferencesHolder fill:#167034,color:#fff

    class ReferencesHolderType {
        <<enumeration>>
        CanHaveInternalRefs
        OnlyExternalRefs
    }
    style ReferencesHolderType fill:#167034,color:#fff

    class InNodePort {
        -QSharedPointer~AReferenceNode~ _inputNode
        -QSharedPointer~AReferenceNode~ _internalInputNode
        -bool _canBeUndefined

        +setInputNode(QSharedPointer~AReferenceNode~ referenceNode,\nSequenceLoadingErrorHandler *errorHandler) bool
        +setInternalInputNode(\nQSharedPointer~AReferenceNode~ internalInputNode) bool
    }
    style InNodePort fill:#167034,color:#fff

    class OutNodePort {
        -QSharedPointer~VariableNode~ _outputNode
        -QSharedPointer~VariableNode~ _internalOutputNode
    }
    style OutNodePort fill:#167034,color:#fff

    class AReferenceNode {
        -QVariant _value
        -TypeManaged _typeManaged
        -Unit unit
    }
    style AReferenceNode fill:#167034,color:#fff

    class VariableNode {
        +setVariableValue(SequenceModuleNodeKey key,\nQVariant value) bool
        +setVariableValue(NodesSequenceKey key,\nQVariant value) bool
        +setVariableValue(ASequenceContainerNodeKey key,\nQVariant value) bool
    }
    style VariableNode fill:#167034,color:#fff

    class ConstantNode {
        +setConstantValue(QVariant value,\nSequenceLoadingErrorHandler *erroHandler) bool
    }
    style ConstantNode fill:#167034,color:#fff

    class SequenceModuleNodeKey
    style SequenceModuleNodeKey fill:#167034,color:#fff

    class NodesSequenceKey
    style NodesSequenceKey fill:#167034,color:#fff

    class ASequenceContainerNodeKey
    style ASequenceContainerNodeKey fill:#167034,color:#fff

    class TypeManaged
    style TypeManaged fill:#00A8A5,color:#000

    class Unit
    style Unit fill:#00A8A5,color:#000

    ReferencesHolder "1" --> "1" ReferencesHolderType
    AReferenceNode <|-- VariableNode
    AReferenceNode <|-- ConstantNode
    InNodePort "1" o--> "0..1" AReferenceNode: _inputNode
    InNodePort "1" o--> "0..1" AReferenceNode: _internalInputNode
    OutNodePort "1" o--> "0..1" AReferenceNode: _outputNode
    OutNodePort "1" o--> "0..1" AReferenceNode: _internalOutputNode
    ReferencesHolder "1" *--> "*" InNodePort: _inputs
    ReferencesHolder "1" *--> "*" OutNodePort: _outputs
    VariableNode ..> SequenceModuleNodeKey: uses
    VariableNode ..> NodesSequenceKey: uses
    VariableNode ..> ASequenceContainerNodeKey: uses
    AReferenceNode "1" --> "1" TypeManaged
    AReferenceNode "1" --> "1" Unit
```

#### Sequences nodes

```mermaid
classDiagram
    direction TB

    class ANode {
        -QString _instanceName
        -QString _description
        -NodeType _nodeType
        -QSharedPointer~NodesSequence~ _parent

        #~slot~ forgetAllAboutIt(QSharedPointer~ANode~ nodeToDetachWith)*

        +~~signal~ newLog(SequenceLogData newLog)
    }
    style ANode fill:#167034,color:#fff

    class ASequenceNode {
        -bool _isActive
        -QAbstractState* _state

        +canItBeTargeted() TargetType*
        +canTargetNode() TargetType*
        +addTransitionToThisNode(QState& state,\nASequenceNode &callerNode,\nTargetingSequenceNodesKey targetingKey,\nQAbstractTransition *transition) QAbstractTransition&*

        +removeTransitionWithThisNode(ASequenceNode &callerNode,\nTargetingSequenceNodesKey targetingKey) bool
        +changeParentStateIfDifferentWithWaterfallEffect(\nQState *parentStateToCompareWith,\nASequenceNode &callerNode,\nQPointer~QAbstractState~ modifiedTargetedState,\nTargetingSequenceNodesKey targetingKey) bool
        +isActive() bool

        #nodeActivity()*
        #changeParentStateWithWaterfallEffect(QState* parentState,\nASequenceNode &callerNode,\nQPointer<QAbstractState> &modifiedTargetedState) bool*

        +~signal~ exit()
        +~signal~ error(ANode guiltyNode, QString error)
    }
    style ASequenceNode fill:#167034,color:#fff

    class NodeType {
        <<enumeration>>
        NodesSequence
        InitialNode
        JoinNode
        MergeNode
        FinalNode
        SequenceModuleNode
        VariableNode
        ConstantNode
        ForkNode
        DecisionalNode
        GroupNode
        InclusionNode
        UnknownNode
    }
    style NodeType fill:#167034,color:#fff

    class ATargetingNode {
        -QAbstractTransition *_transition
        -QSharedPointer~ASequenceNode~ _targetedNode

        +addTransitionToNode(QSharedPointer~ASequenceNode~ targetingNode) bool
        +removeTransitionToNode() bool
    }
    style ATargetingNode fill:#167034,color:#fff

    class ASequenceContainerNode {
        -bool _failIfAnErrorOccurred

        +getChildNodesSequence() QSharedPointer~NodesSequence~*

        #preparingSequenceBeforeStart(QString &error) bool
        #forgetAllAboutMe()

        #~slot~ onChildNodesSequenceFinished(bool success)
    }
    style ASequenceContainerNode fill:#167034,color:#fff

    class GroupNode {
        -QSharedPointer~NodesSequence~ _nodesSequence

        +setNodesSequence(QSharedPointer~NodesSequence~ nodesSequence)
    }
    style GroupNode fill:#167034,color:#fff

    class InclusionNode {
        -QString _sequenceFileName
        -SequenceFile* _sequenceFile
        -NodesSequenceManager* _nodesSequenceManager

        +setAndLoadSequenceFile(QString filePath,\nSequenceLoadingErrorHandler* errorHandler) bool
    }
    style InclusionNode fill:#167034,color:#fff

    class DecisionalNode {
        -QSharedPointer~AReferenceNode~ _refNodeToTest
        -QSharedPointer~ASequenceNode~ _defaultNode
        -QAbstractTransition *_defaultTransition
        -QHash~QSharedPointer~ASequenceNode~#44; TransitionInfos~ _transitions

        +setReferenceNodeToTest(QSharedPointer~AReferenceNode~ refNodeToTest)
        +setDefaultNode(QSharedPointer~ASequenceNode~ defaultNode) bool
        +addTransitionToNode(QSharedPointer~ASequenceNode~ targetedNode,\nJsonObject transitionElem,\nSequenceLoadingErrorHandler *errorHandler) bool
        +getTransitions(JsonArray &transitionsElems) bool
        +getLinkedNodes() QVector~QSharedPointer~ASequenceNode~~
        +removeTransitionToNode(QSharedPointer~ASequenceNode~ targetedNode) bool
    }
    style DecisionalNode fill:#167034,color:#fff

    class ForkNode {
        -QHash~QSharedPointer~ASequenceNode~#44; ParallelChildState~ _transitions
        -QSharedPointer~JoinNode~ _joinNodeLinked
        -QAbstractTransition *_joinNodetransition

        +addTransitionToNode(QSharedPointer~ASequenceNode~ targetedNode) bool
        +removeTransitionToNode(QSharedPointer~ASequenceNode~ targetedNode) bool
        +setJoinNodeLinked(QSharedPointer~JoinNode~ joinNode,\nQSharedPointer~ForkNode~ thisForkNode) bool
        +getJoinNodeLinked() QSharedPointer~JoinNode~ joinNode
    }
    style ForkNode fill:#167034,color:#fff

    class JoinNode {
        -QSharedPointer~ForkNode~ _forkNode
        -QHash~ASequenceNode*, QFinalState*~ _finalNodes

        +setForkNodeLinked(QState &forkState,\nQSharedPointer~ForkNode~ forkNode,\nForkNodeKey forkNodeKey) QAbstractTransition*
        +removeForkNodeLinked() bool
    }
    style JoinNode fill:#167034,color:#fff

    class SequenceModuleNode {
        -QString _bankName
        -QString _moduleName
        -ReferencesHolder _referencesHolder
        -ISequenceInstanceModule* _sequenceItfModule
        -bool _enable

        +setSequenceBankIft(ISequenceInstanceModule *seqBankItf,\nSequenceLoadingErrorHandler *errorhandler) bool
    }
    style SequenceModuleNode fill:#167034,color:#fff

    class InitialNode
    style InitialNode fill:#167034,color:#fff

    class MergeNode
    style MergeNode fill:#167034,color:#fff

    class FinalNode
    style FinalNode fill:#167034,color:#fff

    class TransitionInfos {
        +TestResult testResult
        +ADecisionalTransition* transition
    }
    style TransitionInfos fill:#167034,color:#fff

    class TestResult {
        <<enumeration>>
        NotPerformed
        Success
        Failed
    }
    style TestResult fill:#167034,color:#fff

    class AReferenceNode {
    }
    style AReferenceNode fill:#167034,color:#fff

    class VariableNode {
    }
    style VariableNode fill:#167034,color:#fff

    class ConstantNode {
    }
    style ConstantNode fill:#167034,color:#fff

    class ParallelChildState {
        +QAbstractTransition *transition
        +QStateMachine *child
        +QState *initialChildOfChild
    }
    style ParallelChildState fill:#167034,color:#fff

    class TargetType {
        <<enumeration>>
        None
        OnlyOne
        Multiple
    }
    style TargetType fill:#167034,color:#fff

    class TargetingSequenceNodesKey
    style TargetingSequenceNodesKey fill:#167034,color:#fff

    class SequenceLogData
    style SequenceLogData fill:#00A8A5,color:#000

    class SequenceFile
    style SequenceFile fill:#167034,color:#fff

    class NodesSequenceManager
    style NodesSequenceManager fill:#167034,color:#fff

    class ReferencesHolder
    style ReferencesHolder fill:#167034,color:#fff

    class ISequenceInstanceModule
    style ISequenceInstanceModule fill:#747239,color:#fff

    ANode <|-- ASequenceNode
    ASequenceNode <|-- ForkNode
    ASequenceNode <|-- FinalNode
    ASequenceNode ..> TargetType: uses
    ASequenceNode ..> TargetingSequenceNodesKey: uses
    ASequenceNode <|-- ATargetingNode
    ASequenceNode <|-- DecisionalNode
    ATargetingNode "1" o--> "1" ASequenceNode : _targetedNode
    ATargetingNode <|-- JoinNode
    ATargetingNode <|-- ASequenceContainerNode
    ATargetingNode <|-- SequenceModuleNode
    ATargetingNode <|-- MergeNode
    ATargetingNode <|-- InitialNode
    ASequenceContainerNode <|-- GroupNode
    ASequenceContainerNode <|-- InclusionNode
    InclusionNode "1" *--> "1" SequenceFile : _sequenceFile
    InclusionNode "1" *--> "1" NodesSequenceManager : _nodesSequenceManager
    InclusionNode "1" *--> "1" NodesSequenceManager : _nodesSequenceManager
    ANode ..> SequenceLogData : uses
    ANode "1" *--> "1" NodeType : _nodeType
    ANode <|-- AReferenceNode
    AReferenceNode <|-- VariableNode
    AReferenceNode <|-- ConstantNode
    TransitionInfos ..> TestResult : uses
    DecisionalNode ..> TransitionInfos : uses
    DecisionalNode "1" --> "1" AReferenceNode : _refNodeToTest
    ForkNode "1" *--> "*" ParallelChildState
    ForkNode "0..1" -- "0..1" JoinNode
    SequenceModuleNode "1" *--> "1" ISequenceInstanceModule: _sequenceItfModule
    SequenceModuleNode "1" *--> "1" ReferencesHolder: _referencesHolder
```

#### Decisional transitions

```mermaid
classDiagram
    class DecisionalNode
    style DecisionalNode fill:#167034,color:#fff

    class ADecisionalTransition {
        -QVariant _valueToCompareWith
        -DecisionalOperator _decisionalOperator
        -bool _masteredByParent

        +isTestingEquality() bool
        +isLogicalOperator() bool

        +setValueToCompareWith(\nQVariant valueToCompareWith) bool*
        +testValue(QVariant value) bool*
    }
    style ADecisionalTransition fill:#167034,color:#fff

    class ALogicalOperatorDecisionalTransition {
        -QVector~ADecisionalTransition*~ _transitions
    }
    style ALogicalOperatorDecisionalTransition fill:#167034,color:#fff

    class AndDecisionalTransition
    style AndDecisionalTransition fill:#167034,color:#fff

    class EqualityDecisionalTransition
    style EqualityDecisionalTransition fill:#167034,color:#fff

    class GreaterThanDecisionalTransition
    style GreaterThanDecisionalTransition fill:#167034,color:#fff

    class LesserThanDecisionalTransition
    style LesserThanDecisionalTransition fill:#167034,color:#fff

    class OrDecisionalTransition
    style OrDecisionalTransition fill:#167034,color:#fff

    class DecisionalOperator {
        EqualTo
        NotEqualTo
        GreaterThan
        GreaterOrEqualTo
        LesserThan
        LesserOrEqualTo
        And
        Or
        Unknown
    }
    style DecisionalOperator fill:#167034,color:#fff

    class DecisionalEventValue {
        -QVariant _value
        -bool _triggered
    }
    style DecisionalEventValue fill:#167034,color:#fff

    class QAbstractTransition
    class QEvent

    QAbstractTransition <|-- ADecisionalTransition
    ADecisionalTransition <|-- ALogicalOperatorDecisionalTransition
    ALogicalOperatorDecisionalTransition <|-- AndDecisionalTransition
    ALogicalOperatorDecisionalTransition <|-- EqualityDecisionalTransition
    ALogicalOperatorDecisionalTransition <|-- GreaterThanDecisionalTransition
    ALogicalOperatorDecisionalTransition <|-- LesserThanDecisionalTransition
    ALogicalOperatorDecisionalTransition <|-- OrDecisionalTransition
    QEvent <|-- DecisionalEventValue
    ADecisionalTransition ..> DecisionalEventValue: uses
    ADecisionalTransition ..> DecisionalOperator: uses
    DecisionalNode ..> ADecisionalTransition: uses
```
