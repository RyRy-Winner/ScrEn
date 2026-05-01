#include "raylib.h"
#include "bridge.h"
#include <vector>

// --- SHARED STATE ---
// Because logic and drawing are in the same file,
// they can both see these variables easily.
struct Entity {
    Vector2 position;
    Color color;
    float radius;
};

std::vector<Entity> shapes;
float rotation = 0.0f;

// Called 1st (Used for beforehand calculations)
// --- LOGIC ---
void UserUpdate()
{

}

// Called 2nd (Draws to the screen)
// --- VISUALS ---
void UserDraw()
{

}