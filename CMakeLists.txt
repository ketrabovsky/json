cmake_minimum_required(VERSION 3.7)

project(program)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(googletest)

set(Headers
    fileReader.h
    Json.h
)

set(Sources
    main.cpp
    Json.cpp
    fileReader.cpp
)
enable_testing()

add_library(program STATIC ${Sources} ${Headers})

add_subdirectory(test)

add_executable(programBinary ${Sources} ${Headers})
