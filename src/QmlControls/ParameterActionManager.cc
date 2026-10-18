#include "ParameterActionManager.h"
#include "ParameterAction.h"
#include "Fact.h"
#include "JsonParsing.h"
#include "AppMessages.h"
#include "SettingsManager.h"
#include "AppSettings.h"
#include "QGCLoggingCategory.h"
#include "QmlObjectListModel.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "ParameterManager.h"

#include <QtCore/QDir>
#include <QtCore/QJsonArray>
#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtQml/QQmlEngine>

QGC_LOGGING_CATEGORY(ParameterActionManagerLog, "QMLControls.ParameterActionManager")

ParameterActionManager::ParameterActionManager(QObject *parent)
    : QObject(parent)
    , _actions(new QmlObjectListModel(this))
{

    QObject::connect(MultiVehicleManager::instance(), &MultiVehicleManager::activeVehicleChanged, this, &ParameterActionManager::_activeVehicleChanged, Qt::UniqueConnection);
   // qCDebug(ParameterActionManagerLog) << Q_FUNC_INFO << this;
}

ParameterActionManager::ParameterActionManager(Fact *actionFileNameFact, QObject *parent)
    : QObject(parent)
    , _actions(new QmlObjectListModel(this))
{
    QObject::connect(MultiVehicleManager::instance(), &MultiVehicleManager::activeVehicleChanged, this, &ParameterActionManager::_activeVehicleChanged, Qt::UniqueConnection);
    setActionFileNameFact(actionFileNameFact);
}

ParameterActionManager::~ParameterActionManager()
{
   // qCDebug(ParameterActionManagerLog) << Q_FUNC_INFO << this;
}

void ParameterActionManager::setActionFileNameFact(Fact *actionFileNameFact)
{
    _actionFileNameFact = actionFileNameFact;
    emit actionFileNameFactChanged();
    (void) connect(_actionFileNameFact, &Fact::rawValueChanged, this, &ParameterActionManager::_loadActionsFile);

    _loadActionsFile();
}

