aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include/stb STB_HEADER)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src STB_SRC)

add_library(stb ${STB_SRC} ${STB_HEADER})
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
