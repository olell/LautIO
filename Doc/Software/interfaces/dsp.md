# Category: `DSP`

## Commands:
* `get_controls`
* `update_control`

## Command `get_controls`
* Returns all available controls
### Request
```JSON
{
    "category": "dsp",
    "command": "get_controls"
}
```
### Response
```JSON
{
    "controls": [...],
    "cmd": "dsp_controls",
    "status": "ok"/"error"
}

```

## Command `update_control`
* Update a control
### Request
```JSON
{
    "category": "dsp",
    "control": {updated control}   
}
```
### Response
```JSON
{
    "status": "ok",
    "message": "done!",
    "cmd": "dsp_update_control"
}