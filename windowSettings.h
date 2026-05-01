#pragma once
#include "raylib.h"

struct WindowSettings {
    const char* windowTitle;
    const char* iconPath; // <--- Path to your .png or .ico

    Color backgroundColor;
    Color gridColor;
    Color titleBarColor;
    Color titleTextColor;

    Color iconColor;
    Color closeHoverColor;
    Color maxHoverColor;
    Color minHoverColor;
};

extern WindowSettings config;