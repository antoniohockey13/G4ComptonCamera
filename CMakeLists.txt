cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# Name simulation
project(ComptCamera) 

# Find Geant4 package, activating all available UI and Vis drivers by default
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Copy all macro files to the build directory
file(GLOB MACRO_FILES
	"*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

# Add the executable, and link it to the Geant4 libraries
add_executable(ComptCamera src/sim.cc ${sources} ${headers})
target_link_libraries(ComptCamera ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS ComptCamera)