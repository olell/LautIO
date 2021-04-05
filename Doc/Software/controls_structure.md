# Controls & Their Structure

## What is a control
A control is a basic element for controlling the DSP. E.G. a volume control or a equalizer etc.

## What type of controls are implemented

| Control Type | Internal ID |
|--------------|------------:|
| Volume Slew  | 0           |

## dsp/structure.json ?
This file defines the control structure for the webinterface.

## dsp/Controls.csv ?
This is a simple list of all controls available. First col of each row is the id, second is the address and the third is the type id (see list above).