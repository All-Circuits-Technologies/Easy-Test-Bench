<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Peak Can plugin

## Table of contents

- [Peak Can plugin](#peak-can-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [WriteCanMsg](#writecanmsg)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)
    - [MultipleWriteCanMsgs](#multiplewritecanmsgs)
      - [Presentation](#presentation-2)
      - [Schematic representation](#schematic-representation-1)
    - [OnlyReadCanMsg](#onlyreadcanmsg)
      - [Presentation](#presentation-3)
      - [Schematic representation](#schematic-representation-2)
    - [MultipleOnlyReadCanMsgs](#multipleonlyreadcanmsgs)
      - [Presentation](#presentation-4)
      - [Schematic representation](#schematic-representation-3)
    - [CompareCanMsg](#comparecanmsg)
      - [Presentation](#presentation-5)
      - [Schematic representation](#schematic-representation-4)
    - [StartAutoAnswer](#startautoanswer)
      - [Presentation](#presentation-6)
      - [Schematic representation](#schematic-representation-5)
    - [StopAutoAnswer](#stopautoanswer)
      - [Presentation](#presentation-7)
      - [Schematic representation](#schematic-representation-6)

## Presentation

This plugin contains modules to communicate in CAN with a Peak probe.

## Modules list

### WriteCanMsg

#### Presentation

This module sends a CAN message and may expect an answer.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "WriteCanMsg"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[testGroupName]
                direction TB
                Param1Info["Type: string\nRequired: false\nDesc: The name of the test groups, which will be displayed in logs files."]
            end
            subgraph Param2[expectingMsgTimeoutInMs]
                direction TB
                Param2Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: The timeout linked to the expected msg(s).\n-1 if we wait indefinitely."]
            end
            subgraph Param3[exitIfFail]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and a message\nverification failed in the module, it will fail."]
            end
            subgraph Param4[bitrateSwitch]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: Activate, or not, the bitrate switch. This is only relevant when using CAN FD."]
            end
            subgraph Param5[extendedFrameFormat]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: false\nDesc: Activate, or not, the extended frame format for the message sent."]
            end
            subgraph Param6[expectedMsgId]
                direction TB
                Param6Info["Type: number\nRequired: false\nMin: -1; max: 0x3FFFFFFF; step: 1\nDefault: -1\nDesc: The ID of the expected message, if equals to -1, no answer is expected."]
            end
            subgraph Param7[expectedMsgVerifs]
                direction TB
                Param7Info["Type: string\nRequired: false\nDesc: This contains the elements to verify in the message received.\nThe info are stored in a string with the following format:\n'MASK_IDX/MASK/MASK_RESULT', if the payload of the message\nmatches the mask and its result, everything is right.\nIt's possible to add a boolean to wait the right value before\nreturning. To do so, you have to append the following\ninformation with the format:\n'MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED'\nIt's possible to add logs to the test, to do so, you have\nto append the following information with the format:\n'MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED/TEST_NAME/LOG_IF_SUCCESS/LOG_IF_FAIL'"]
            end
            subgraph Param8[canMsgToSend]
                direction TB
                Param8Info["Type: string\nRequired: false\nDesc: The CAN MSG to send. The message is stored in a string with the\nfollowing format: 'CAN_ID/PAYLOAD_LENGTH/PAYLOAD'"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
            Param7 ~~~ Param8
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### MultipleWriteCanMsgs

#### Presentation

This module sends sequentially multiple CAN messages and may expect answers for each one.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "MultipleWriteCanMsgs"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[testGroupName]
                direction TB
                Param1Info["Type: string\nRequired: false\nDesc: The name of the test groups, which will be displayed in logs files."]
            end
            subgraph Param2[expectingMsgTimeoutInMs]
                direction TB
                Param2Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: The timeout linked to the expected msg(s).\n-1 if we wait indefinitely."]
            end
            subgraph Param3[exitIfFail]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and a message\nverification failed in the module, it will fail."]
            end
            subgraph Param4[bitrateSwitch]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: Activate, or not, the bitrate switch. This is only relevant when using CAN FD."]
            end
            subgraph Param5[extendedFrameFormat]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: false\nDesc: Activate, or not, the extended frame format for the message sent."]
            end
            subgraph Param6[expectedMsgsIds]
                direction TB
                Param6Info["Type: number[]\nRequired: false\nMin: -1; max: 0x3FFFFFFF; step: 1\nDefault: -1\nDesc: The ID list of the expected messages, if one message id is\nequals to -1, it means that no message is expected."]
            end
            subgraph Param7[expectedMsgsVerifs]
                direction TB
                Param7Info["Type: string[]\nRequired: false\nDesc: This contains the elements to verify in the received\nmessages. The info are stored in a string with the following\nformat: 'MSG_IDX/MASK_IDX/MASK/MASK_RESULT',\nif the payload of the message matches the mask and its\nresult, everything is right. The message index matches the\nexpected messages list index and it says that the\nverification has to be applied on the message received for\nwith the given index.\nIt's possible to add a boolean to wait the right value before\nreturning. To do so, you have to append the following\ninformation with the format:\n'MSG_IDX/MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED'\nIt's possible to add logs to the test, to do so, you have\nto append the following information with the format:\n'MSG_IDX/MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED/TEST_NAME/LOG_IF_SUCCESS/LOG_IF_FAIL'"]
            end
            subgraph Param8[canMsgsToSend]
                direction TB
                Param8Info["Type: string[]\nRequired: false\nDesc: The CAN messages to send. The messages are stored in a string\nwith the following format: 'CAN_ID/PAYLOAD_LENGTH/PAYLOAD'"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
            Param7 ~~~ Param8
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### OnlyReadCanMsg

#### Presentation

This module only waits for an expected CAN message.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "OnlyReadCanMsg"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[testGroupName]
                direction TB
                Param1Info["Type: string\nRequired: false\nDesc: The name of the test groups, which will be displayed in logs files."]
            end
            subgraph Param2[expectingMsgTimeoutInMs]
                direction TB
                Param2Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: The timeout linked to the expected msg(s).\n-1 if we wait indefinitely."]
            end
            subgraph Param3[exitIfFail]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and a message\nverification failed in the module, it will fail."]
            end
            subgraph Param4[bitrateSwitch]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: Activate, or not, the bitrate switch. This is only relevant when using CAN FD."]
            end
            subgraph Param5[extendedFrameFormat]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: false\nDesc: Activate, or not, the extended frame format for the message sent."]
            end
            subgraph Param6[expectedMsgId]
                direction TB
                Param6Info["Type: number\nRequired: false\nMin: -1; max: 0x3FFFFFFF; step: 1\nDefault: -1\nDesc: The ID of the expected message, if equals to -1, no answer is expected."]
            end
            subgraph Param7[expectedMsgVerifs]
                direction TB
                Param7Info["Type: string\nRequired: false\nDesc: This contains the elements to verify in the message received.\nThe info are stored in a string with the following format:\n'MASK_IDX/MASK/MASK_RESULT', if the payload of the message\nmatches the mask and its result, everything is right.\nIt's possible to add a boolean to wait the right value before\nreturning. To do so, you have to append the following\ninformation with the format:\n'MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED'\nIt's possible to add logs to the test, to do so, you have\nto append the following information with the format:\n'MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED/TEST_NAME/LOG_IF_SUCCESS/LOG_IF_FAIL'"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### MultipleOnlyReadCanMsgs

#### Presentation

This module waits for expected CAN messages.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "MultipleOnlyReadCanMsgs"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[testGroupName]
                direction TB
                Param1Info["Type: string\nRequired: false\nDesc: The name of the test groups, which will be displayed in logs files."]
            end
            subgraph Param2[expectingMsgTimeoutInMs]
                direction TB
                Param2Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: The timeout linked to the expected msg(s).\n-1 if we wait indefinitely."]
            end
            subgraph Param3[exitIfFail]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and a message\nverification failed in the module, it will fail."]
            end
            subgraph Param4[bitrateSwitch]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: Activate, or not, the bitrate switch. This is only relevant when using CAN FD."]
            end
            subgraph Param5[extendedFrameFormat]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: false\nDesc: Activate, or not, the extended frame format for the message sent."]
            end
            subgraph Param6[expectedMsgsIds]
                direction TB
                Param6Info["Type: number[]\nRequired: false\nMin: -1; max: 0x3FFFFFFF; step: 1\nDefault: -1\nDesc: The ID list of the expected messages, if one message id is\nequals to -1, it means that no message is expected."]
            end
            subgraph Param7[expectedMsgsVerifs]
                direction TB
                Param7Info["Type: string[]\nRequired: false\nDesc: This contains the elements to verify in the received\nmessages. The info are stored in a string with the following\nformat: 'MSG_IDX/MASK_IDX/MASK/MASK_RESULT',\nif the payload of the message matches the mask and its\nresult, everything is right. The message index matches the\nexpected messages list index and it says that the\nverification has to be applied on the message received for\nwith the given index.\nIt's possible to add a boolean to wait the right value before\nreturning. To do so, you have to append the following\ninformation with the format:\n'MSG_IDX/MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED'\nIt's possible to add logs to the test, to do so, you have\nto append the following information with the format:\n'MSG_IDX/MASK_IDX/MASK/MASK_RESULT/WAIT_UNTIL_RECEIVED/TEST_NAME/LOG_IF_SUCCESS/LOG_IF_FAIL'"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### CompareCanMsg

#### Presentation

This module sends a CAN message and extract a value from the expected answer received. The value
extracted is tested and logs are emitted.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "CompareCanMsg"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[testGroupName]
                direction TB
                Param1Info["Type: string\nRequired: false\nDesc: The name of the test groups, which will be displayed in logs files."]
            end
            subgraph Param2[expectingMsgTimeoutInMs]
                direction TB
                Param2Info["Type: number\nRequired: false\nUnit: ms\nMin: -1; max: maxInt32; step: 1\nDefault: -1\nDesc: The timeout linked to the expected msg(s).\n-1 if we wait indefinitely."]
            end
            subgraph Param3[exitIfFail]
                direction TB
                Param3Info["Type: boolean\nRequired: false\nDefault: false\nDesc: If the option is active and a message\nverification failed in the module, it will fail."]
            end
            subgraph Param4[bitrateSwitch]
                direction TB
                Param4Info["Type: boolean\nRequired: false\nDefault: true\nDesc: Activate, or not, the bitrate switch. This is only relevant when using CAN FD."]
            end
            subgraph Param5[extendedFrameFormat]
                direction TB
                Param5Info["Type: boolean\nRequired: false\nDefault: false\nDesc: Activate, or not, the extended frame format for the message sent."]
            end
            subgraph Param6[expectedMsgId]
                direction TB
                Param6Info["Type: number\nRequired: false\nMin: -1; max: 0x3FFFFFFF; step: 1\nDefault: -1\nDesc: The ID of the expected message, if equals to -1, no answer is expected."]
            end
            subgraph Param7[expectedMsgVerifs]
                direction TB
                Param7Info["Type: string\nRequired: false\nDesc: This contains the elements to verify in the message received.\nThe info are stored in a string with the following format:\n'MASK_IDX/MASK/IS_LSB/INF_TOLERANCE/SUP_TOLERANCE', if the\npayload of the message with the mask is between INF_TOLERANCE\nand SUP_TOLERANCE, everything is right. It's possible to add\na boolean to wait the right value before returning. To do\nso, you have to append the following information with the\nformat:\n'MASK_IDX/MASK/IS_LSB/INF_TOLERANCE/SUP_TOLERANCE/WAIT_UNTIL_RECEIVED'\nIt's possible to add logs to the test, to do so, you have to\nappend the following information with the format:\n'MASK_IDX/MASK/IS_LSB/INF_TOLERANCE/SUP_TOLERANCE/WAIT_UNTIL_RECEIVED/TEST_NAME'"]
            end
            subgraph Param8[canMsgToSend]
                direction TB
                Param8Info["Type: string\nRequired: false\nDesc: The CAN MSG to send. The message is stored in a string with the\nfollowing format: 'CAN_ID/PAYLOAD_LENGTH/PAYLOAD'"]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
            Param3 ~~~ Param4
            Param4 ~~~ Param5
            Param5 ~~~ Param6
            Param6 ~~~ Param7
            Param7 ~~~ Param8
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### StartAutoAnswer

#### Presentation

This is the start auto answer module. This module allows to start the automatic answering process
in background.

The process can be stopped by calling the [StopAutoAnswer](#stopautoanswer) module.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "StartAutoAnswer"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
            subgraph Param1[displayStats]
                direction TB
                Param1Info["Type: bool\nRequired: false\nDefault: false\nDesc: If the option is active, at the end the module will display in logs the\nnumber of received and sent commands (only the expected one)."]
            end
            subgraph Param2[answersToSend]
                direction TB
                Param2Info["Type: string[]\nRequired: true\nDesc: The CAN messages to send if the expected command has been received.\nThe messages are stored in a string with the following format:\n'MSG_IDX/CAN_ID/PAYLOAD_LENGTH/PAYLOAD'\nThe MSG_IDX indicates the expected message linked to this command to\nsend. The MSG_IDX can't overflow the expected messages list length.\n"]
            end
            subgraph Param3[expectedMsgsVerifs]
                direction TB
                Param3Info["Type: string[]\nRequired: true\nDesc: This contains the elements to verify. The info are stored\nin a string with the following format:\n'MSG_ID/MASK_IDX/MASK/MASK_RESULT',\nif the payload of the message matches the mask and its\nresult, we send an answer."]
            end
            Param1 ~~~ Param2
            Param2 ~~~ Param3
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```

### StopAutoAnswer

#### Presentation

This is the stop auto answer module. This module allows to stop the automatic answering process in
background.

The process can be started by calling the [StartAutoAnswer](#startautoanswer) module.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "StopAutoAnswer"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[inputPeakCanIntf]
                direction TB
                Input1Info["Type: string\nDesc: The input contains the name of the peak CAN interface."]
            end
        end
        subgraph "Parameters"
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
