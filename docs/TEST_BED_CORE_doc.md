<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Test bed core documentation

## Table of contents

- [Test bed core documentation](#test-bed-core-documentation)
  - [Table of contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Class diagram](#class-diagram)
    - [Errors](#errors)
    - [Logs](#logs)
    - [Parsers](#parsers)
    - [Types](#types)
    - [Utility](#utility)
    - [Others](#others)

## Introduction

This file contains the definition of the interface between the plugins and test bed lib

## Class diagram

### Errors

```mermaid
classDiagram
    class FileLoadingErrorHelper {
        #addErrorDescriptor(ErrorLayer layer,\nint error,\nFileLoadingErrorHandler& errorHandler,\nQVector~QVariant~ errorParameters)$
    }
    style FileLoadingErrorHelper fill:#00A8A5,color:#000

    class FileLoadingErrorCauseHelper
    style FileLoadingErrorCauseHelper fill:#00A8A5,color:#000

    class FileLoadingErrorTypeHelper
    style FileLoadingErrorTypeHelper fill:#00A8A5,color:#000

    class FileLoadingErrorHandler {
        -QVector~ErrorDescriptor~ _errorDescriptor
        -bool _missingProjectFileError

        +getMessages() QStringList*
        +addErrorDescriptor(ErrorDescriptor errorDescriptor)
        +clearErrors()
    }
    style FileLoadingErrorHandler fill:#00A8A5,color:#000

    class ErrorLayer {
        <<enumeration>>
        ErrorType
        ErrorCause
        FailedNodesSequence
        FailedChildNode
    }
    style ErrorLayer fill:#00A8A5,color:#000

    class ErrorCause {
        <<enumeration>>
    }
    style ErrorCause fill:#00A8A5,color:#000

    class ErrorType {
        <<enumeration>>
        MissingProjectFile
        MissingSequenceFile
        InaccessibleFile
        UnreadableSequence
        UnreadableProject
        UnsupportedFileFormat
    }
    style ErrorType fill:#00A8A5,color:#000

    class ErrorDescriptor {
        +ErrorLayer layer
        +int error
        +QVector~QVariant~ parameters
    }
    style ErrorDescriptor fill:#00A8A5,color:#000

    FileLoadingErrorHelper ..> ErrorLayer : uses
    FileLoadingErrorHelper ..> FileLoadingErrorHandler : uses
    FileLoadingErrorHelper <|-- FileLoadingErrorCauseHelper
    FileLoadingErrorHelper <|-- FileLoadingErrorTypeHelper
    FileLoadingErrorHandler "1" *--> "*" ErrorDescriptor
    FileLoadingErrorCauseHelper ..> ErrorCause : uses
    FileLoadingErrorTypeHelper ..> ErrorType : uses
    ErrorDescriptor  ..> ErrorLayer : uses
```

### Logs

```mermaid
classDiagram
    class SequenceLogCategory {
        <<enumeration>>
        BusinessScope
        ApplicationScope
        Channel1
        Channel2
    }
    style SequenceLogCategory fill:#00A8A5,color:#000

    class SequenceLogData {
        -qint64 _timestampMSecsSinceEpoch
        -LogMsgType _type
        -SequenceLogCategory _category
        -QString _text
    }
    style SequenceLogData fill:#00A8A5,color:#000

    class SequenceLogFormatter
    style SequenceLogFormatter fill:#00A8A5,color:#000

    class SequenceLogFormatType {
        DisplayAll
        OnlyDisplayMsg
        Unknown
    }
    style SequenceLogFormatType fill:#00A8A5,color:#000

    SequenceLogFormatter ..> SequenceLogFormatType : uses
    SequenceLogFormatter ..> SequenceLogData : uses
    SequenceLogData "1" --> "1" SequenceLogCategory
```

### Parsers

```mermaid
classDiagram
    class BankJsonFormatter
    style BankJsonFormatter fill:#00A8A5,color:#000

    class BankJsonParamValidator
    style BankJsonParamValidator fill:#00A8A5,color:#000

    class BankJsonParser
    style BankJsonParser fill:#00A8A5,color:#000

    class CommonJsonParser
    style CommonJsonParser fill:#00A8A5,color:#000

    class JsonParsersConstants
    style JsonParsersConstants fill:#00A8A5,color:#000

    BankJsonFormatter ..> BankJsonParser : uses
    BankJsonFormatter ..> JsonParsersConstants : uses
    BankJsonParamValidator ..> BankJsonParser : uses
    BankJsonParser ..> CommonJsonParser
    BankJsonParser ..> JsonParsersConstants
```

### Types

```mermaid
classDiagram
    class SequenceStateType {
        <<enumeration>>
        Running
        InPause
        Stopped
        Unknown
    }
    style SequenceStateType fill:#00A8A5,color:#000

    class StopAskedType {
        <<enumeration>>
        AskedByUser
        EndOfProcess
        NoStopAsked
    }
    style StopAskedType fill:#00A8A5,color:#000

    class TypeManaged {
        <<enumeration>>
        Boolean
        String
        Number
        BooleanList
        StringList
        NumberList
    }
    style TypeManaged fill:#00A8A5,color:#000

    class Unit {
        -UnitType _unitType
        -qint8 _powerOfTen
        -QString _specificSymbolType
    }
    style Unit fill:#00A8A5,color:#000

    class UnitType {
        <<enumeration>>
        Ampere
        Volt
        Watt
        CelciusDegree
        Ohm
        Second
        Hertz
        Adc
        Lumen
        Farad
        Henry
        Kelvin
        BitPerSecond
        Other
        None
    }
    style UnitType fill:#00A8A5,color:#000

    Unit "1" --> "1" UnitType
```

### Utility

```mermaid
classDiagram
    class PauseAndStopHelper
    style PauseAndStopHelper fill:#00A8A5,color:#000

    class SequenceFilesHelper
    style SequenceFilesHelper fill:#00A8A5,color:#000

    class SequenceStateType {
        <<enumeration>>
    }
    style SequenceStateType fill:#00A8A5,color:#000

    class StopAskedType {
        <<enumeration>>
    }
    style StopAskedType fill:#00A8A5,color:#000

    class SequenceStateHelper {
        -SequenceStateType _currentState
        -StopAskedType _stopAskedType
        -bool _pauseAsked

        +isPauseAsked() bool
        +isRunning() bool
        +isStopped() bool

        +~slot~ onRunning()
        +~slot~ onPauseOrResume(bool setToPause)
        +~slot~ onStopAsked(StopAskedType stopAskedType)
        +~slot~ onStop()

        +~signal~ newState(SequenceStateType newState,\nSequenceStateType previousState)
        +~signal~ stopAsked(StopAskedType stopAskedType)
    }
    style SequenceStateHelper fill:#00A8A5,color:#000

    SequenceStateHelper "1" --> "1" SequenceStateType
    SequenceStateHelper "1" --> "1" StopAskedType
```

### Others

```mermaid
classDiagram
    class CustomEvents {
        <<enumeration>>
        DecisionalEventValue
    }
    style CustomEvents fill:#00A8A5,color:#000
```
