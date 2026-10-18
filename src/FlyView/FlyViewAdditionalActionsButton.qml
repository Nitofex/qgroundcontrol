import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QGroundControl
import QGroundControl.Controls
import QGroundControl.FlyView

ToolStripAction {
    id:         action
    text:       qsTr("Actions")
    iconSource: "qrc:/qmlimages/HamburgerThin.svg"
    visible:    _additionalActions.anyActionAvailable || _mavlinkActions.anyActionAvailable || _customActions.anyActionAvailable || _parameterActions.anyActionAvailable
    enabled:    true

    property var _guidedController: globals.guidedControllerFlyView

    property var _additionalActions: FlyViewAdditionalActionsList {
        guidedController: _guidedController
    }

    property var _mavlinkActions: MavlinkActionManager {
        actionFileNameFact: QGroundControl.settingsManager.mavlinkActionsSettings.flyViewActionsFile

        property bool anyActionAvailable: QGroundControl.multiVehicleManager.activeVehicle && actions.count > 0
    }

    property var _customActions: FlyViewAdditionalCustomActionsList {
        guidedController: _guidedController
    }

    property var _parameterActions: ParameterActionManager {
        actionFileNameFact: QGroundControl.settingsManager.parameterActionsSettings.parameterActionsFile

        property bool anyActionAvailable: QGroundControl.multiVehicleManager.activeVehicle && actions.count > 0
    }

    dropPanelComponent: Component {
        FlyViewAdditionalActionsPanel {
            additionalActions:  _additionalActions
            mavlinkActions:     _mavlinkActions.actions
            customActions:      _customActions
            parameterActions:   _parameterActions.actions
        }
    }
}
