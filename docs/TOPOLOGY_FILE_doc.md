<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Topology file documentation

## Table of contents

- [Topology file documentation](#topology-file-documentation)
  - [Table of contents](#table-of-contents)
  - [Preamble](#preamble)
  - [Introduction](#introduction)
  - [Library](#library)
  - [Instances](#instances)
  - [Bindings](#bindings)
    - [External bindings](#external-bindings)
    - [Internal bindings](#internal-bindings)
  - [Route hints](#route-hints)

## Preamble

**The topology mechanism has never been completly developed.** We have begun to work on it but we
never finished it (until now, we didn't need it).

In this readme we present what we wanted to do.

## Introduction

Topology consists of several configuration elements:

- [A components library](#library), which is a set of classes (not-yet instanciated object
  definition)
- A set of instances (instances of components to be found in library)
- Bindings (connections between neighbour instances pins)
- Route hints (route hints between foreign instances pins)

## Library

Classes of objects defines how an object looks like, which pins it may have.
Those definitions must be loadable from several locations (most of them are provided by plugins and
a few may be added through text configuration files, typically the definition of the Devices Under
Test)

Below is a pseudo JSON/XSD definition of such objects:

```json
{
    "name": "[string]",
    "description": "[string]",
    "type": "basic|intermediary",
    "category": "[string]",
    "pins": [
        {
            "groupName": "[string]",
            "elements": [
                {
                    "label": "[string]",
                    "kind": "signal|[string]",
                    "?direction":"input|ouput|inout"
                }
            ]
        }
    ],
    "params": [
        {
            "name": "[string]",
            "type": "string",
            "description": "[string]",
            "?minLength": "[number]",
            "?maxLength": "[number]",
            "?patternRegex": "[string]",
            "?defaultValue": "[string]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "bool",
            "description": "[string]",
            "?defaultValue": "[bool]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "number",
            "description": "[string]",
            "commaShift": "[integer]",
            "nbAfterCommaToDisplay": "[integer]",
            "?min": "[integer]",
            "?max": "[integer]",
            "?step": "[integer]",
            "?unit": "[A|V|W|°C|Ω|s|Hz|ADC(bits)|lm|F|H||K|string<exp></exp><sub></sub>]",
            "?unitPowerOf10": "[integer]",
            "?defaultValue": "[integer]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "date",
            "description": "[string]",
            "?min": "[integer]",
            "?max": "[integer]",
            "?defaultValue": "[integer]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "time",
            "description": "[string]",
            "?min": "[integer]",
            "?max": "[integer]",
            "?defaultValue": "[integer]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "select",
            "description": "[string]",
            "options": [{
                "key": "[string]",
                "value": "[string]",
                "?hide": "[bool]"
            }],
            "?optionDefault": [
                "[string]"
            ],
            "?multiple": "[bool]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "checkBoxList",
            "description": "[string]",
            "list": [{
                "key": "[string]",
                "?hide": "[bool]"
            }],
            "?checkedList": [
                "[string]"
            ],
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "radioList",
            "description": "[string]",
            "list": [{
                "key": "[string]",
                "?hide": "[bool]"
            }],
            "?defaultChecked": "[string]",
            "?hide": "[bool]",
            "?required": "[bool]"
        },
        {
            "name": "[string]",
            "type": "color",
            "description": "[string]",
            "?defaultValue": "[integer]",
            "?hide": "[bool]",
            "?required": "[bool]"
        }
    ]
}
```

Below is a proposal of such a few elements class definition as generated by plugins. Those ones do
not need to be saved in project file, they can be found back based on instances configuration file
and plugins.

```json
{
    "name": "34970A",
    "description": "Agilent DMM mainframe",
    "type": "basic",
    "category": "Agilent",
    "pins": [
        {
            "groupName": "CTRL",
            "elements": [
                { "label": "RS232", "kind": "stream/rs232" },
                { "label": "GP-IB", "kind": "stream/ieee488" }
            ]
        },
        {
            "groupName": "slots",
            "elements": [
                { "label": "slot0", "kind": "agilent-card-slot", "direction": "input" },
                { "label": "slot1", "kind": "agilent-card-slot", "direction": "input" },
                { "label": "slot2", "kind": "agilent-card-slot", "direction": "input" }
            ]
        }
    ],
    "params": []
  }

  {
    "name": "34908A",
    "summary": "Agilent 40-single-channel multiplexer card",
    "category": "Agilent",
    "type": "intermediary",
    "pins": [
        {
            "groupName": "slot",
            "elements": [{ "label": "slot", "kind": "agilent-card-slot", "direction": "output" }]
        },
        {
            "groupName": "channels",
            "elements": [
                { "label": "0-Hi", "kind": "signal", "direction": "input" },
                { "label": "1-Hi", "kind": "signal", "direction": "input" },
                { "label": "2-Hi", "kind": "signal", "direction": "input" },
                { "label": "3-Hi", "kind": "signal", "direction": "input" },
                { "label": "40-Hi", "kind": "signal", "direction": "input" }
            ]
        }
    ],
    "params": [
        {
            "name": "exclusive",
            "type": "boolean",
            "defaultValue": true,
            "description": "Automatically deselect relays so one and only one is active"
        },
        {
            "name": "dmm",
            "type": "boolean",
            "defaultValue": true,
            "description": "Are common-Hi and common-Lo internally routed to mainframe internal Digital Multi-Measurement unit, requires exclusivity"
        }
    ]
  }

  {
    "name": "ftdi-usb-serial",
    "description": "Raw RS-232 <-> USB-CDC serial converter",
    "category": "FTDI",
    "type": "basic",
    "pins": [
        {
            "groupName": "RS232",
            "elements" : [ { "label": "RS232", "kind": "stream/rs232" } ]
        },
        {
            "groupName": "USB",
            "elements": [ { "label": "USB", "kind": "usb" } ]
        }
    ],
    "params": [
        {
            "name": "serial_number",
            "type": "string",
            "description": "Optional known SN helps finding back this device"
        }
    ]
}
```

User can add private class definitions in its project file.
For simplicity, a few buses are seen as simple pins like LIN which is actually a two-wire network
(with ground) and micro-USB which is actually a set of several test points (D+/D-) externally
connected to an USB wire.

```json
{
    "name": "examplebox",
    "description": "Device under test",
    "category": "EXAMPLE",
    "type": "basic",
    "pins": [
        {
            "groupName": "Test points",
            "elements": [
                { "label": "SW1", "kind": "signal", "direction": "output" },
                { "label": "SW2", "kind": "signal", "direction": "output" },
                { "label": "SW3", "kind": "signal", "direction": "output" },
                { "label": "LDO1", "kind": "signal", "direction": "output" },
                { "label": "3V8_GSM", "kind": "signal", "direction": "output" }
            ]
        },
        {
            "groupName": "leds",
            "elements": [
                { "label": "led-green", "kind": "light", "direction": "output" },
                { "label": "led-red", "kind": "light", "direction": "output" }
            ]
        }
    ],
    "params": []
}

{
    "name": "lin-to-rs232-converter",
    "description": "Raw LIN <-> RS-232 serial converter",
    "category": "EXAMPLE",
    "type": "basic",
    "pins": [
        {
            "groupName": "LIN",
            "elements": [ { "label": "LIN", "kind":"stream/lin" } ]
        },
        {
            "groupName": "RS232",
            "elements": [ { "label": "RS232", "kind": "stream/rs232" } ]
        }
    ]
}
```

## Instances

Instances configuration contains the set of used components with their global settings and a few
graphical hints.

Below is a pseudo JSON/XSD definition of such objects

```json
{
    "name": "[string]",
    "component": "[string]",
    "params": [ { "key": "[string]", "value": "[string|bool|integer]" } ],
    "_*":"IDE private hints"
}
```

Such an example can be:

```json
{
    "name": "product1",
    "component": "examplebox",
    "params": [],
    "_myGuiIdeHints": { "x": "25", "y": "64" }
}

{
    "name": "product2",
    "component": "examplebox",
    "params": [],
    "_myGuiIdeHints": { "x": "35", "y": "64" }
}

{
    "name": "product3",
    "component": "examplebox",
    "params": [],
    "_myGuiIdeHints": { "x": "45", "y": "64" }
}

{
    "name": "product4",
    "component": "examplebox",
    "params": [],
    "_myGuiIdeHints": { "x": "55", "y": "64" }
}

{
    "name": "serial-debug-adapter-1",
    "component": "ftdi-usb-serial",
    "params": [ { "key": "serial_number", "value":"F7C464" } ],
    "_myGuiIdeHints": { "x": "55", "y": "64" }
}

{
    "name": "lin-to-rs232-1",
    "component": "lin-to-rs232-converter",
    "params": [],
    "_myGuiIdeHints": { "x": "25", "y": "44" }
}

{
    "name": "serial-lin-adapter-1",
    "component": "ftdi-usb-serial",
    "params": [ { "key": "serial-number", "value": "X784F1" } ]
}

{
    "name": "34970A",
    "component": "34970A",
    "params": []
}

{
    "name": "34901A-1",
    "component": "34901A",
    "params": [ { "key": "exclusive", "value": true } ]
}
```

## Bindings

### External bindings

Each binding is a set of several instance pads which indicates the logical binding of usually two
pins, optionally more.

```json
{
    "pins": [
        { "instance": "34901A-1", "pin": "slot" },
        { "instance": "34970A", "pin": "slot1" }
    ],
    "_myGuiIdeHints": {
        "keyPoints": [
            { "x": 54, "y": 78 },
            { "x": 74, "y": 54 }
        ]
    }
}
```

### Internal bindings

Some modules likes multiplexeres or matrixes have an internal routing. The internal routing is
configured by parameters and can be retrieved by a common method for all modules of this type. The
method also gives an unique key to use for activate the route chosen.

The JSON object returns by the module is as follow:

```json
[
    {
        "uniqueKey": "0-Hi:slot",
        "fromPin": "0-Hi",
        "toPin": "slot"
    }
]
```

## Route hints

Route hints definition is wanted to be optional in the future. If you see bindings like MAC network,
then routes is IP network. They will likely not be visible graphically as opposite to bindings which
are likely seen as wires.

Project can define hints for routes between two pins. Software is expected to use those route hints
when applicable, and avoid a self route discovery in those cases. First version of test bed will
likely require to find a hint for all routes waiting for an auto-discovery mechanism to be
implemented.

```json
{
    "uniqueKey": "my-artelbox_SW3:my-keithley:Hi",
    "fromPin": { "instance": "my-artelbox", "pin": "SW3" },
    "toPin":   { "instance": "my-keithley", "pin": "Hi" },
    "path": [ { "instance": "matrix1", "fromPin": "row7", "toPin": "col1" } ]
}
```
