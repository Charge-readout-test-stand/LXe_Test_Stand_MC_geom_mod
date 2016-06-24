#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(nEXO)

#add_compile_options(-std=c++11)
#----------------------------------------------------------------------------
# Compilar flags
#if(CMAKE_COMPILER_IS_GNUCXX)
#  set(CMAKE_CXX_FLAGS "-O2")
#endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add ROOT header files
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
#message(${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
find_package(VGM REQUIRED)
include_directories(${VGM_INCLUDE_DIRS})
#message(${VGM_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and ROOT libraries
#
add_executable(nEXO nEXO.cc ${sources} ${headers})
target_link_libraries(nEXO ${Geant4_LIBRARIES} ${VGM_LIBRARIES} ${ROOT_LIBRARIES} -lGeom)

#----------------------------------------------------------------------------
# Check if using variable G4_V10
if($ENV{G4_V10})
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DG4_V10")
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build nEXO. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(nEXO_SCRIPTS
  nEXO.in
  SimulationScripts
  SimulationExamples
  )

foreach(_script ${nEXO_SCRIPTS})
  if(NOT IS_DIRECTORY  ${PROJECT_SOURCE_DIR}/${_script})
      configure_file(
      ${PROJECT_SOURCE_DIR}/${_script}
      ${PROJECT_BINARY_DIR}/${_script}
      COPYONLY
      )
  else(NOT IS_DIRECTORY  ${PROJECT_SOURCE_DIR}/${_script})
      file(GLOB templateFiles RELATIVE ${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_SOURCE_DIR}/${_script}/*)
      foreach(templateFile ${templateFiles})
          if(NOT IS_DIRECTORY  ${PROJECT_SOURCE_DIR}/${_script}/${templateFile})
              configure_file(
              ${PROJECT_SOURCE_DIR}/${_script}/${templateFile}
              ${PROJECT_BINARY_DIR}/${_script}/${templateFile}
              COPYONLY
              )
          endif(NOT IS_DIRECTORY   ${PROJECT_SOURCE_DIR}/${_script}/${templateFile})
      endforeach(templateFile)
  endif(NOT IS_DIRECTORY   ${PROJECT_SOURCE_DIR}/${_script})
endforeach()



#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
#add_custom_target(nEXO DEPENDS nEXO)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS nEXO DESTINATION bin)

