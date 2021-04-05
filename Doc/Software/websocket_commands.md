# Websocket Commands

Every data sent through the websocket (/ws) to the ESP should be in JSON format.
It should contain the base field "command" which descides where the data will be
interpreted: Example:

```JSON
{
    "command": "test"
}
```

## Command: `test`
This is the test command, it just returns 
``` JSON
{"status": "ok", "message": "test command received!"}
```

## Command `dsp`
This command indicates that the DSP should be controlled.

The `action` field is required!

### Action `update_volslew`
Updates a volume_slew control. It requires following fields:
* `id`
* `volume`
* `slew`