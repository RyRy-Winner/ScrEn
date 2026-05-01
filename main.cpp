#include "raylib.h"
<<<<<<< Updated upstream
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
=======
#include <math.h>
#include "windowSettings.h"
#include "bridge.h"

// User-Facing Functions
void UserUpdate();
void UserDraw();

// --- OS HARDWARE CURSOR BYPASS ---
#if defined(_WIN32)
extern "C" { __declspec(dllimport) int __stdcall GetCursorPos(long* lpPoint); }
#endif

Vector2 GetAbsoluteMousePos(Vector2 winPos, Vector2 localMouse) {
#if defined(_WIN32)
    long p[2]; GetCursorPos(p);
    return (Vector2){ (float)p[0], (float)p[1] };
#else
    return (Vector2){ winPos.x + localMouse.x, winPos.y + localMouse.y };
#endif
}

// --- ENGINE STATE ---
struct ScrEnState {
    // 1. Logic Flags
    bool isDragging, isResizing, isMaximized, keepRunning;
    int resizeEdge;

    // 2. UI Metrics
    float dpiScale, borderThickness, titleHeight;
    int gridSpacing;

    // 3. Rectangles (All grouped)
    Rectangle prevRect;
    Rectangle activeWorkArea;
    Rectangle contentArea;
    Rectangle dragStartRect;

    // 4. Vectors (All grouped)
    Vector2 dragOffset;
    Vector2 dragStartMouse;

    // 5. Integers/Booleans
    int activeMonitor;
    bool closePressed, maxPressed, minPressed;

} state = {
    // 1. Logic Flags
    false, false, false, true, 0,
    // 2. UI Metrics
    1.0f, 12.0f, 40.0f, 40,
    // 3. Rectangles
    { 100, 100, 1280, 720 }, // prevRect
    { 0, 0, 0, 0 },          // activeWorkArea
    { 0, 0, 0, 0 },          // contentArea
    { 0, 0, 0, 0 },          // dragStartRect
    // 4. Vectors
    { 0, 0 },                // dragOffset
    { 0, 0 },                // dragStartMouse
    // 5. Final pieces
    -1,                      // activeMonitor
    false, false, false      // Buttons
};

// --- HELPER: Priority Resize Check ---
bool IsMouseInResizeZone(Vector2 mouse, int w, int h) {
    return (mouse.x < state.borderThickness || mouse.x > w - state.borderThickness ||
            mouse.y < state.borderThickness || mouse.y > h - state.borderThickness);
}

// --- ENGINE LOGIC ---

void HandleWindowControls(Vector2 mouse, int w, int h, Vector2 winPos) {
    if (IsMouseInResizeZone(mouse, w, h) || state.isResizing) {
        state.closePressed = state.maxPressed = state.minPressed = false;
        return;
    }

    float btnW = 46.0f * state.dpiScale;
    Rectangle closeBtn = { (float)w - btnW, 0, btnW, state.titleHeight };
    Rectangle maxBtn = { (float)w - (btnW * 2), 0, btnW, state.titleHeight };
    Rectangle minBtn = { (float)w - (btnW * 3), 0, btnW, state.titleHeight };

    bool overClose = CheckCollisionPointRec(mouse, closeBtn);
    bool overMax = CheckCollisionPointRec(mouse, maxBtn);
    bool overMin = CheckCollisionPointRec(mouse, minBtn);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
       state.activeMonitor = GetCurrentMonitor();
       Vector2 monPos = GetMonitorPosition(state.activeMonitor);
       state.activeWorkArea = { monPos.x, monPos.y, (float)GetMonitorWidth(state.activeMonitor), (float)GetMonitorHeight(state.activeMonitor) };
       if (overClose) state.closePressed = true;
       else if (overMax) state.maxPressed = true;
       else if (overMin) state.minPressed = true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (state.closePressed && overClose) state.keepRunning = false;
        else if (state.minPressed && overMin) MinimizeWindow();
        else if (state.maxPressed && overMax) {
            state.isDragging = false;
            state.isResizing = false;
            if (state.isMaximized) {
                SetWindowSize((int)state.prevRect.width, (int)state.prevRect.height);
                SetWindowPosition((int)state.prevRect.x, (int)state.prevRect.y);
                state.isMaximized = false;
            } else {
                state.prevRect = { winPos.x, winPos.y, (float)w, (float)h };
                SetWindowPosition((int)state.activeWorkArea.x, (int)state.activeWorkArea.y);
                SetWindowSize((int)state.activeWorkArea.width, (int)state.activeWorkArea.height);
                state.isMaximized = true;
            }
        }
        state.closePressed = state.maxPressed = state.minPressed = false;
    }
}

