#include "ParameterActionManagerTest.h"

#include "QmlObjectListModel.h"

#include "Fact.h"
#include "FactMetaData.h"

#include "Vehicle.h"
#include "ParameterManager.h"

#include "ParameterAction.h"
#include "ParameterActionManager.h"

// ===========================================================================
// configuration loading
// ===========================================================================

void ParameterActionManagerTest::_loadMalformedParameterActions() {
    _setupFact(":/unittest/UT-MalformedParameterActions.json");
    expectAppMessage(QRegularExpression("missing name separator"));
    _parameterManager = new ParameterActionManager(_actionFilenameFact);
    verifyExpectedLogMessage();

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 0);

    _cleanupParameters();
}

void ParameterActionManagerTest::_loadWrongVersionParameterActions() {
    _setupFact(":/unittest/UT-WrongVersionParameterActions.json");
    expectAppMessage(QRegularExpression("File version 2 is newer than current supported version 1"));
    _parameterManager = new ParameterActionManager(_actionFilenameFact);
    verifyExpectedLogMessage();

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 0);

    _cleanupParameters();
}

void ParameterActionManagerTest::_loadIncorrectSingleParameterActions() {
    _setupFact(":/unittest/UT-IncorrectSingleParameterActions.json");
    expectAppMessage(QRegularExpression("Incorrect value type"));
    _parameterManager = new ParameterActionManager(_actionFilenameFact);
    verifyExpectedLogMessage();

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 0);
    _cleanupParameters();
}

void ParameterActionManagerTest::_loadIncorrectMultiParameterActions() {
    _setupFact(":/unittest/UT-IncorrectMultiParameterActions.json");
    expectAppMessage(QRegularExpression("Incorrect value type"));
    _parameterManager = new ParameterActionManager(_actionFilenameFact);
    verifyExpectedLogMessage();

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 0);
    _cleanupParameters();
}

void ParameterActionManagerTest::_loadSingleParameterActions() {
    _setupFact(":/unittest/UT-SingleParameterActions.json");
    _parameterManager = new ParameterActionManager(_actionFilenameFact);

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 2);
    _cleanupParameters();
}

void ParameterActionManagerTest::_updateSingleFacts()
{
}

void ParameterActionManagerTest::_checkSingleAssignment()
{
}

void ParameterActionManagerTest::_checkSingleRebootAssignment()
{
}

void ParameterActionManagerTest::_loadMultiParameterActions() {
    _setupFact(":/unittest/UT-MultiParameterActions.json");
    _parameterManager = new ParameterActionManager(_actionFilenameFact);

    QmlObjectListModel* actions = _parameterManager->actions();
    QVERIFY(actions->count() == 3);
    _cleanupParameters();
}

void ParameterActionManagerTest::_updateMultiFacts()
{
}

void ParameterActionManagerTest::_checkMultiAssignment()
{
}

void ParameterActionManagerTest::_checkMultiRebootAssignment()
{
}

void ParameterActionManagerTest::_setupFact(const QString& filepath) {
    _actionFilenameFact = new Fact(0, "ParameterActionsSettings", FactMetaData::valueTypeString);
    _actionFilenameFact->setName("ParameterActionsSettings");
    _actionFilenameFact->setRawValue(filepath);
}

void ParameterActionManagerTest::_cleanupParameters() {
    _parameterManager->actions()->clearAndDeleteContents();
    delete _parameterManager;
    _parameterManager = nullptr;
    delete _actionFilenameFact;
    _actionFilenameFact = nullptr;
}

UT_REGISTER_TEST(ParameterActionManagerTest, TestLabel::Unit)
