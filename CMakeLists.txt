cmake_minimum_required(VERSION 3.16)

project(notex VERSION 0.1 LANGUAGES C CXX)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCLUDE_FILES
    app.hpp
    color.hpp
    figure.hpp
    geometry.hpp
    graphics.hpp
    keys.hpp
    messages.hpp
    resources.hpp
)

set(SRC_FILES
    app.cpp
    color.cpp
    figure.cpp
    geometry.cpp
    graphics.cpp    
    main.cpp
    messages.cpp
    resources.cpp
)

set(EXTERNAL_FILES
    glad/src/gl.c
    tinyfiledialogs/tinyfiledialogs.c
)

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)
list(TRANSFORM EXTERNAL_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/external/)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    # No console
    add_executable(notex WIN32 ${INCLUDE_FILES} ${SRC_FILES} ${EXTERNAL_FILES})
    # Static linking
    set_property(TARGET notex PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    # With console
    add_executable(notex ${INCLUDE_FILES} ${SRC_FILES} ${EXTERNAL_FILES})
    # DEBUG definition
    target_compile_definitions(notex PRIVATE DEBUG)
endif()

target_include_directories(notex
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        external/glfw/include
        external/glad/include
        external/tinyfiledialogs
)

target_link_directories(notex
    PRIVATE
        external/glfw/lib
)

target_link_libraries(notex
    glfw3_mt
)