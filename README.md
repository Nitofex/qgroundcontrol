## Addition

Added a `Parameter Actions` feature to Fly View, which can be accessed after connecting to an aircraft with a valid loaded `ParameterActions` json.

The `test_actions.json` file can be placed in it's appropriate directory outlined in [Custom Parameter Actions](docs/en/qgc-user-guide/custom_actions/custom_parameter_actions.md)

## Supports

- Setting singular or multiple parameters with a single action
- Checking for parameter validity and value validity
- Armed state safety for parameters requiring reboot

## Testing

Tests can be run as part of default QGC tests, or as a single test using `.\QGroundControl.exe --unittest:ParameterActionManagerTest`