#cmake_minimum_required(VERSION 3.7)

#project(program)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIRECTORY src)

set(Headers
 include/Json.h
 include/fileReader.h
)

set(Sources
	${SOURCE_DIRECTORY}/main.cpp
	${SOURCE_DIRECTORY}/Json.cpp
	${SOURCE_DIRECTORY}/fileReader.cpp
)

add_library(json STATIC ${Sources} ${Headers})
target_include_directories(json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

