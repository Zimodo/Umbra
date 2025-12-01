#include "raylib.h"
#include <math.h>

void drawEarthAndMoon();

const int screenWidth = 800;
const int screenHeight = 800;
float theta = 0;

int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        struct earth{
            Vector2 position = { (float)screenWidth/2, (float)screenHeight/2 };
        };
        
        struct moon{
            float theta;
            float offset;
            Vector2 position;
        };
            
        drawEarthAndMoon();
        
    }

    CloseWindow();

    return 0;
}

void drawEarthAndMoon(){
    
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    
    BeginDrawing();
        // ------------
        // Calculations
        // ------------
        
        // Earth
        
        float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        
        if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
        if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f; 

        earthPosition.x += leftStickX*200*GetFrameTime();
        earthPosition.y += leftStickY*200*GetFrameTime();

        // Moon
        
        float rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float rightStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
        
        // Calculate angle
        
        if (!(rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX && rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY)){
            
            if (rightStickX > 0){
         
            theta = atan( rightStickY/rightStickX );
         
            } else {
            
            theta = atan( rightStickY/rightStickX ) + PI;
            
            }
        }   
        
        Vector2 moonOffset = { cos(theta)*100, sin(theta)*100 };
        Vector2 moonPosition = { earthPosition.x + moonOffset.x, earthPosition.y + moonOffset.y };

        // ----------
        // Debug text
        // ----------

        DrawFPS(10,10);
        
        DrawText(TextFormat("Theta: %f", theta), 200, 80, 20, RED);

        // -------
        // Visuals
        // -------
        
        ClearBackground(BLACK);
        
        // Draw Earth
        DrawCircleV(earthPosition, 50, BLUE);
        
        // Draw Moon
        DrawCircleV(moonPosition, 20, GRAY);
    EndDrawing();
}
