# See http://www.na-mic.org/Bug/view.php?id=3340
# message(WARNING "warning: Module ${MODULE_NAME} doesn't have any test !")

#-----------------------------------------------------------------------------
set(CLP ${MODULE_NAME})

#-----------------------------------------------------------------------------
set(TEMP ${Slicer_BINARY_DIR}/Testing/Temporary)

#-----------------------------------------------------------------------------
add_executable(${CLP}Test ${CLP}Test.cxx)
target_link_libraries(${CLP}Test ${CLP}Lib ${SlicerExecutionModel_EXTRA_EXECUTABLE_TARGET_LIBRARIES})
set_target_properties(${CLP}Test PROPERTIES LABELS ${CLP})

set(testname ${CLP}Test)
add_test(NAME ${testname} COMMAND ${SEM_LAUNCH_COMMAND} $<TARGET_FILE:${CLP}Test>
  ModuleEntryPoint
 --returnparameterfile ${TEMP}/FiducialRegistration.params
 --fixedLandmarks 37.5615,82.3156,0 --fixedLandmarks -26.373,81.5164,0 --fixedLandmarks -2.39754,-21.5779,0
 --movingLandmarks 39.959,59.9385,30 --movingLandmarks -24.7746,62.3361,30 --movingLandmarks 0.79918,-52.7459,33.75
 --saveTransform ${TEMP}/FiducialRegistration_vtkMRMLLinearTransformNodeE.txt
 --transformType Rigid
)

set_property(TEST ${testname} PROPERTY LABELS ${CLP})
