# WiFi Configuration

LautIO has two different WiFi modes: `client` or `ap`.
In client mode, LautIO tries to connect to the configured Access Point. In AP mode,
LautIO opens up an Access Point with configured credentials. If you enable the fallback_ap,
LautIO opens up an Access Point in client mode when it fails to connect to your configured AP

## Configuration Fields

| name        | type   | description                  | required               |
|-------------|--------|------------------------------|------------------------|
| mode        | string | WiFi Mode (`client`/`ap`)    | Yes                    |
| fallback_ap | bool   | Enable fallback AP           | In client mode         |
| ap_ssid     | string | Hosted Access Point Name     | In AP mode/fallback_ap |
| ap_pass     | string | Hosted Access Point Password | In AP mode/fallback_ap |
| ssid        | string | Access Point Name            | In client mode         |
| pass        | string | Access Point Name            | In client mode         |