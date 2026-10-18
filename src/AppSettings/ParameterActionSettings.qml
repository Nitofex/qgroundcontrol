import QtQuick
import QtQuick.Layouts

import QGroundControl
import QGroundControl.Controls

SettingsGroupLayout {
    Layout.fillWidth:       true
    Layout.preferredWidth:  ScreenTools.defaultFontPixelWidth * 35
    heading:                qsTr("Parameter Actions")
    headingDescription:     qsTr("Action JSON files should be created in the '%1' folder.").arg(QGroundControl.settingsManager.appSettings.parameterActionsSavePath)

    property var _parameterActionsSettings: QGroundControl.settingsManager.parameterActionsSettings

    function parameterActionList() {
        var fileModel = QGCFileDialogController.getFiles(QGroundControl.settingsManager.appSettings.parameterActionsSavePath, "*.json")
        fileModel.unshift(qsTr("<None>"))
        return fileModel
    }

    LabelledComboBox {
        Layout.fillWidth:   true
        label:              qsTr("Fly View Parameter Actions")
        model:              parameterActionList()
        onActivated:        (index) => index == 0 ? _parameterActionsSettings.parameterActionsFile.rawValue = "" : _parameterActionsSettings.parameterActionsFile.rawValue = comboBox.currentText
        enabled:            model.length > 1

        Component.onCompleted: {
            var index = comboBox.find(_parameterActionsSettings.parameterActionsFile.valueString)
            comboBox.currentIndex = index == -1 ? 0 : index
        }
    }
}
