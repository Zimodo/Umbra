#include "raylib.h"
#include <math.h>

const int screenWidth = 800;
const int screenHeight = 800;

struct Earth{
    float posX;
    float posY;
    int radius;
};

struct Moon{
    float posX;
    float posY;
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
    earth.posX = screenWidth/2;
    earth.posY = screenHeight/2;
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

    earth->posX += leftStickX*200*GetFrameTime();
    earth->posY += leftStickY*200*GetFrameTime();
    
    if (earth->posX > screenWidth - earth->radius) earth->posX = screenWidth - earth->radius;
    if (earth->posY > screenHeight - earth->radius) earth->posY = screenHeight - earth->radius;
    if (earth->posX < earth->radius) earth->posX = earth->radius;
    if (earth->posY < earth->radius) earth->posY = earth->radius;

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
    
    moon->posX = earth->posX + cos(moon->theta)*100;
    moon->posY = earth->posY + sin(moon->theta)*100;
}

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon){

    // ----------
    // Debug text
    // ----------

    DrawFPS(10,10);
    
    DrawText(TextFormat("Theta: %f", moon->theta), 200, 80, 20, RED);
    DrawText(TextFormat("Earth posX: %f", earth->posX), 200, 100, 20, RED);
    DrawText(TextFormat("Earth posY: %f", earth->posY), 200, 120, 20, RED);

    // -------
    // Visuals
    // -------
    
    // Draw Earth
    Vector2 earthPosVector = {earth->posX, earth->posY};
    DrawCircleV(earthPosVector, earth->radius, BLUE);
    
    // Draw Moon
    Vector2 moonPosVector = {moon->posX, moon->posY};
    DrawCircleV(moonPosVector, moon->radius, GRAY);
}
