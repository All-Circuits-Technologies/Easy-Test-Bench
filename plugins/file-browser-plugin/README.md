<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# File Browser plugin

## Table of contents

- [File Browser plugin](#file-browser-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [FileBrowerModule](#filebrowermodule)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)
    - [PermStoreFileBrowserModule](#permstorefilebrowsermodule)
      - [Presentation](#presentation-2)
      - [Schematic representation](#schematic-representation-1)
    - [ProjectCacheBrowserModule](#projectcachebrowsermodule)
      - [Presentation](#presentation-3)
      - [Schematic representation](#schematic-representation-2)

## Presentation

This plugin contains modules for displaying a file browser to user.

## Modules list

### FileBrowerModule

#### Presentation

Simple module to browse file or folder (without getting or saving anything).

#### Schematic representation

```mermaid
flowchart LR
    subgraph "FileBrowerModule"
        direction LR
        subgraph "Inputs"
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[selectDir]
                direction TB
                Param1Info["Type: boolean\nRequired: false\nDefault: false\nDesc: True if you want to select a directory, instead of a file"]
            end
            subgraph Param2[fileExtensions]
                direction TB
                Param2Info["Type: stringList\nRequired: false\nDesc: The file extensions filter, the format has to be like this: Text (*.txt)"]
            end
            subgraph Param3[fileDialogDirectory]
                direction TB
                Param3Info["Type: string\nRequired: false\nDesc: If given, the file dialog will open on this directory (if no previous value has been given)"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[filePathSelected]
                direction TB
                Output1Info["Type: string\nDesc: The file path selected"]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### PermStoreFileBrowserModule

#### Presentation

Useful module to browse file and store the value got in the permanent storage.
The module memorizes in permanent storage the path chosen, and will reload it at start.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "PermStoreFileBrowserModule"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[permStorageKey]
                direction TB
                Input1Info["Type: string\nDesc: The key of permanent storage to store the selected file"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[selectDir]
                direction TB
                Param1Info["Type: boolean\nRequired: false\nDefault: false\nDesc: True if you want to select a directory, instead of a file"]
            end
            subgraph Param2[fileExtensions]
                direction TB
                Param2Info["Type: stringList\nRequired: false\nDesc: The file extensions filter, the format has to be like this: Text (*.txt)"]
            end
            subgraph Param3[fileDialogDirectory]
                direction TB
                Param3Info["Type: string\nRequired: false\nDesc: If given, the file dialog will open on this directory (if no previous value has been given)"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[filePathSelected]
                direction TB
                Output1Info["Type: string\nDesc: The file path selected"]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### ProjectCacheBrowserModule

#### Presentation

Useful module to browse file and store the value got in the project cache.
he module memorizes in project cache the path chosen, and will reload it at start.
The project cache is only alive until the user closes the current project or close the apps.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "ProjectCacheBrowserModule"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[projectCacheKey]
                direction TB
                Input1Info["Type: string\nDesc: The key of project cache to store the selected file"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[selectDir]
                direction TB
                Param1Info["Type: boolean\nRequired: false\nDefault: false\nDesc: True if you want to select a directory, instead of a file"]
            end
            subgraph Param2[fileExtensions]
                direction TB
                Param2Info["Type: stringList\nRequired: false\nDesc: The file extensions filter, the format has to be like this: Text (*.txt)"]
            end
            subgraph Param3[fileDialogDirectory]
                direction TB
                Param3Info["Type: string\nRequired: false\nDesc: If given, the file dialog will open on this directory (if no previous value has been given)"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[filePathSelected]
                direction TB
                Output1Info["Type: string\nDesc: The file path selected"]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
