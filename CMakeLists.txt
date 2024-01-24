cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Simulation) # Name simulation

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/Headers)
file(GLOB sources ${PROJECT_SOURCE_DIR}/Source/*.cc)
file(GLOB headers ${PROJECT_HEADERS_DIR}/Headers/*.hh) # HEADRS por SOURCE

file(GLOB MACRO_FILES
	"*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(sim Source/sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS sim)