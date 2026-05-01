<<<<<<< Updated upstream
cmake_minimum_required(VERSION 3.11...4.2) # Use a range to support modern features while allowing older ones

# THIS IS THE FIX: Tell CMake to ignore the <3.5 errors in Raylib
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(screngine)
=======
cmake_minimum_required(VERSION 3.14)

# THE FIX: Tells CMake 4.0+ not to panic about Raylib 5.0's older version tag
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
>>>>>>> Stashed changes

project(ScrEngine CXX)

<<<<<<< Updated upstream
# 1. Fetch Raylib
=======
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Raylib (Using the direct URL bypass since it worked for you)
>>>>>>> Stashed changes
include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.zip
)
FetchContent_MakeAvailable(raylib)

<<<<<<< Updated upstream
# 2. Define the app FIRST
add_executable(screngine main.cpp)

# 3. Include directories so the compiler finds GLFW
target_include_directories(screngine PRIVATE
        ${raylib_SOURCE_DIR}/src
        ${raylib_SOURCE_DIR}/src/external/glfw/include
=======
# Define the Executable
set(SOURCES
        main.cpp
        windowSettings.cpp
        windowSettings.h
        bridge.h
        program.cpp
>>>>>>> Stashed changes
)
add_executable(${PROJECT_NAME} ${SOURCES})

<<<<<<< Updated upstream
# 4. Link and set Windows properties
target_link_libraries(screngine raylib)
set_target_properties(screngine PROPERTIES WIN32_EXECUTABLE OFF)
=======
# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Platform-specific tweaks
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
>>>>>>> Stashed changes