void HandleTransformations(Vector2 mouse, int w, int h, Vector2 winPos) {
    if (state.isMaximized) return;
    float btnW = 46.0f * state.dpiScale;
    bool edgeT = (mouse.y < state.borderThickness);
    bool edgeB = (mouse.y > h - state.borderThickness);
    bool edgeL = (mouse.x < state.borderThickness);
    bool edgeR = (mouse.x > w - state.borderThickness);
    bool overTitleBar = (mouse.y < state.titleHeight && mouse.x < w - (btnW * 3));

    if (!state.isResizing && !state.isDragging) {
       if ((edgeT && edgeL) || (edgeB && edgeR)) SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
       else if ((edgeT && edgeR) || (edgeB && edgeL)) SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
       else if (edgeT || edgeB) SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
       else if (edgeL || edgeR) SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
       else if (overTitleBar) SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
       else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
       Vector2 sMouse = GetAbsoluteMousePos(winPos, mouse);
       if (edgeT || edgeB || edgeL || edgeR) {
          state.isResizing = true;
          state.resizeEdge = (edgeT ? 1 : 0) | (edgeB ? 2 : 0) | (edgeL ? 4 : 0) | (edgeR ? 8 : 0);
          state.dragStartRect = { winPos.x, winPos.y, (float)w, (float)h };
          state.dragStartMouse = sMouse;
       } else if (overTitleBar && !state.closePressed && !state.maxPressed && !state.minPressed) {
          state.isDragging = true;
          state.dragOffset = mouse;
       }
    }
    if (state.isDragging) {
       Vector2 sMouse = GetAbsoluteMousePos(winPos, mouse);
       SetWindowPosition((int)(sMouse.x - state.dragOffset.x), (int)(sMouse.y - state.dragOffset.y));
    }
    if (state.isResizing) {
       Vector2 sMouse = GetAbsoluteMousePos(winPos, mouse);
       float dx = sMouse.x - state.dragStartMouse.x; float dy = sMouse.y - state.dragStartMouse.y;
       int tw = (int)state.dragStartRect.width; int th = (int)state.dragStartRect.height;
       int tx = (int)state.dragStartRect.x; int ty = (int)state.dragStartRect.y;
       if (state.resizeEdge & 8) tw += (int)dx; if (state.resizeEdge & 2) th += (int)dy;
       if (state.resizeEdge & 4) { tw -= (int)dx; tx += (int)dx; }
       if (state.resizeEdge & 1) { th -= (int)dy; ty += (int)dy; }
       if (tw < 400) tw = 400; if (th < 300) th = 300;
       SetWindowSize(tw, th); SetWindowPosition(tx, ty);
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) state.isDragging = state.isResizing = false;
}

void UpdateEngine() {
    int w = GetScreenWidth(); int h = GetScreenHeight();
    Vector2 mouse = GetMousePosition(); Vector2 winPos = GetWindowPosition();
    state.dpiScale = GetWindowScaleDPI().x;
    state.gridSpacing = (int)(40 * state.dpiScale);
    state.titleHeight = 40.0f * state.dpiScale;
    state.borderThickness = 12.0f * state.dpiScale;
    state.contentArea = { 0, state.titleHeight, (float)w, (float)h - state.titleHeight };

    HandleWindowControls(mouse, w, h, winPos);
    HandleTransformations(mouse, w, h, winPos);
    UserUpdate();
}

