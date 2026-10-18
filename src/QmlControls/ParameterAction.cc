#include "ParameterAction.h"
#include "Vehicle.h"
#include "QGCLoggingCategory.h"
#include "ParameterManager.h"

QGC_LOGGING_CATEGORY(ParameterActionLog, "QMLControls.ParameterAction")

ParameterAction::ParameterAction(QObject *parent)
    : QObject(parent)
{
   // qCDebug(ParameterActionLog) << Q_FUNC_INFO << this;
}

ParameterAction::ParameterAction(
    const QString &label,
    QVector<QPair<QString, double>>& parameters,
    QObject *parent
    ) : QObject(parent)
    , _label(label)
    , _parameters(parameters)
{
   // qCDebug(ParameterActionLog) << Q_FUNC_INFO << this;
}

ParameterAction::~ParameterAction()
{
   // qCDebug(ParameterActionLog) << Q_FUNC_INFO << this;
}

void ParameterAction::execute(Vehicle *vehicle)
{
    if (!vehicle) {
        return;
    }

    if(!_checkParameterChangeValidity(vehicle)) {
        setActionAvailable(false);
        return;
    }

    const auto paramMgr = vehicle->parameterManager();

    for(const auto& param : _parameters) {
        auto currentParameter = paramMgr->getParameter(vehicle->compId(), param.first);
        if(currentParameter->rawValue() == QVariant::fromValue(param.second)) {
            continue;
        }
        currentParameter->setRawValue(param.second);
    }
}

void ParameterAction::setActionAvailable(const bool state) {

    if(_actionAvailable == state) {
        return;
    }
    _actionAvailable = state;
    emit actionAvailableChanged();
}

// TODO: Add error messages to UI
bool ParameterAction::_checkParameterChangeValidity(Vehicle *vehicle) {
    const auto paramMgr = vehicle->parameterManager();

    for(const auto& param : _parameters) {

        if(!paramMgr->parameterExists(vehicle->compId(), param.first)) {
            return false; // Missing parameter

        } else if(paramMgr->getParameter(vehicle->compId(), param.first)->readOnly()) {
            return false; // Parameter is read-only

        } else if(paramMgr->getParameter(vehicle->compId(), param.first)->typeIsBool() || paramMgr->getParameter(vehicle->compId(), param.first)->typeIsString()) {
            return false; // Incompatible value types
        }
    }
    return true;
}
