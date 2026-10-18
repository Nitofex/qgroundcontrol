# Custom Mavlink Parameter Action

The Fly View supports the ability execute arbitrary parameter changes to the active vehicle. In the Fly View these will show up in the Toolstrip Action list.

## Mavlink Parameter Actions File

The actions available are defined in a JSON file. The format of that file is as follows:

```json
{
  "fileType": "ParameterActions",
  "version": 1,
  "actions": [
    {
      "label": "Disable GPS aiding",
      "parameters": [
        { "name": "EKF2_GPS_CTRL", "value": 0 }
      ]
    },
    {
      "label": "Enable GPS aiding",
      "parameters": [
        { "name": "EKF2_GPS_CTRL", "value": 7 }
      ]
    }
  ]
}
```

Fields:

- actions (required) - An array of json objects, one for each command
- label (required) - The user visible short description for the command. This is used as the button text for the Fly View - Actions command list.
- parameters (required) - An array of json objects, one for each parameter tied to the given command.
- name (required) - The parameter name of the aircraft you want to change. The component id used is of vehicle->compId, which should return the default component ID, or `MAV_COMP_ID_AUTOPILOT1`.
- value (required) - The value of the parameter to be changed. Supports `int`, `uint`, `float`, and `double`, though is internally interpreted as `double`, allowing the conversions to be handled by `ParameterManager`.

Mavlink parameter action files should be located in the ParameterActions directory of the QGC save location. For example on Linux that would be `~/Documents/QGroundControl/ParameterActions` or `~/Documents/QGroundControl Daily/ParameterActions`.

When you start up QGC it will load these files if they exist and make the commands available for use, given the supported versions match and the file itself is valid.
