<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Read Setting plugin

## Table of contents

- [Read Setting plugin](#read-setting-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [ReadStrSettingModule](#readstrsettingmodule)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)

## Presentation

This plugin contains modules used to read the test bed library settings.

## Modules list

### ReadStrSettingModule

#### Presentation

The module reads a string setting from the test bed lib, thanks to the key given.

The key has to be known by the test bed lib or an error is raised.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "ReadStrSettingModule"
        direction LR
        subgraph "Inputs"
        end
        subgraph "Parameters"
            subgraph Param1[exitIfFail]
                direction TB
                Param1Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and an error occurs in the module,\nthe module will fail."]
            end
            subgraph Param2[settingKey]
                direction TB
                Param2Info["Type: string\nRequired: true\nDesc: The setting key to get the value wanted."]
            end
            Param1 ~~~ Param2
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[successOutput]
                direction TB
                Output1Info["Type: boolean\nDesc: This output is updated with the result of the module action."]
            end
            subgraph Output2[settingValueOutput]
                direction TB
                Output2Info["Type: string\nDesc: The setting output value."]
            end
            Output1 ~~~ Output2
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
