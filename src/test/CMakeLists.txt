# test.cmake
aux_source_directory(${PROJECT_SOURCE_DIR}/src/test TEST_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils TEST_SRC)

add_executable(test ${TEST_SRC})
set_target_properties(test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)

link_directories("${Python2_LIBRARY_DIRS}")
target_include_directories(test PUBLIC ${Python2_INCLUDE_DIRS})
target_link_libraries(test ${Python2_LIBRARIES})

add_dependencies(test pugixml)
target_link_libraries(test pugixml)

add_dependencies(test pybind11)
target_link_libraries(test pybind11)

add_dependencies(test freetype)
target_link_libraries(test freetype)