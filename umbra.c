#include "raylib.h"
#include <math.h>

const int screenWidth = 800;
const int screenHeight = 800;

struct Earth{
    Vector2 pos;
    int speed;
    int radius;
};

struct Moon{
    Vector2 pos;
    float offset;
    float theta;
    int radius;
    int orbit;
};

// this struct is broken rn
/*
umbra.c:138:1: error: number of arguments doesn't match prototype
  138 | void updateBulletSpawner(){
      | ^~~~
umbra.c:37:6: error: prototype declaration
   37 | void updateBulletSpawner(struct Spawner* spawner);
      |      ^~~~~~~~~~~~~~~~~~~
umbra.c:142:12: error: 'spawner' undeclared (first use in this function); did you mean 'Spawner'?
  142 |     switch(spawner.side){
      |            ^~~~~~~
      |            Spawner
umbra.c:142:12: note: each undeclared identifier is reported only once for each function it appears in
umbra.c:146:13: error: 'pos' undeclared (first use in this function); did you mean 'pow'?
  146 |             pos.x = pos.x + speed.x;
      |             ^~~
      |             pow
*/

struct Spawner{
    Vector2 pos;
    int side;
};

/*
typedef struct Bullet {
    Vector2 position;       // Bullet position on screen
    Vector2 acceleration;   // Amount of pixels to be incremented to position every frame
    bool disabled;          // Skip processing and draw case out of screen
    Color color;            // Bullet color
} Bullet;
*/

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateBulletSpawner(struct Spawner* spawner);
void updateBullets();

int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");
    
    struct Earth earth;
    struct Moon moon;
    struct Spawner spawner;

    // Init Earth and Moon
    earth.pos.x = screenWidth/2;
    earth.pos.y = screenHeight/2;
    earth.radius = 50;
    earth.speed = 200;
    moon.radius = 20;
    moon.orbit = 100;
    
    // Init Spawner
    spawner.pos.x = screenWidth + 200.0;
    spawner.pos.y = screenHeight + 200.0;
    spawner.side = 1;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        updateEarthAndMoon(&earth, &moon);
        void updateBullets();
        
        BeginDrawing();
        
            ClearBackground(BLACK);
            drawEarthAndMoon(&earth, &moon); // pass by reference using pointers
            
            updateBulletSpawner(&spawner); // take out of drawing phase later

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

    earth->pos.x += leftStickX*earth->speed*GetFrameTime();
    earth->pos.y += leftStickY*earth->speed*GetFrameTime();
    
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
    
    moon->pos.x = earth->pos.x + cos(moon->theta)*moon->orbit;
    moon->pos.y = earth->pos.y + sin(moon->theta)*moon->orbit;
}


void updateBulletSpawner(){
    
    float carry;
    
    switch(spawner.side){
        case 1:
            
            Vector2 speed = { 3.0f, 0.0f };
            pos.x = pos.x + speed.x;
            pos.y = pos.y + speed.y;
            
            if (pos.x > 600.0) {
                spawner->side++;
                carry = pos.x - 600.0;
                pos.x = 600.0;
                pos.y = 200.0 + carry;
            }
            
            // hi :)
            break;
        case 2:
            // hi :)
            break;
        case 3:
            // hi :)
            break;
        case 4:
            // hi :)
            break;
    }
    
    DrawCircle(pos.x,pos.y,20,WHITE);
}

void updateBullets(){
    
}

void drawEarthAndMoon(struct Earth* earth, struct Moon* moon){

    // ----------
    // Debug text
    // ----------

    DrawFPS(10,10);
    
    //DrawText(TextFormat("Theta: %f", moon->theta), 200, 80, 20, RED);
    //DrawText(TextFormat("Earth posX: %f", earth->pos.x), 200, 100, 20, RED);
    //DrawText(TextFormat("Earth posY: %f", earth->pos.y), 200, 120, 20, RED);

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
