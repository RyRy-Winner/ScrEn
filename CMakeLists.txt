set(CMAKE_POLICY_DEFAULT_CMP0000 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5) # This tells CMake to ignore the <3.5 errors

cmake_minimum_required(VERSION 3.11)
project(screngine)

set(CMAKE_CXX_STANDARD 20)

# 1. Fetch Raylib from GitHub automatically
include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# 2. Add your source code
add_executable(screngine main.cpp
)

# 3. Link Raylib to your app (this handles the GPU/Windowing headers)
target_link_libraries(screngine raylib)

# 4. Windows specific: Hide the console window for a "clean" app feel
set_target_properties(screngine PROPERTIES WIN32_EXECUTABLE ON)
