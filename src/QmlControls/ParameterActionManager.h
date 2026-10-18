#pragma once

#include <QtCore/QObject>
#include <QtQmlIntegration/QtQmlIntegration>

class Fact;
class QmlObjectListModel;
class Vehicle;

/// \brief Loads the specified action file and provides access to the actions it contains.
///
/// Action files are loaded from the default ParameterActions directory.
/// The actions file name is filename only, no path.
///
class ParameterActionManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("Fact.h")
    Q_MOC_INCLUDE("QmlObjectListModel.h")
    Q_PROPERTY(Fact* actionFileNameFact READ actionFileNameFact WRITE setActionFileNameFact NOTIFY actionFileNameFactChanged)
    Q_PROPERTY(QmlObjectListModel* actions READ actions CONSTANT)

public:
    explicit ParameterActionManager(QObject *parent = nullptr);
    explicit ParameterActionManager(Fact *actionFileNameFact, QObject *parent = nullptr);
    ~ParameterActionManager();

    Fact *actionFileNameFact() { return _actionFileNameFact; }
    void setActionFileNameFact(Fact *actionFileNameFact);
    QmlObjectListModel *actions() { return _actions; }

signals:
    void actionFileNameFactChanged();

private slots:
    void _loadActionsFile();
    void _activeVehicleChanged(Vehicle* vehicle);
    void _vehicleParametersReadyChanged(const bool state);
    void _vehicleArmedStateChanged(const bool state);

private:
    void _checkParameterExecutionAvailability(const bool vehicleArmed);

private:
    Fact *_actionFileNameFact = nullptr;
    QmlObjectListModel *_actions = nullptr;
    Vehicle *_activeVehicle = nullptr;

    static constexpr int _supportedVersion = 1;
};
