#include <stdio.h>
#include "raylib.h"

//------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------

typedef struct Map
{
    Texture2D background;
    Texture2D foreground;

    float backgroundY;
    float foregroundY;

    float scrollingBack;
    float scrollingFore;
    int framesCounter;
    int framesSpeed;

    float ceiling;
    float ground;

} Map;

typedef struct Bird
{
    Texture2D birdSprite;
    float frameWidth;
    float x, y;

    float rotation;

    int isJumping;
    float velocity;
    float acceleration;
    float gravity;

} Bird;

typedef struct Pipe
{
    Texture2D topPipe;
    Texture2D bottomPipe;

    float topPipe_frameWidth;
    float topPipe_frameHeight;

    float bottomPipe_frameWidth;
    float bottomPipe_frameHeight;

    float x, topY, bottomY;
    float topY_min, topY_max;
    float bottomY_min, bottomY_max;

} Pipe;

//------------------------------------------------------------------------------------
// Structure variable
//------------------------------------------------------------------------------------

static Map map;
static Bird bird;
static Pipe pipe;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int screenWidth = 490;
static const int screenHeight = 735;

int currentFrame = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

static void InitGame(void);
static void jump(void);
static void drawGame(void);
static void updateGame(void);
static void loadTexture(void);
static void unloadTexture(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main()
{
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    loadTexture();
    SetTargetFPS(60);
    InitGame();
    while (!WindowShouldClose())
    {
        updateGame();
        drawGame();
    }
    unloadTexture();
    CloseWindow();

    return 0;
}

//------------------------------------------------------------------------------------
// Initializing variable
//------------------------------------------------------------------------------------
void InitGame(void)
{
    map.backgroundY = -500;
    map.foregroundY = 630;

    map.scrollingBack = 0.0f;
    map.scrollingFore = 0.0f;
    map.framesCounter = 0;
    map.framesSpeed = 8;

    map.ceiling = 30.0f;
    map.ground = 620.0f;

    bird.frameWidth = (bird.birdSprite.width / 3);

    bird.x = 220.0f;
    bird.y = 362.5f;

    bird.rotation = 0.0f;

    bird.isJumping = 0;
    bird.velocity = 0.0f;
    bird.acceleration = 0.0f;
    bird.gravity = 100.0f;


    pipe.topPipe_frameWidth = (float) pipe.topPipe.width * 2.5;
    pipe.topPipe_frameHeight = (float) pipe.topPipe.height * 2.5;

    pipe.bottomPipe_frameWidth = (float) pipe.bottomPipe.width * 2.5;
    pipe.bottomPipe_frameHeight = (float) pipe.bottomPipe.height * 2.5;

    pipe.x = 330 * 3;

    pipe.topY_min = -(pipe.topPipe_frameHeight) + (bird.birdSprite.height * 2);
    pipe.topY_max = 0;
    pipe.topY = GetRandomValue(pipe.topY_min,pipe.topY_max);
//    float topY_val = pipe.topY;

    pipe.bottomY_min = (pipe.bottomPipe_frameHeight) + (bird.birdSprite.height * 3);
    pipe.bottomY_max = -(pipe.bottomPipe_frameHeight - map.foregroundY - bird.birdSprite.height/2);
    pipe.bottomY = GetRandomValue(pipe.bottomY_min, pipe.bottomY_max);
//    float bottomY_val = pipe.bottomY;

    float distanceTop = pipe.bottomY_min - pipe.topY_max;
    float distanceBottom = pipe.topY_min - pipe.bottomY_max;

//pipe.topY_max, pipe.bottomY_min
//pipe.topY_min, pipe.bottomY_max

//    if((pipe.topY && pipe.bottomY) <= (distanceTop || distanceBottom))
//        pipe.topY = topY_val;
//        pipe.bottomY = bottomY_val;
}

void jump(void)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        bird.acceleration = 10.0f;
        bird.velocity = -bird.gravity / 1.5f;
        bird.rotation = -35;
    }
    else
    {
        bird.acceleration += bird.gravity * GetFrameTime();
        bird.rotation ++;
    }

    if (bird.acceleration >= bird.gravity) bird.acceleration = bird.gravity;

    bird.velocity += bird.acceleration * GetFrameTime() * 10;
    bird.y += bird.velocity * GetFrameTime() * 5;
}

void drawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureEx(map.background, (Vector2) {map.scrollingBack, map.backgroundY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY}, 0.0f,
                  2.5f, WHITE);

    DrawTextureEx(pipe.topPipe, (Vector2){pipe.x,pipe.topY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(pipe.bottomPipe, (Vector2){pipe.x,pipe.bottomY}, 0.0f, 2.5f, WHITE);

    DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},
                  0.0f,2.5f, WHITE);

//    DrawTextureRec(bird.birdSprite, (Rectangle) {currentFrame * frameWidth, 0, frameWidth,
//                                                 bird.birdSprite.height}, (Vector2) {birdX, birdY}, WHITE);

    DrawTexturePro(bird.birdSprite, (Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth,
                                                 bird.birdSprite.height}, (Rectangle) {bird.x + (bird.x/3), bird.y,bird.frameWidth, bird.birdSprite.height},
                   (Vector2){bird.frameWidth, bird.birdSprite.height} , bird.rotation, WHITE);

    DrawRectangle(bird.x, bird.y - bird.birdSprite.height, bird.frameWidth, bird.birdSprite.height, RED);
    DrawRectangle(pipe.x, pipe.topY,pipe.topPipe_frameWidth, pipe.topPipe_frameHeight, BLUE);
    DrawRectangle(pipe.x, pipe.bottomY,pipe.bottomPipe_frameWidth, pipe.bottomPipe_frameHeight, BLUE);

    EndDrawing();
}

void updateGame(void)
{
    Rectangle birdRec = {bird.x, bird.y - bird.birdSprite.height, bird.frameWidth, bird.birdSprite.height};
    Rectangle topPipeRec = {pipe.x, pipe.topY,pipe.topPipe_frameWidth, pipe.topPipe_frameHeight};
    Rectangle bottomPipeRec = {pipe.x, pipe.bottomY,pipe.bottomPipe_frameWidth, pipe.bottomPipe_frameHeight};

    map.framesCounter++;
    if (map.framesCounter >= (60 / map.framesSpeed))
    {
        map.framesCounter = 0;
        currentFrame++;

        if (currentFrame > 2) currentFrame = 0;

        birdRec.x = (float) currentFrame * (float) bird.birdSprite.width / 3;
    }

    map.scrollingBack -= 0.1f;
    map.scrollingFore -= 3.0f;
    if (map.scrollingBack <= -(float) map.background.width * 2) map.scrollingBack = 0;
    if (map.scrollingFore <= -(float) map.foreground.width * 2) map.scrollingFore = 0;

    if (IsKeyPressed(KEY_SPACE))bird.isJumping = 1;

    if (bird.isJumping == 1) {
        pipe.x -= 3;
        if (bird.y < map.ground && bird.y > map.ceiling) jump();
        else if (bird.y == map.ground || bird.y == map.ceiling) bird.isJumping = 0; //isJumping should change to be Game Over
    }

    if (CheckCollisionRecs (birdRec, topPipeRec) || CheckCollisionRecs (birdRec, bottomPipeRec)){
        printf("Collide\n");
    }
}

void loadTexture(void)
{
    static const char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Background.png";
    static const char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Foreground.png";
    static const char birdPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Bird.png";
    static const char topPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/topPipe.png";
    static const char bottomPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/bottomPipe.png";

    map.background = LoadTexture(backgroundPath);
    map.foreground = LoadTexture(foregroundPath);
    bird.birdSprite = LoadTexture(birdPath);
    pipe.topPipe = LoadTexture(topPipePath);
    pipe.bottomPipe = LoadTexture(bottomPipePath);
}

void unloadTexture(void)
{
    UnloadTexture(map.background);
    UnloadTexture(map.foreground);
    UnloadTexture(bird.birdSprite);
    UnloadTexture(pipe.topPipe);
    UnloadTexture(pipe.bottomPipe);
}