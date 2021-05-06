# FTP Server configuration

LautIO can host an experimental FTP server, serving files from the SD Card.

## Configuration Fields

| name        | type   | description                  | required               |
|-------------|--------|------------------------------|------------------------|
| enable      | bool   | enable / disable ftp server  | yes                    |
| username    | string | ftp username                 | if enabled             |
| password    | string | ftp password                 | if enabled             |