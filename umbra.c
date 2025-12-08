#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define MAX_BULLETS 500000

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

struct Spawner{
    Vector2 pos;
    int side;
    Vector2 accel;
    float spawnCooldown;
};

typedef struct Bullet {
    Vector2 position;       // Bullet position on screen
    Vector2 acceleration;   // Amount of pixels to be incremented to position every frame
    bool disabled;          // Skip processing and draw case out of screen
    Color color;            // Bullet color
} Bullet;


void drawEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateBulletSpawner(struct Spawner* spawner);
void updateBullets(float bullets[]);

int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");

    Bullet *bullets = (Bullet *)RL_CALLOC(MAX_BULLETS, sizeof(Bullet)); // Bullets array
    int bulletCount = 0;
    int bulletDisabledCount = 0; // Used to calculate how many bullets are on screen
    int bulletRadius = 10;
    float bulletSpeed = 3.0f;
    int bulletRows = 6;
    Color bulletColor[2] = { RED, BLUE };
    
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
    spawner.pos.x = 200.0;
    spawner.pos.y = 200.0;
    spawner.side = 1;
    spawner.accel.x = 3.0f;
    spawner.accel.y = 0.0f;
    spawner.spawnCooldown = 5.0f;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        updateEarthAndMoon(&earth, &moon);

        updateBulletSpawner(&spawner); // take out of drawing phase later

        updateBullets();
        
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

void updateBulletSpawner(struct Spawner* spawner){
    
    float carry;
    
    spawner->pos.x = spawner->pos.x + spawner->accel.x;
    spawner->pos.y = spawner->pos.y + spawner->accel.y;
            
    switch(spawner->side){
        case 1:
            if (spawner->pos.x > 600.0) {
                spawner->side++;
                carry = spawner->pos.x - 600.0;
                spawner->pos.x = 600.0;
                spawner->pos.y = 200.0 + carry;
                spawner->accel.x = 0.0f;
                spawner->accel.y = 3.0f;
            }
            break;
        case 2:
            if (spawner->pos.y > 600.0) {
                spawner->side++;
                carry = spawner->pos.y - 600.0;
                spawner->pos.x = 600.0 - carry;
                spawner->pos.y = 600.0;
                spawner->accel.x = -3.0f;
                spawner->accel.y = 0.0f;
            }
            break;
        case 3:
            if (spawner->pos.x < 200.0) {
                spawner->side++;
                carry = 200.0 - spawner->pos.x;
                spawner->pos.x = 200.0;
                spawner->pos.y = 600.0 - carry;
                spawner->accel.x = 0.0f;
                spawner->accel.y = -3.0f;
            }
            break;
        case 4:
            if (spawner->pos.y < 200.0) {
                spawner->side = 1;
                carry = 200.0 - spawner->pos.y;
                spawner->pos.x = 200.0 + carry;
                spawner->pos.y = 200.0;
                spawner->accel.x = 3.0f;
                spawner->accel.y = 0.0f;
            }
            break;
    }
    
    //DrawCircle(spawner->pos.x,spawner->pos.y,20,WHITE); // the spawner shouldnt be drawn in final build this is for visualization
}

void updateBullets(){

    // Update
    //----------------------------------------------------------------------------------
    // Reset the bullet index
    // New bullets will replace the old ones that are already disabled due to out-of-screen
    if (bulletCount >= MAX_BULLETS)
    {
        bulletCount = 0;
        bulletDisabledCount = 0;
    }

    spawnCooldownTimer--;
    if (spawnCooldownTimer < 0)
    {
        spawnCooldownTimer = spawnCooldown;

        // Spawn bullets
        float degreesPerRow = 360.0f/bulletRows;
        for (int row = 0; row < bulletRows; row++)
        {
            if (bulletCount < MAX_BULLETS)
            {
                bullets[bulletCount].position = (Vector2){(float) screenWidth/2, (float) screenHeight/2};
                bullets[bulletCount].disabled = false;
                bullets[bulletCount].color = bulletColor[row%2];

                float bulletDirection = baseDirection + (degreesPerRow*row);

                // Bullet speed*bullet direction, this will determine how much pixels will be incremented/decremented
                // from the bullet position every frame. Since the bullets doesn't change its direction and speed,
                // only need to calculate it at the spawning time
                // 0 degrees = right, 90 degrees = down, 180 degrees = left and 270 degrees = up, basically clockwise
                // Case you want it to be anti-clockwise, add "* -1" at the y acceleration
                bullets[bulletCount].acceleration = (Vector2){
                    bulletSpeed*cosf(bulletDirection*DEG2RAD),
                    bulletSpeed*sinf(bulletDirection*DEG2RAD)
                };

                bulletCount++;
            }
        }

        baseDirection += angleIncrement;
    }

    // Update bullets position based on its acceleration
    for (int i = 0; i < bulletCount; i++)
    {
        // Only update bullet if inside the screen
        if (!bullets[i].disabled)
        {
            bullets[i].position.x += bullets[i].acceleration.x;
            bullets[i].position.y += bullets[i].acceleration.y;

            // Disable bullet if out of screen
            if ((bullets[i].position.x < -bulletRadius*2) ||
                (bullets[i].position.x > screenWidth + bulletRadius*2) ||
                (bullets[i].position.y < -bulletRadius*2) ||
                (bullets[i].position.y > screenHeight + bulletRadius*2))
            {
                bullets[i].disabled = true;
                bulletDisabledCount++;
            }
        }
    }    
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
