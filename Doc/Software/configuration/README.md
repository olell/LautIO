# LautIO configuration

LautIO is configured via a single json file stored in LautIO's filesystem.
The file should be located at `/config/config.json`!

The basic structure of LautIOs config file is pretty simple, you just
have to create a json file which contains one object per config section (see other files for sections).

## Example structure
```json

{

    "wifi": {
        "wifi": "stuff"
    },

    "boom": {
        "boom": "pow"
    }

}

```

## Required sections:

* `wifi` (see wifi.md)