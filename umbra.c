#include "raylib.h"
#include <math.h>

const int screenWidth = 800;
const int screenHeight = 800;

struct Earth{
    Vector2 pos;
    int radius;
};

struct Moon{
    Vector2 pos;
    float offset;
    float theta;
    int radius;
};

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateEarthAndMoon(struct Earth* earth, struct Moon* moon);

int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");
    
    struct Earth earth;
    struct Moon moon;

    // init structs
    earth.pos.x = screenWidth/2;
    earth.pos.y = screenHeight/2;
    earth.radius = 50;
    moon.radius = 20;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        updateEarthAndMoon(&earth, &moon);
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        drawEarthAndMoon(&earth, &moon); // pass by reference using pointers
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void updateEarthAndMoon(struct Earth* earth, struct Moon* moon){
    
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    
    // ------------
    // Calculations
    // ------------
    
    // Earth
    
    float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
    
    if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
    if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f; 

    earth->pos.x += leftStickX*200*GetFrameTime();
    earth->pos.y += leftStickY*200*GetFrameTime();
    
    // Screen edge collision
    
    if (earth->pos.x > screenWidth - earth->radius) earth->pos.x = screenWidth - earth->radius;
    if (earth->pos.y > screenHeight - earth->radius) earth->pos.y = screenHeight - earth->radius;
    if (earth->pos.x < earth->radius) earth->pos.x = earth->radius;
    if (earth->pos.y < earth->radius) earth->pos.y = earth->radius;

    // Moon
    
    float rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
    float rightStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
    
    // Calculate angle
    
    if (!(rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX && rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY)){
        
        if (rightStickX > 0){
     
        moon->theta = atan( rightStickY/rightStickX );
     
        } else {
        
        moon->theta = atan( rightStickY/rightStickX ) + PI;
        
        }
    }   
    
    moon->pos.x = earth->pos.x + cos(moon->theta)*100;
    moon->pos.y = earth->pos.y + sin(moon->theta)*100;
}

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon){

    // ----------
    // Debug text
    // ----------

    DrawFPS(10,10);
    
    DrawText(TextFormat("Theta: %f", moon->theta), 200, 80, 20, RED);
    DrawText(TextFormat("Earth posX: %f", earth->pos.x), 200, 100, 20, RED);
    DrawText(TextFormat("Earth posY: %f", earth->pos.y), 200, 120, 20, RED);

    // -------
    // Visuals
    // -------
    
    // Draw Earth
    Vector2 earthPosVector = {earth->pos.x, earth->pos.y};
    DrawCircleV(earthPosVector, earth->radius, BLUE);
    
    // Draw Moon
    Vector2 moonPosVector = {moon->pos.x, moon->pos.y};
    DrawCircleV(moonPosVector, moon->radius, GRAY);
}
