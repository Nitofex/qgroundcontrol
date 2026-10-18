# ============================================================================
# QmlControls Unit Tests
# ============================================================================

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        MapMarkerGroupingTest.cc
        MapMarkerGroupingTest.h
        MapPositionTrackerTest.cc
        MapPositionTrackerTest.h
        ObjectItemModelBaseTest.cc
        ObjectItemModelBaseTest.h
        ObjectListModelBaseTest.cc
        ObjectListModelBaseTest.h
        ParameterActionManagerTest.cc
        ParameterActionManagerTest.h
        QmlObjectListModelTest.cc
        QmlObjectListModelTest.h
        QmlObjectTreeModelTest.cc
        QmlObjectTreeModelTest.h
        TestDirtyObject.h
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

qt_add_resources(${CMAKE_PROJECT_NAME} "QmlControls_res"
    PREFIX "/unittest"
    FILES
        UT-SingleParameterActions.json
        UT-MultiParameterActions.json
        UT-IncorrectSingleParameterActions.json
        UT-IncorrectMultiParameterActions.json
        UT-MalformedParameterActions.json
        UT-WrongVersionParameterActions.json
)

add_qgc_test(MapMarkerGroupingTest LABELS Unit)
add_qgc_test(MapPositionTrackerTest LABELS Unit)
add_qgc_test(ObjectItemModelBaseTest LABELS Unit)
add_qgc_test(ObjectListModelBaseTest LABELS Unit)
add_qgc_test(ParameterActionManagerTest LABELS Unit)
add_qgc_test(QmlObjectListModelTest LABELS Unit)
add_qgc_test(QmlObjectTreeModelTest LABELS Unit)
