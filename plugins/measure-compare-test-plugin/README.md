<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Measure Compare Test plugin

## Table of contents

- [Measure Compare Test plugin](#measure-compare-test-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [MeasureInputsBoundsTest](#measureinputsboundstest)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)
    - [MeasureParamsBoundsTest](#measureparamsboundstest)
      - [Presentation](#presentation-2)
      - [Schematic representation](#schematic-representation-1)

## Presentation

This plugin contains modules which allows to compare values and displays logs depending of the
compare test result.

## Modules list

### MeasureInputsBoundsTest

#### Presentation

Test if the given measured value is contained in the boundaries set.

The boundaries values are got from inputs.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "MeasureInputsBoundsTest"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[measureDone]
                direction TB
                Input1Info["Type: number\nDesc: This is the measure to test with boundaries."]
            end
            subgraph Input2[infBound]
                direction TB
                Input2Info["Type: number\nDesc: This is the inferior boundary for testing the measure given."]
            end
            subgraph Input3[supBound]
                direction TB
                Input3Info["Type: number\nDesc: This is the superior boundary for testing the measure given."]
            end
            Input1 ~~~ Input2
            Input2 ~~~ Input3
        end
        subgraph "Parameters"
            subgraph Param1[elementTested]
                direction TB
                Param1Info["Type: string\nRequired: true\nDesc: Description of the element tested."]
            end
            subgraph Param2[testDescription]
                direction TB
                Param2Info["Type: string\nRequired: false\nDesc: The description of the test."]
            end
            subgraph Param3[fatalError]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If the test fails, this param says if we have to stop the sequence or not."]
            end
            subgraph Param4[isInfBoundaryIncluded]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If true, the inferior boundary given is included in\ntest, same as: inf <= x. If false, the inferior\nboundary isn't included in test, same as: inf < x."]
            end
            subgraph Param5[isSupBoundaryIncluded]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If true, the superior boundary given is included in\ntest, same as: x <= sup. If false, the superior\nboundary isn't included in test, same as: x < supr."]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[success]
                direction TB
                Output1Info["Type: boolean\nDesc: Tell if the test has been successful."]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### MeasureParamsBoundsTest

#### Presentation

Test if the given measured value is contained in the boundaries set.

The boundaries values are got from parameters.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "MeasureParamsBoundsTest"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[measureDone]
                direction TB
                Input1Info["Type: number\nDesc: This is the measure to test with boundaries."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[elementTested]
                direction TB
                Param1Info["Type: string\nRequired: true\nDesc: Description of the element tested."]
            end
            subgraph Param2[testDescription]
                direction TB
                Param2Info["Type: string\nRequired: false\nDesc: The description of the test."]
            end
            subgraph Param3[fatalError]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If the test fails, this param says if we have to stop the sequence or not."]
            end
            subgraph Param4[isInfBoundaryIncluded]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If true, the inferior boundary given is included in\ntest, same as: inf <= x. If false, the inferior\nboundary isn't included in test, same as: inf < x."]
            end
            subgraph Param5[isSupBoundaryIncluded]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: true\nDesc: If true, the superior boundary given is included in\ntest, same as: x <= sup. If false, the superior\nboundary isn't included in test, same as: x < supr."]
            end
            subgraph Param6[infBound]
                direction TB
                Param6Info["Type: number\nRequired: true\nDesc: This is the inferior boundary for testing the measure given."]
            end
            subgraph Param7[supBound]
                direction TB
                Param7Info["Type: number\nRequired: true\nDesc: This is the superior boundary for testing the measure given."]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
        end
        subgraph "Outputs"
            direction TB
            subgraph Output1[success]
                direction TB
                Output1Info["Type: boolean\nDesc: Tell if the test has been successful."]
            end
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
