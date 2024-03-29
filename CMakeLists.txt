cmake_minimum_required(VERSION 3.8)

project(HEngine)

file(GLOB_RECURSE ALL_SRC RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")

add_executable(engine WIN32 ${ALL_SRC} src/camera/CameraManager.cpp src/camera/CameraManager.h)
add_custom_command(TARGET engine POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:engine> ${PROJECT_SOURCE_DIR})
set_target_properties(engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)

target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/src)

# add python lib/include dir
find_package(Python2 COMPONENTS Interpreter Development)
link_directories("${Python2_LIBRARY_DIRS}")
target_include_directories(engine PUBLIC ${Python2_INCLUDE_DIRS})
target_link_libraries(engine ${Python2_LIBRARIES})
MESSAGE(STATUS "${Python2_LIBRARIES} ${Python2_LIBRARY_DIRS}")

add_subdirectory(3rd_party/glfw-3.3.2)
add_dependencies(engine glfw)
target_link_libraries(engine glfw)

add_subdirectory(3rd_party/freetype-2.10.2)
add_dependencies(engine freetype)
target_link_libraries(engine freetype)

add_subdirectory(3rd_party/stb)
add_dependencies(engine stb)
target_link_libraries(engine stb)

add_subdirectory(3rd_party/sb7)
add_dependencies(engine sb7)
target_link_libraries(engine sb7)

add_subdirectory(3rd_party/pybind11)
target_link_libraries(engine pybind11)

add_subdirectory(3rd_party/pugixml)
add_dependencies(engine pugixml)
target_link_libraries(engine pugixml)

include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/3rd_party/rapidjson/include)
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/3rd_party/glm-master)

target_link_libraries(engine opengl32)

add_subdirectory(src/test)
