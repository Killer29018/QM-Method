cmake_minimum_required (VERSION 3.18)

# Setup Project
project (QMMethod)

# Set C++ Standard
set (CMAKE_CXX_STANDARD 20)

# Change the output directory
set (binaryDir "/bin")
set (outputDirectory "${PROJECT_SOURCE_DIR}${binaryDir}")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message ("Debug Build")
    set (outputDirectory "${outputDirectory}/debug")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    message ("Release Build")
    set (outputDirectory "${outputDirectory}/release")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${outputDirectory})

# Setup build flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -Wextra -Wpedantic \
-Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable")

# Add all files
file (GLOB_RECURSE projectFiles CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${projectFiles})
