<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Atmel program plugin

## Table of contents

- [Atmel program plugin](#atmel-program-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [DetectBoardAtProgram](#detectboardatprogram)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)
    - [ProgramAtProgram](#programatprogram)
      - [Presentation](#presentation-2)
      - [Schematic representation](#schematic-representation-1)
    - [ReadEepromAtProgram](#readeepromatprogram)
      - [Presentation](#presentation-3)
      - [Schematic representation](#schematic-representation-2)

## Presentation

This plugin contains modules to communicate with an atmel probe.

## Modules list

### DetectBoardAtProgram

#### Presentation

This module is used to detect the tested board through the atmel probe (or detect if none is
present).

The module also displays a pop-up in the application to inform the user of what he has to do:
- if we want to detect the board, we ask the user to put the board in test bench,
- if we want to not detect the board, we ask the user to take the board out the test bench.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "DetectBoardAtProgram"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[atProgramExe]
                direction TB
                Input1Info["Type: string\nDesc: The atprogram absolute executable path"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[detectPresence]
                direction TB
                Param1Info["Type: bool\nRequired: true\nDefault: true\nDesc: If true, the module waits to detect a bord.\nIf false, the module waits to be no more connected to a board."]
            end
            subgraph Param2[baseImagePathName]
                direction TB
                Param2Info["Type: string\nRequired: true\nDesc: The path of the image to display with GUI"]
            end
            subgraph Param3[baseImagePathName]
                direction TB
                Param3Info["Type: string\nRequired: true\nDesc: The path of the image to display with GUI"]
            end
            subgraph Param4[hmiDetectDesc]
                direction TB
                Param4Info["Type: string\nRequired: false\nDefault: (TR) Put the board on the test bed.\nDesc: Hmi text instruction if wait board detection"]
            end
            subgraph Param5[hmiNoDetectDesc]
                direction TB
                Param5Info["Type: string\nRequired: false\nDefault: (TR) Take the board out of the test bed.\nDesc: Hmi text instruction if wait no board detection"]
            end
            subgraph Param6[manageBoardOnBenchProblem]
                direction TB
                Param6Info["Type: string\nRequired: false\nDefault: false\nDesc: If enabled, it means that the board may not be detected\nbecause an electronic element is not working on the board."]
            end
            subgraph Param7[boardOnBenchLogError]
                direction TB
                Param7Info["Type: string\nRequired: false\nDefault: A component is not working on the board.\nDesc: This is the detail of error which will be displayed in\nlogs when operator click on the board on bench error button."]
            end
            subgraph Param8[detectTimeoutInMs]
                direction TB
                Param8Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: Timeout in Ms; set -1 to disable the timeout"]
            end
            subgraph Param9[waitBetweenTriesInMs]
                direction TB
                Param9Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: 1000\nDesc: This is the time to wait in milliseconds between tries"]
            end
            subgraph Param10[waitForDisplayHmiInMs]
                direction TB
                Param10Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: This is the time to wait in milliseconds between displaying the HMI.\nUse this when it's mostly likely to have a compliant test right now."]
            end
            subgraph Param11[chipUsedName]
                direction TB
                Param11Info["Type: string\nRequired: true\nDesc: The name of the chip targetted by the action"]
            end
            subgraph Param12[physicalInterface]
                direction TB
                Param12Info["Type: string\nRequired: true\nDesc: The physical interface used to communicate with the board"]
            end
            subgraph Param13[timeoutInMs]
                direction TB
                Param13Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: 1500\nDesc: Program timeout in Ms"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
            Param7 ~~~ Param8
            Param8 ~~~ Param9
            Param9 ~~~ Param10
            Param10 ~~~ Param11
            Param11 ~~~ Param12
            Param12 ~~~ Param13
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### ProgramAtProgram

#### Presentation

Program the microprocessor memory with a hex file through the atmel probe.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "ProgramAtProgram"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[atProgramExe]
                direction TB
                Input1Info["Type: string\nDesc: The atprogram absolute executable path"]
            end
            subgraph Input2[hexFilePath]
                direction TB
                Input2Info["Type: string\nDesc: Path to the Intel hex file which contains the binaries to write"]
            end
            Input1 ~~~ Input2
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[chipUsedName]
                direction TB
                Param1Info["Type: string\nRequired: true\nDesc: The name of the chip targetted by the action"]
            end
            subgraph Param2[physicalInterface]
                direction TB
                Param2Info["Type: string\nRequired: true\nDesc: The physical interface used to communicate with the board"]
            end
            subgraph Param3[timeoutInMs]
                direction TB
                Param3Info["Type: number\nRequired: true\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: 1500\nDesc: Program timeout in Ms"]
            end
            subgraph Param4[verifyAfterProgram]
                direction TB
                Param4Info["Type: bool\nRequired: false\nDefault: true\nDesc: The module will verify the memory after program"]
            end
            subgraph Param5[deleteFileAfterProgram]
                direction TB
                Param5Info["Type: bool\nRequired: false\nDefault: false\nDesc: Delete the input file after programming the memory"]
            end
            subgraph Param6[memoryTypeParamName]
                direction TB
                Param6Info["Type: select\nRequired: true\nOptions: ['Flash', 'Eeprom']\nDesc: Allow to choose the memory type to manage"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### ReadEepromAtProgram

#### Presentation

Read the microprocessor EEPROM and write the result to a linked file.

At each call of process module, the instance removes the previous read file linked (if it exists).
When the instance is destroyed the read file linked is destroyed

#### Schematic representation

```mermaid
flowchart LR
    subgraph "ReadEepromAtProgram"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[atProgramExe]
                direction TB
                Input1Info["Type: string\nDesc: The atprogram absolute executable path"]
            end
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[chipUsedName]
                direction TB
                Param1Info["Type: string\nRequired: true\nDesc: The name of the chip targetted by the action"]
            end
            subgraph Param2[physicalInterface]
                direction TB
                Param2Info["Type: string\nRequired: true\nDesc: The physical interface used to communicate with the board"]
            end
            subgraph Param3[timeoutInMs]
                direction TB
                Param3Info["Type: number\nRequired: true\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: 1500\nDesc: Program timeout in Ms"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[hexFilePath]
                direction TB
                Output1Info["Type: string\nDesc: Path to the Intel hex file which contains the binaries read"]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