void DrawEngine() {
    int w = GetScreenWidth(); int h = GetScreenHeight();
    Vector2 m = GetMousePosition(); float s = state.dpiScale;

    BeginDrawing();
    ClearBackground(config.backgroundColor);

    for (int i = 0; i < w; i += state.gridSpacing) DrawLine(i, (int)state.titleHeight, i, h, config.gridColor);
    for (int i = (int)state.titleHeight; i < h; i += state.gridSpacing) DrawLine(0, i, w, i, config.gridColor);

    BeginScissorMode(0, (int)state.titleHeight, w, h - (int)state.titleHeight);
    UserDraw();
    EndScissorMode();

    // --- TITLE BAR ---
    DrawRectangle(0, 0, w, (int)state.titleHeight, config.titleBarColor);

    // --- TITLE TEXT ---
    int fontSize = (int)(20 * s);
    int textWidth = MeasureText(config.windowTitle, fontSize);
    float btnW = 46.0f * s;
    float sepX = (w - (btnW * 3)) - (8 * s);
    float idealX = (w / 2.0f) - (textWidth / 2.0f);
    float maxX = sepX - (12 * s) - textWidth;
    float textX = (idealX < maxX) ? idealX : maxX;
    DrawText(config.windowTitle, (int)textX, (int)(10 * s), fontSize, config.titleTextColor);

    // --- BUTTONS & ICONS ---
    DrawLineEx({sepX, 12*s}, {sepX, state.titleHeight - 12*s}, 1.0f, Fade(config.titleTextColor, 0.2f));
    Rectangle closeR = { (float)w - btnW, 0, btnW, state.titleHeight };
    Rectangle maxR = { (float)w - (btnW * 2), 0, btnW, state.titleHeight };
    Rectangle minR = { (float)w - (btnW * 3), 0, btnW, state.titleHeight };

    if (!IsMouseInResizeZone(m, w, h)) {
        if (CheckCollisionPointRec(m, closeR)) DrawRectangleRec(closeR, state.closePressed ? Fade(config.closeHoverColor, 0.8f) : Fade(config.closeHoverColor, 0.5f));
        if (CheckCollisionPointRec(m, maxR)) DrawRectangleRec(maxR, state.maxPressed ? Fade(config.maxHoverColor, 0.6f) : Fade(config.maxHoverColor, 0.3f));
        if (CheckCollisionPointRec(m, minR)) DrawRectangleRec(minR, state.minPressed ? Fade(config.minHoverColor, 0.6f) : Fade(config.minHoverColor, 0.3f));
    }

    float iS = roundf(5.0f * s); float cx = roundf(closeR.x + btnW/2); float cy = roundf(state.titleHeight/2);
    DrawLineEx({cx-iS, cy-iS}, {cx+iS, cy+iS}, 1.3f, config.iconColor);
    DrawLineEx({cx+iS, cy-iS}, {cx-iS, cy+iS}, 1.3f, config.iconColor);
    cx = roundf(maxR.x + btnW/2);
    if (!state.isMaximized) DrawRectangleLinesEx({cx-iS, cy-iS, iS*2, iS*2}, 1.3f, config.iconColor);
    else {
        DrawRectangleLinesEx({cx-iS+2*s, cy-iS-2*s, iS*2, iS*2}, 1.3f, config.iconColor);
        DrawRectangleRec({cx-iS-s, cy-iS+s, iS*2, iS*2}, config.titleBarColor);
        DrawRectangleLinesEx({cx-iS-s, cy-iS+s, iS*2, iS*2}, 1.3f, config.iconColor);
    }
    cx = roundf(minR.x + btnW/2);
    DrawLineEx({cx-iS, cy}, {cx+iS, cy}, 1.3f, config.iconColor);

    DrawFPS(10, h - (int)(25 * s));
>>>>>>> Stashed changes
    EndDrawing();
}

int main() {
<<<<<<< Updated upstream
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 720, "ScrEngine");
    SetTargetFPS(144);
    ScrEnState state = { 0 };
    state.keepRunning = true;
    while (state.keepRunning && !WindowShouldClose()) {
        PollInputEvents();
        RunEngineFrame(&state);
    }
=======
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, config.windowTitle);

    if (FileExists(config.iconPath)) {
        Image icon = LoadImage(config.iconPath); SetWindowIcon(icon); UnloadImage(icon);
    }

    while (state.keepRunning && !WindowShouldClose()) { UpdateEngine(); DrawEngine(); }
>>>>>>> Stashed changes
    CloseWindow();
    return 0;
}