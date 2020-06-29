cmake_minimum_required(VERSION 3.8)

project(HEngine)

aux_source_directory(${PROJECT_SOURCE_DIR}/src ALL_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/sb7 ALL_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/model_loader ALL_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/GLObject ALL_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils ALL_SRC)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/sb7)

find_package(Python2 COMPONENTS Interpreter Development)
include_directories(${Python2_INCLUDE_DIRS})
link_directories(${Python2_LIBRARY_DIRS})
MESSAGE(STATUS "${Python2_LIBRARIES} ${Python2_LIBRARY_DIRS}")

link_libraries(opengl32)

add_executable(engine WIN32 ${ALL_SRC})
set_target_properties(engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)

add_subdirectory(3rd_party/glfw-3.3.2)
add_dependencies(engine glfw)
target_link_libraries(engine glfw)

add_subdirectory(src/test)
