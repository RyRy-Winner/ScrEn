#include "raylib.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h" // Low-level access to the window manager
#include <thread>
#include <atomic>

#define STEEL CLITERAL(Color){ 50, 50, 60, 255}

// --- SHARED DATA ---
std::atomic<bool> keepRunning(true);
std::atomic<float> sidebarWidth(100.0f);

// --- THE DRAWING LOGIC ---
// Put everything you want to see during the resize in here
void RenderFrame() {
    BeginDrawing();
    ClearBackground(STEEL);

    // Draw using the atomic value from the worker thread
    DrawRectangle(0, 0, (int)sidebarWidth.load(), GetScreenHeight(), CLITERAL(Color){ 60, 60, 65, 255 });

    DrawText("LIVE RESIZE ACTIVE", 250, 200, 20, RAYWHITE);
    EndDrawing();
}

// --- THE CALLBACK ---
// This is executed by the Windows OS itself during the "freeze"
void CustomResizeCallback(GLFWwindow* window, int width, int height) {
    RenderFrame();
}

// --- THE ENGINE THREAD ---
void EngineThread() {
    while (keepRunning) {
        sidebarWidth = GetScreenWidth()/8;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    // 1. Setup
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);
    InitWindow(800, 450, "ScrEngine");

    // 2. The Secret Sauce
    // Get the internal GLFW window handle and attach our callback
    GLFWwindow* handle = glfwGetCurrentContext();
    glfwSetFramebufferSizeCallback(handle, CustomResizeCallback);

    // 3. Start Logic Thread
    std::thread worker(EngineThread);

    // 4. Main Loop
    while (!WindowShouldClose()) {
        RenderFrame();
    }

    // 5. Cleanup
    keepRunning = false;
    if (worker.joinable()) worker.join();
    CloseWindow();

    return 0;
}
