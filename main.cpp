#include "raylib.h"
#include <cmath>

#if defined(PLATFORM_DESKTOP)
    #define GLFW_INCLUDE_NONE
    #include "GLFW/glfw3.h"
#endif

struct ScrEnState {
    bool isDragging;
    bool isResizing;
    bool keepRunning;
    int resizeEdge;
    // Anchor points to prevent "drift"
    Vector2 initialWinPos;
    Vector2 initialSize;
    Vector2 initialMousePos;
};

void RunEngineFrame(ScrEnState* state) {
    GLFWwindow* window = glfwGetCurrentContext();
    int leftMouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    double localX, localY;
    glfwGetCursorPos(window, &localX, &localY);

    int wx, wy, ww, wh;
    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);

    Vector2 gMouse = {(float)(wx + localX), (float)(wy + localY)};
    Vector2 delta = GetMouseDelta();
    Vector2 mouse = {(float)localX, (float)localY};

    // --- 1. THE CLOSE BUTTON (Refined) ---
    Rectangle closeRect = { (float)ww - 40, 0, 40, 40 };
    bool overClose = CheckCollisionPointRec(mouse, closeRect);

    // CHANGE: Use Raylib's IsMouseButtonPressed so it only triggers ONCE per click
    // and only if the mouse is actually over the button at that moment.
    if (overClose) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND); // Feedback for the button
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->keepRunning = false;
        }
    }

    // --- 2. EDGE DETECTION ---
    int b = 10;
    bool edgeTop    = (mouse.y >= 0 && mouse.y < b);
    bool edgeBottom = (mouse.y > wh - b && mouse.y <= wh);
    bool edgeLeft   = (mouse.x >= 0 && mouse.x < b);
    bool edgeRight  = (mouse.x > ww - b && mouse.x <= ww);

    // --- 3. CURSOR & STATE INITIALIZATION ---
    if (!state->isResizing && !state->isDragging) {
        // If not over the Close button, handle resize cursors
        if (!overClose) {
            if ((edgeTop && edgeLeft) || (edgeBottom && edgeRight)) SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
            else if ((edgeTop && edgeRight) || (edgeBottom && edgeLeft)) SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
            else if (edgeTop || edgeBottom) SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
            else if (edgeLeft || edgeRight) SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
            else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Trigger Resize
        if (leftMouse == GLFW_PRESS && (edgeTop || edgeBottom || edgeLeft || edgeRight)) {
            state->isResizing = true;
            state->resizeEdge = (edgeTop ? 1 : 0) | (edgeBottom ? 2 : 0) | (edgeLeft ? 4 : 0) | (edgeRight ? 8 : 0);
            state->initialWinPos = {(float)wx, (float)wy};
            state->initialSize = {(float)ww, (float)wh};
            state->initialMousePos = gMouse;
        }
        // Trigger Drag (Only if NOT resizing and NOT over the Close button)
        else if (leftMouse == GLFW_PRESS && mouse.y <= 40 && !overClose) {
            state->isDragging = true;
        }
    }

    // --- 4. RESIZE EXECUTION ---
    if (state->isResizing) {
        // Keep the appropriate resize cursor visible while dragging the edge
        if (state->resizeEdge & 1 || state->resizeEdge & 2) SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
        if (state->resizeEdge & 4 || state->resizeEdge & 8) SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        if ((state->resizeEdge & 1 && state->resizeEdge & 4)) SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);

        int nx = (int)state->initialWinPos.x, ny = (int)state->initialWinPos.y;
        int nw = (int)state->initialSize.x, nh = (int)state->initialSize.y;
        float dx = gMouse.x - state->initialMousePos.x;
        float dy = gMouse.y - state->initialMousePos.y;

        if (state->resizeEdge & 8) nw = (int)state->initialSize.x + (int)dx;
        if (state->resizeEdge & 2) nh = (int)state->initialSize.y + (int)dy;
        if (state->resizeEdge & 4) { nx = (int)state->initialWinPos.x + (int)dx; nw = (int)state->initialSize.x - (int)dx; }
        if (state->resizeEdge & 1) { ny = (int)state->initialWinPos.y + (int)dy; nh = (int)state->initialSize.y - (int)dy; }

        if (nw > 300 && nh > 200) {
            SetWindowPosition(nx, ny);
            SetWindowSize(nw, nh);
        }
        if (leftMouse == GLFW_RELEASE) state->isResizing = false;
    }

    // --- 5. DRAGGING ---
    if (state->isDragging) {
        SetWindowPosition(wx + (int)delta.x, wy + (int)delta.y);
        if (leftMouse == GLFW_RELEASE) state->isDragging = false;
    }

    // --- DRAWING ---
    BeginDrawing();
        ClearBackground((Color){ 20, 20, 25, 255 });
        // (Grid drawing code stays the same...)
        for (int i = 0; i < ww; i += 40) DrawLine(i, 0, i, wh, (Color){ 40, 40, 45, 255 });
        for (int i = 0; i < wh; i += 40) DrawLine(0, i, ww, i, (Color){ 40, 40, 45, 255 });

        // Header
        DrawRectangle(0, 0, ww, 40, (Color){ 45, 45, 50, 255 });
        if (overClose) DrawRectangleRec(closeRect, MAROON);
        DrawText("X", ww - 28, 10, 20, WHITE);

        // Sidebar & UI (Existing code...)
        DrawRectangle(ww - 180, 40, 180, wh - 40, (Color){ 30, 30, 35, 255 });
        DrawCircle(ww/2, wh/2, 40, state->isResizing ? SKYBLUE : (state->isDragging ? LIME : MAROON));

        DrawFPS(10, wh - 25);
    EndDrawing();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 720, "ScrEngine");
    SetTargetFPS(144);
    ScrEnState state = { 0 };
    state.keepRunning = true;
    while (state.keepRunning && !WindowShouldClose()) {
        PollInputEvents();
        RunEngineFrame(&state);
    }
    CloseWindow();
    return 0;
}
