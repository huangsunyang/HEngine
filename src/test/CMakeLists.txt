aux_source_directory(${PROJECT_SOURCE_DIR}/src/test TEST_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils TEST_SRC)
add_executable(test ${TEST_SRC})