aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include SB7_HEADER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SB7_SRC)

add_library(sb7 ${SB7_SRC})
add_dependencies(sb7 glfw)
target_link_libraries(sb7 glfw)
target_include_directories(sb7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sb7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/sb7)
