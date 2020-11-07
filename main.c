#include <stdio.h>
#include "raylib.h"
#define MAX_PIPES 100
#define DIST_PIPE 300

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

    float x, topY, bottomY;

} Pipe;

//------------------------------------------------------------------------------------
// Structure variable
//------------------------------------------------------------------------------------

static Map map;
static Bird bird;
static Pipe pipe[MAX_PIPES];

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int screenWidth = 490;
static const int screenHeight = 735;

int currentFrame = 0;

float topPipe_frameWidth;
float topPipe_frameHeight;

float bottomPipe_frameWidth;
float bottomPipe_frameHeight;

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

    topPipe_frameWidth = (float) pipe[0].topPipe.width * 2.5;
    topPipe_frameHeight = (float) pipe[0].topPipe.height * 2.5;

    bottomPipe_frameWidth = (float) pipe[0].bottomPipe.width * 2.5;
    bottomPipe_frameHeight = (float) pipe[0].bottomPipe.height * 2.5;

    float topY_min = -(topPipe_frameHeight) + 145.0f;
    float topY_max = 0;

    for(int i = 0; i < MAX_PIPES; i++)
    {
        pipe[i].x = 900.0f + (DIST_PIPE * i);

        pipe[i].topY = GetRandomValue(topY_min, topY_max);
        pipe[i].bottomY = pipe[i].topY + 550.0f;
    }
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
    DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY}, 0.0f,2.5f, WHITE);

    for (int i = 0; i < MAX_PIPES; i++)
    {
        DrawTextureEx(pipe[i].topPipe, (Vector2) {pipe[i].x, pipe[i].topY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(pipe[i].bottomPipe, (Vector2) {pipe[i].x, pipe[i].bottomY}, 0.0f, 2.5f, WHITE);
    }

    DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},
                  0.0f,2.5f, WHITE);

    DrawTexturePro(bird.birdSprite, (Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth,bird.birdSprite.height},
                                      (Rectangle) {bird.x + (bird.x/3), bird.y,bird.frameWidth, bird.birdSprite.height},
                                        (Vector2){bird.frameWidth, bird.birdSprite.height} , bird.rotation, WHITE);

//    DrawRectangle(bird.x+5, bird.y - bird.birdSprite.height+15, bird.frameWidth-10, bird.birdSprite.height-10, RED);
//    for (int i = 0; i < MAX_PIPES; i++) {
//        DrawRectangle(pipe[i].x, pipe[i].topY, topPipe_frameWidth, topPipe_frameHeight, BLUE);
//        DrawRectangle(pipe[i].x, pipe[i].bottomY, bottomPipe_frameWidth, bottomPipe_frameHeight, BLUE);
//    }

    EndDrawing();
}

void updateGame(void)
{
    Rectangle birdRec = {bird.x+5, bird.y - bird.birdSprite.height+15, bird.frameWidth-10, bird.birdSprite.height-10};

    for (int i = 0; i < MAX_PIPES; i++)
    {
        Rectangle topPipeRec = {pipe[i].x, pipe[i].topY, topPipe_frameWidth, topPipe_frameHeight};
        Rectangle bottomPipeRec = {pipe[i].x, pipe[i].bottomY, bottomPipe_frameWidth, bottomPipe_frameHeight};

        if (CheckCollisionRecs (birdRec, topPipeRec) || CheckCollisionRecs (birdRec, bottomPipeRec)){
            printf("Collide\n");
        }
    }

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
        for (int i = 0; i < MAX_PIPES; i++) pipe[i].x -= 3;
        if (bird.y < map.ground && bird.y > map.ceiling) jump();
        else if (bird.y == map.ground || bird.y == map.ceiling) bird.isJumping = 0; //isJumping should change to be Game Over
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
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipe[i].topPipe = LoadTexture(topPipePath);
        pipe[i].bottomPipe = LoadTexture(bottomPipePath);
    }
}

void unloadTexture(void)
{
    UnloadTexture(map.background);
    UnloadTexture(map.foreground);
    UnloadTexture(bird.birdSprite);
    for (int i = 0; i < MAX_PIPES; i++)
    {
        UnloadTexture(pipe[i].topPipe);
        UnloadTexture(pipe[i].bottomPipe);
    }
}