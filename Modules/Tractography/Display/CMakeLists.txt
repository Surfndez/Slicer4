project(SlicerTractographyDisplay)

#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.2)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# See http://cmake.org/cmake/help/cmake-2-8-docs.html#section_Policies for details
#-----------------------------------------------------------------------------

SET(project_policies
  CMP0001 # NEW: CMAKE_BACKWARDS_COMPATIBILITY should no longer be used.
  CMP0002 # NEW: Logical target names must be globally unique.
  CMP0003 # NEW: Libraries linked via full path no longer produce linker search paths.
  CMP0004 # NEW: Libraries linked may NOT have leading or trailing whitespace.
  CMP0005 # NEW: Preprocessor definition values are now escaped automatically.
  CMP0006 # NEW: Installing MACOSX_BUNDLE targets requires a BUNDLE DESTINATION.
  CMP0007 # NEW: List command no longer ignores empty elements.
  CMP0008 # NEW: Libraries linked by full-path must have a valid library file name.
  CMP0009 # NEW: FILE GLOB_RECURSE calls should not follow symlinks by default.
  CMP0010 # NEW: Bad variable reference syntax is an error.
  CMP0011 # NEW: Included scripts do automatic cmake_policy PUSH and POP.
  CMP0012 # NEW: if() recognizes numbers and boolean constants.
  CMP0013 # NEW: Duplicate binary directories are not allowed.
  CMP0014 # NEW: Input directories must have CMakeLists.txt
  )
FOREACH(policy ${project_policies})
  IF(POLICY ${policy})
    CMAKE_POLICY(SET ${policy} NEW)
  ENDIF()
ENDFOREACH()

# --------------------------------------------------------------------------
# Find Slicer3

if(NOT Slicer_SOURCE_DIR)
  find_package(Slicer3 REQUIRED)
  include(${Slicer_USE_FILE})
  slicer3_set_default_install_prefix_for_external_projects()
endif(NOT Slicer_SOURCE_DIR)

# --------------------------------------------------------------------------
# Add logic first
ADD_SUBDIRECTORY(Logic)

# --------------------------------------------------------------------------
# Include dirs

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Slicer_Libs_INCLUDE_DIRS}
  ${Slicer_Base_INCLUDE_DIRS}
  ${vtkSlicerTractographyDisplayModuleLogic_SOURCE_DIR}
  ${vtkSlicerTractographyDisplayModuleLogic_BINARY_DIR}
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/vtkSlicerTractographyDisplayConfigure.h.in 
  ${CMAKE_CURRENT_BINARY_DIR}/vtkSlicerTractographyDisplayConfigure.h
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES 
  ${headers} 
  "${CMAKE_CURRENT_BINARY_DIR}/vtkSlicerTractographyDisplayConfigure.h"
  DESTINATION ${Slicer_INSTALL_MODULES_INCLUDE_DIR}/${PROJECT_NAME} COMPONENT Development
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.h")
install(FILES 
  ${headers} 
  DESTINATION ${Slicer_INSTALL_MODULES_INCLUDE_DIR}/${PROJECT_NAME}/Resources COMPONENT Development
  )

# --------------------------------------------------------------------------
# Sources

set(SlicerTractographyDisplay_SRCS
  vtkSlicerTractographyDisplayGUI.cxx
  vtkSlicerAllFiberBundlesDisplayWidget.cxx
  vtkSlicerFiberBundleDisplayWidget.cxx
  #vtkSlicerFiberBundleDisplayLogic.cxx
  #vtkSlicerFiberBundleLogic.cxx
  vtkSlicerDiffusionTensorIcons.cxx
  vtkSlicerTractographyDisplayLogic.cxx
  )

# --------------------------------------------------------------------------
# Wrapping

include("${VTK_CMAKE_DIR}/vtkWrapTcl.cmake")
vtk_wrap_tcl3(SlicerTractographyDisplay 
  SlicerTractographyDisplay_TCL_SRCS 
  "${SlicerTractographyDisplay_SRCS}" "")

#---------------------------------------------------------------------------
# Add Loadable Module support

generatelm(SlicerTractographyDisplay_SRCS SlicerTractographyDisplay.txt)

# --------------------------------------------------------------------------
# Build and install the library

set(lib_name SlicerTractographyDisplay)
add_library(${lib_name}
  ${SlicerTractographyDisplay_SRCS}
  ${SlicerTractographyDisplay_TCL_SRCS}
  )
slicer3_set_modules_output_path(${lib_name})

target_link_libraries(${lib_name}
  ${Slicer_Libs_LIBRARIES}
  ${Slicer_Base_LIBRARIES}
  ${KWWidgets_LIBRARIES}
  ${ITK_LIBRARIES}
  vtkSlicerTractographyDisplayModuleLogic
  )

slicer3_install_modules(${lib_name})
