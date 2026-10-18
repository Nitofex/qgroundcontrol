#pragma once

#include "MAVLinkEnums.h"

#include <QtCore/QObject>
#include <QtQmlIntegration/QtQmlIntegration>

class Vehicle;

class ParameterAction: public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("Vehicle.h")
    Q_PROPERTY(QString label READ label CONSTANT)
    Q_PROPERTY(bool actionAvailable READ actionAvailable NOTIFY actionAvailableChanged FINAL)

public:
    explicit ParameterAction(QObject *parent = nullptr);
    ParameterAction(
        const QString &label,
        QVector<QPair<QString, double>>& parameters,
        QObject *parent = nullptr
        );
    ~ParameterAction();

    Q_INVOKABLE void execute(Vehicle *vehicle);

    const QString &label() const { return _label; }
    const QVector<QPair<QString, double>> &parameters() const { return _parameters; }

    void setActionAvailable(const bool state);
    bool actionAvailable() const { return _actionAvailable; }

signals:
    void actionAvailableChanged();

private:
    bool _checkParameterChangeValidity(Vehicle *vehicle);

private:
    const QString _label;
    const QVector<QPair<QString, double>> _parameters;
    bool _actionAvailable{false};
};
