# Controls & Their Structure

## What is a control
A control is a basic element for controlling the DSP. E.G. a volume control or a equalizer etc.

## What type of controls are implemented

| Control Type    | Internal ID |
|-----------------|------------:|
| Volume Slew     | 0           |
| Second order EQ | 1           |

### Volume Slew (`0`)
Simple 2Ch volume control with slew setting. The slew value should be between 5 and 15 (5 is more fast). If you use a value below 5 the DSP might make some cracking noises.

* Internal Name: `volslew`
* Required fields:
    * `volume`
        * the dB value (-50dB up to 0dB)
    * `slew`
        * the slew value (1 up to 15)

### Second Order EQ (`1`)
* Internal Name: `SOEQ`
* Required fields:
    * `filter_type`
        * equalizer type (TODO Document types)
    * `Q`
    * `freq`
    * `boost`
    * `state`
        * 0 -> bypass ; 1-> active

## dsp/structure.json ?
This file defines the control structure for the webinterface.

## dsp/constrols.json
This file defines which controls are available and some internal information about them.

Fields:
* `id`
    * id of this control, must match with structure.md
* `addr`
    * address of this control
* `type`
    * type of this control (internal id)
* `const`
    * the values should not be overwritten -> always same after reboot
* `xyz`
    * required value fields for the specific control type (see above)