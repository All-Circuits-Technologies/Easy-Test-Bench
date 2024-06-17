<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Analyse results plugin

## Table of contents

- [Analyse results plugin](#analyse-results-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [AnalyseResults](#analyseresults)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)
    - [DisplayErrorResults](#displayerrorresults)
      - [Presentation](#presentation-2)
      - [Schematic representation](#schematic-representation-1)

## Presentation

This plugin contains modules used to give meanings on results.

## Modules list

### AnalyseResults

#### Presentation

The module says if an error occured because a module failed or a business error log occured (a log
with warning level).

This module is used to analyse the result of a group of modules, two scenarios are tested. But for
all, the last sequence logs (number of logs to get is given by a parameter) are retrieved:
- If the "inputSequenceSuccess" input value is equals to "True", the module will get the business
  sequence logs in errors, if at least one is an error we detect that an error occured
- If the "inputSequenceSuccess" input value is equals to "False", the module will get the business
  sequence logs in errors

#### Schematic representation

```mermaid
flowchart LR
    subgraph "AnalyseResults"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputSequenceSuccess]
                direction TB
                Input1Info["Type: boolean\nDesc: Current sequence state"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[tailErrorLogsNb]
                direction TB
                Param1Info["Type: number\nRequired: true\nMin: -1; max: maxInt32; step: 1\nDesc: Number of error logs to get. -1 to get all"]
            end
        end
        subgraph "Outputs"
            direction TB
            subgraph "outputSequenceSuccess"
                direction TB
                Output1Info["Type: boolean\nDesc: Result of the sequence success after analyse"]
            end
            subgraph "outputLastErrorLogs"
                direction TB
                Output2Info["Type: string[]\nDesc: Last error logs"]
            end
            outputSequenceSuccess ~~~ outputLastErrorLogs
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### DisplayErrorResults

#### Presentation

Display the last error results and wait the user to remove the board from the test bed.

We wait the user to explicitly remove its board from the test bench.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "DisplayErrorResults"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputErrorsToDisplay]
                direction TB
                Input1Info["Type: string[]\nDesc: The errors to display in HMI"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[errorsNbToDisplayParamName]
                direction TB
                Param1Info["Type: number\nRequired: true\nMin: 0; max: maxInt32; step: 1\nDesc: Number of error logs to display"]
            end
            subgraph Param2[baseImagePathName]
                direction TB
                Param2Info["Type: string\nRequired: true\nDesc: The path of the image to display with GUI"]
            end
            Param1 ~~~ Param2
        end
        subgraph "Outputs"
            direction TB
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
