<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Scan Data Matrix plugin

## Table of contents

- [Scan Data Matrix plugin](#scan-data-matrix-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [ScanDataMatrix](#scandatamatrix)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)

## Presentation

This plugin contains modules to ask user to scan data matrix.

## Modules list

### ScanDataMatrix

#### Presentation

The module asks the user to scan a datamatrix and returns the string returned by the widget.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "ScanDataMatrix"
        direction LR
        subgraph "Inputs"
        end
        subgraph "Parameters"
            subgraph Param1[imagePath]
                direction TB
                Param1Info["Type: string\nRequired: false\nDefault: :/scan.jpg\nDesc: This is the image path to display."]
            end
            subgraph Param2[hmiDetectDesc]
                direction TB
                Param2Info["Type: string\nRequired: false\nDefault: (TR) Waiting the scan of barecode\nDesc: Hmi text instruction for input view."]
            end
            subgraph Param3[regexPattern]
                direction TB
                Param3Info["Type: string\nRequired: false\nPattern regex: ^(?>(?>(?>(?>#92;(#92;?>)|[^(#92;r#92;n])*#92;((?>(?>#92;(#92;?>)|[^(#92;r#92;n])*#92;))|(?>(?>#92;(#92;?>)|[^(#92;r#92;n]))*$\nDesc: This is the pattern used to validate the entered input\ninformation by user. If the regexp contains captured\nbrackets, the returned will be a concatenated result of the\ncaptured elements."]
            end
            subgraph Param4[clearFieldIfError]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If set to true and if the input isn't correctly filled (checked\nby regex), when the user enters new information if will first\nerase the data already contained in the input."]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[data]
                direction TB
                Output1Info["Type: string\nDesc: The string data read from data matrix."]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
