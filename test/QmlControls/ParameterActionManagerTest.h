#pragma once

#include <QtCore/QObject>

#include "UnitTest.h"

class ParameterActionManager;
class Fact;

class ParameterActionManagerTest : public UnitTest
{
    Q_OBJECT

private slots:
    // configuration loading
    void _loadMalformedParameterActions();
    void _loadWrongVersionParameterActions();
    void _loadIncorrectSingleParameterActions();
    void _loadIncorrectMultiParameterActions();
    void _loadSingleParameterActions();
    void _loadMultiParameterActions();

    // fact update single
    void _updateSingleFacts();
    void _checkSingleAssignment();
    void _checkSingleRebootAssignment();

    // fact update multi
    void _updateMultiFacts();
    void _checkMultiAssignment();
    void _checkMultiRebootAssignment();

private:
    void _setupFact(const QString& filepath);
    void _cleanupParameters();

private:

    Fact* _actionFilenameFact = nullptr;
    ParameterActionManager* _parameterManager = nullptr;
};
