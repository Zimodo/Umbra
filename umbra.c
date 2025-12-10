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
    float spawnCooldownTimer;
    float baseDirection;
    int angleIncrement;
    int border;
};

typedef struct Bullet {
    Vector2 position;       // Bullet position on screen
    Vector2 acceleration;   // Amount of pixels to be incremented to position every frame
    bool disabled;          // Skip processing and draw case out of screen
    Color color;            // Bullet color
} Bullet;

struct BulletData {
    int bulletCount;
    int bulletDisabledCount; // Used to calculate how many bullets are on screen
    int bulletRadius;
    float bulletSpeed;
    int bulletRows;
    Color bulletColor;
};

void updateEarthAndMoon(struct Earth* earth, struct Moon* moon);
void updateBulletSpawner(struct Spawner* spawner);
void updateBullets(Bullet bullets[], struct BulletData* bulletdata, struct Spawner* spawner);
void drawEarthAndMoon(struct Earth* earth, struct Moon* moon);
void drawBullets(Bullet bullets[], struct BulletData* bulletdata);
void drawBulletSpawner(struct Spawner* spawner);

int main(){
    
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "UMBRA");

    Bullet *bullets = (Bullet *)RL_CALLOC(MAX_BULLETS, sizeof(Bullet)); // Bullets array
    
    /*
    int bulletCount = 0;
    int bulletDisabledCount = 0; // Used to calculate how many bullets are on screen
    int bulletRadius = 10;
    float bulletSpeed = 3.0f;
    int bulletRows = 6;
    Color bulletColor[2] = { RED, BLUE };
    */
    
    struct Earth earth;
    struct Moon moon;
    struct Spawner spawner;
    struct BulletData bulletData;
    
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
    spawner.spawnCooldown = 1.0f;
    spawner.border = 200;
    
    // Init BulletData
    bulletData.bulletCount = 0;
    bulletData.bulletDisabledCount = 0; // Used to calculate how many bullets are on screen
    bulletData.bulletRadius = 10;
    bulletData.bulletSpeed = 4.0f;
    bulletData.bulletRows = 10;
    bulletData.bulletColor = RED;
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        updateEarthAndMoon(&earth, &moon);
        updateBulletSpawner(&spawner);
        updateBullets(bullets, &bulletData, &spawner);
        
        BeginDrawing();
        
            ClearBackground(BLACK);
            drawEarthAndMoon(&earth, &moon); // pass by reference using pointers
            drawBulletSpawner(&spawner);
            drawBullets(bullets, &bulletData);
            
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

void updateBullets(Bullet bullets[], struct BulletData* bulletData, struct Spawner* spawner){

    /*
    //this is gonna reset every frame i just want to see if it would work if i passed everything in
    int bulletCount = 0;
    int bulletDisabledCount = 0; // Used to calculate how many bullets are on screen
    int bulletRadius = 10;
    float bulletSpeed = 3.0f;
    int bulletRows = 6;
    Color bulletColor[2] = { RED, BLUE };
    */
    
    // Update
    //----------------------------------------------------------------------------------
    // Reset the bullet index
    // New bullets will replace the old ones that are already disabled due to out-of-screen
    if (bulletData->bulletCount >= MAX_BULLETS)
    {
        bulletData->bulletCount = 0;
        bulletData->bulletDisabledCount = 0;
    }

    spawner->spawnCooldownTimer--;
    if (spawner->spawnCooldownTimer < 0)
    {
        spawner->spawnCooldownTimer = spawner->spawnCooldown;

        // Spawn bullets
        float degreesPerRow = 360.0f/bulletData->bulletRows;
        for (int row = 0; row < bulletData->bulletRows; row++)
        {
            if (bulletData->bulletCount < MAX_BULLETS)
            {
                bullets[bulletData->bulletCount].position = (Vector2){(float) spawner->pos.x, (float) spawner->pos.y};
                bullets[bulletData->bulletCount].disabled = false;
                bullets[bulletData->bulletCount].color = bulletData->bulletColor;

                float bulletDirection = spawner->baseDirection + (degreesPerRow*row);

                // Bullet speed*bullet direction, this will determine how much pixels will be incremented/decremented
                // from the bullet position every frame. Since the bullets doesn't change its direction and speed,
                // only need to calculate it at the spawning time
                // 0 degrees = right, 90 degrees = down, 180 degrees = left and 270 degrees = up, basically clockwise
                // Case you want it to be anti-clockwise, add "* -1" at the y acceleration
                bullets[bulletData->bulletCount].acceleration = (Vector2){
                    bulletData->bulletSpeed*cosf(bulletDirection*DEG2RAD),
                    bulletData->bulletSpeed*sinf(bulletDirection*DEG2RAD)
                };

                bulletData->bulletCount++;
            }
        }

        spawner->baseDirection += spawner->angleIncrement;
    }

    // Update bullets position based on its acceleration
    for (int i = 0; i < bulletData->bulletCount; i++)
    {
        // Only update bullet if inside the screen
        if (!bullets[i].disabled)
        {
            bullets[i].position.x += bullets[i].acceleration.x;
            bullets[i].position.y += bullets[i].acceleration.y;

            // Disable bullet if out of screen
            if ((bullets[i].position.x < -bulletData->bulletRadius*2) ||
                (bullets[i].position.x > screenWidth + bulletData->bulletRadius*2) ||
                (bullets[i].position.y < -bulletData->bulletRadius*2) ||
                (bullets[i].position.y > screenHeight + bulletData->bulletRadius*2))
            {
                bullets[i].disabled = true;
                bulletData->bulletDisabledCount++;
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

void drawBulletSpawner(struct Spawner* spawner){
    DrawCircle(spawner->pos.x,spawner->pos.y,20,WHITE);
}
void drawBullets(Bullet bullets[], struct BulletData* bulletData){
    // setting this to some random number, gonna have to replace this later with the actual value
    // int bulletCount = 500;
    
    // Draw bullets using pre-rendered texture containing circle
    for (int i = 0; i < bulletData->bulletCount; i++)
    {
        // Do not draw disabled bullets (out of screen)
        if (!bullets[i].disabled)
        {
            /*
            DrawTexture(bulletTexture.texture,
                (int)(bullets[i].position.x - bulletTexture.texture.width*0.5f),
                (int)(bullets[i].position.y - bulletTexture.texture.height*0.5f),
                bullets[i].color);
            */
            
            DrawCircle(bullets[i].position.x, bullets[i].position.y, 5, RED);
        }
    }
}