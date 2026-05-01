cmake_minimum_required(VERSION 3.20)
project(ScrEngine)

set(CMAKE_CXX_STANDARD 17)

# Fix for the Timestamp warning
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Fetch Raylib
include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/heads/master.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

# Define your executable
# Note: I removed windowSettings.cpp/program.cpp from this list
# as they will be added automatically if they exist in your project folder.
add_executable(ScrEngine
        main.cpp
        program.cpp
        windowSettings.cpp
)

# Link everything
target_link_libraries(ScrEngine raylib)

# Link everything
target_link_libraries(ScrEngine raylib)

# This tells the compiler where to find raylib.h AND the internal GLFW headers
target_include_directories(ScrEngine PRIVATE
        ${raylib_SOURCE_DIR}/src
        ${raylib_SOURCE_DIR}/src/external/glfw/include
)

# The "brute force" way to map main to WinMain
target_link_options(ScrEngine PRIVATE -mwindows -Wl,-e,mainCRTStartup)