void ParameterActionManager::_loadActionsFile()
{
    _actions->clearAndDeleteContents();
    const QString actionFileName = _actionFileNameFact->rawValue().toString();
    if (actionFileName.isEmpty()) {
        return;
    }

            // Custom actions are always loaded from the custom actions save path
    const QString savePath = SettingsManager::instance()->appSettings()->parameterActionsSavePath();
    const QDir saveDir = QDir(savePath);
    const QString fullPath = saveDir.absoluteFilePath(actionFileName);

            // It's ok for the file to not exist
    const QFileInfo fileInfo = QFileInfo(fullPath);
    if (!fileInfo.exists()) {
        return;
    }

    constexpr const char *kQgcFileType = "ParameterActions";
    constexpr const char *kActionListKey = "actions";
    constexpr const char *kParameterListKey = "parameters";

    _actions->clearAndDeleteContents();

    QString errorString;
    int version;
    const QJsonObject jsonObject = JsonParsing::openInternalQGCJsonFile(fullPath, kQgcFileType, _supportedVersion, _supportedVersion, version, errorString);
    if (!errorString.isEmpty()) {
        QGC::showAppMessage(tr("Failed to load parameter actions file: `%1` error: `%2`").arg(fullPath, errorString));
        return;
    }

    const QList<JsonParsing::KeyValidateInfo> keyInfoList = {
                                                             { kActionListKey, QJsonValue::Array, /* required= */ true },
                                                             };
    if (!JsonParsing::validateKeys(jsonObject, keyInfoList, errorString)) {
        QGC::showAppMessage(tr("Parameter actions file - incorrect format: %1").arg(errorString));
        return;
    }

    const QJsonArray actionList = jsonObject[kActionListKey].toArray();
    for (const auto &actionJson: actionList) {
        if (!actionJson.isObject()) {
            QGC::showAppMessage(tr("Parameter actions file - incorrect action format: JsonValue not an object"));
            _actions->clearAndDeleteContents();
            return;
        }

        const QList<JsonParsing::KeyValidateInfo> actionKeyInfoList = {
                                                                       { "label",          QJsonValue::String, /* required= */ true },
                                                                       { "parameters",    QJsonValue::Array, /* required= */ true }
                                                                       };

        const auto actionObj = actionJson.toObject();
        if (!JsonParsing::validateKeys(actionObj, actionKeyInfoList, errorString)) {
            QGC::showAppMessage(tr("Parameter actions file - incorrect parameter format: %1").arg(errorString));
            _actions->clearAndDeleteContents();
            return;
        }

        const auto label = actionObj["label"].toString();

        const QJsonArray paramList = actionObj[kParameterListKey].toArray();
        QVector<QPair<QString, double>> parameters;
        for (const auto &paramJson: paramList) {
            if (!paramJson.isObject()) {
                QGC::showAppMessage(tr("Parameter actions file - incorrect parameter list [%1] format: JsonValue not an object").arg(label));
                _actions->clearAndDeleteContents();
                return;
            }

            const QList<JsonParsing::KeyValidateInfo> parameterKeyInfoList = {
                                                                            { "name",          QJsonValue::String, /* required= */ true },
                                                                            { "value",    QJsonValue::Double, /* required= */ true }
                                                                            };

            const auto paramObj = paramJson.toObject();
            if (!JsonParsing::validateKeys(paramObj, parameterKeyInfoList, errorString)) {
                QGC::showAppMessage(tr("Parameter actions file - incorrect parameter list [%1] format: %2").arg(label).arg(errorString));
                _actions->clearAndDeleteContents();
                return;
            }

            const auto name = paramObj["name"].toString();
            const auto value = paramObj["value"].toDouble(0.0);

            for(const auto &param : parameters) {

                if(param.first == name) {
                    QGC::showAppMessage(tr("Parameter actions file - duplicate entries not allowed: [%1] Parameter: %2").arg(label).arg(name));
                    _actions->clearAndDeleteContents();
                    return;
                }
            }
            const QPair<QString, double> parameterEntry(name, value);
            parameters.emplace_back(name, value);
        }

        ParameterAction *const action = new ParameterAction(label, parameters, this);
        QQmlEngine::setObjectOwnership(action, QQmlEngine::CppOwnership);
        (void) _actions->append(action);
    }
}

void ParameterActionManager::_activeVehicleChanged(Vehicle* vehicle) {
    if(!vehicle) {
        return;
    }

    _activeVehicle = vehicle;
    QObject::connect(vehicle->parameterManager(), &ParameterManager::parametersReadyChanged, this, &ParameterActionManager::_vehicleParametersReadyChanged, Qt::UniqueConnection);
    QObject::connect(vehicle, &Vehicle::armedChanged, this, &ParameterActionManager::_vehicleArmedStateChanged, Qt::UniqueConnection);
}

void ParameterActionManager::_vehicleParametersReadyChanged(const bool state) {
    if(state) {
        _checkParameterExecutionAvailability(_activeVehicle->armed());
    }
}

void ParameterActionManager::_vehicleArmedStateChanged(const bool state) {
    _checkParameterExecutionAvailability(state);
}

void ParameterActionManager::_checkParameterExecutionAvailability(const bool vehicleArmed) {
    if(!_activeVehicle) {
        return;
    }

    auto paramMgr = _activeVehicle->parameterManager();

    for(int i = 0; i < _actions->count(); i++) {
        auto action = _actions->value<ParameterAction*>(i);
        bool parameterMissing = false;
        bool parameterRequiresReboot = false;

        for(const auto& param : action->parameters()) {

            if(!paramMgr->parameterExists(_activeVehicle->compId(), param.first)) {
                action->setActionAvailable(false);
                parameterMissing = true;
                break;
            } else if(vehicleArmed && paramMgr->getParameter(_activeVehicle->compId(), param.first)->vehicleRebootRequired()) {
                action->setActionAvailable(false);
                parameterRequiresReboot = true;
                break;
            }
        }
        action->setActionAvailable(!parameterMissing && !parameterRequiresReboot);
    }
}
