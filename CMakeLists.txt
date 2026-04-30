cmake_minimum_required(VERSION 3.11...4.2) # Use a range to support modern features while allowing older ones

# THIS IS THE FIX: Tell CMake to ignore the <3.5 errors in Raylib
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(screngine)

set(CMAKE_CXX_STANDARD 20)

# 1. Fetch Raylib
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# 2. Define the app FIRST
add_executable(screngine main.cpp)

# 3. Include directories so the compiler finds GLFW
target_include_directories(screngine PRIVATE
        ${raylib_SOURCE_DIR}/src
        ${raylib_SOURCE_DIR}/src/external/glfw/include
)

# 4. Link and set Windows properties
target_link_libraries(screngine raylib)
set_target_properties(screngine PROPERTIES WIN32_EXECUTABLE OFF)
