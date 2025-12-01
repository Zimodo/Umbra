#include "raylib.h"
#include <math.h>



const int screenWidth = 800;
const int screenHeight = 800;
float theta = 0;
struct Earth{
    float posX;// = screenWidth/2;
    float posY;// = screenHeight/2;
};

struct Moon{
    float posX;
    float posY;
    float offset;
    float theta;
};
void drawEarthAndMoon(struct Earth* earth, struct Moon* moon);



int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");
    
    struct Earth earth;
    struct Moon moon;

    // init structs
    earth.posX = screenWidth/2;
    earth.posY = screenHeight/2;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        drawEarthAndMoon(&earth, &moon); // pass by reference using pointers
        
    }

    CloseWindow();

    return 0;
}

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon){
    
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

        earth->posX += leftStickX*200*GetFrameTime();
        earth->posY += leftStickY*200*GetFrameTime();

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
        Vector2 moonPosition = { earth->posX + moonOffset.x, earth->posY + moonOffset.y };

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
        Vector2 earthPosVector = {earth->posX, earth->posY};
        DrawCircleV(earthPosVector, 50, BLUE);
        
        // Draw Moon
        DrawCircleV(moonPosition, 20, GRAY);
    EndDrawing();
}
