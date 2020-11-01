#include <stdio.h>
#include "raylib.h"

//------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------

typedef struct Bird
{
    Texture2D birdSprite;
} Bird;

typedef struct Map
{
    Texture2D background;
    Texture2D foreground;
} Map;

//------------------------------------------------------------------------------------
// Structure variable
//------------------------------------------------------------------------------------

static Bird bird;
static Map map;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int screenWidth = 490;
static const int screenHeight = 735;

float backgroundY = -500;
float foregroundY = 630;

int currentFrame = 0;

float scrollingBack = 0.0f;
float scrollingFore = 0.0f;
int framesCounter = 0;
int framesSpeed = 8;

float rotation = 0.0f;

float birdX, birdY;

int isJumping;
float velocity;
float acceleration;
float gravity;

float ceiling;
float ground;

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
    birdX = 220.0f;
    birdY = 362.5f;

    isJumping = 0;
    velocity = 0.0f;
    acceleration = 0.0f;
    gravity = 100.0f;

    ceiling = 30.0f;
    ground = 630.0f;
}

void jump(void)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        acceleration = 10.0f;
        velocity = -gravity / 1.5f;
        rotation = -35;
    }
    else
    {
        acceleration += gravity * GetFrameTime();
        rotation ++;
    }

    if (acceleration >= gravity) acceleration = gravity;

    velocity += acceleration * GetFrameTime() * 10;
    birdY += velocity * GetFrameTime() * 5;
}

void drawGame(void)
{
    float frameWidth = (bird.birdSprite.width / 3);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureEx(map.background, (Vector2) {scrollingBack, backgroundY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + scrollingBack, backgroundY}, 0.0f,
                  2.5f, WHITE);

    DrawTextureEx(map.foreground, (Vector2) {scrollingFore, foregroundY}, 0.0f, 2.5f, WHITE);
    DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + scrollingFore, foregroundY}, 0.0f,
                  2.5f, WHITE);

//    DrawTextureRec(bird.birdSprite, (Rectangle) {currentFrame * frameWidth, 0, frameWidth,
//                                                 bird.birdSprite.height}, (Vector2) {birdX, birdY}, WHITE);

    DrawTexturePro(bird.birdSprite, (Rectangle) {currentFrame * frameWidth, 0, frameWidth,
                                                 bird.birdSprite.height}, (Rectangle) {birdX + (birdX/3), birdY, frameWidth, bird.birdSprite.height},
                                                (Vector2){frameWidth, bird.birdSprite.height} , rotation, WHITE);

    EndDrawing();
}

void updateGame(void)
{
    float frameWidth = (bird.birdSprite.width / 3);
    Rectangle birdRec = {birdX, birdY, frameWidth, frameWidth };

    framesCounter++;
    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;

        if (currentFrame > 2) currentFrame = 0;

        birdRec.x = (float) currentFrame * (float) bird.birdSprite.width / 3;
    }

    scrollingBack -= 0.1f;
    scrollingFore -= 3.0f;
    if (scrollingBack <= -(float) map.background.width * 2) scrollingBack = 0;
    if (scrollingFore <= -(float) map.foreground.width * 2) scrollingFore = 0;

    if (IsKeyPressed(KEY_SPACE)) isJumping = 1;
    if (isJumping == 1)
    {
        if (birdY < ground && birdY > ceiling) jump();
        else if (birdY == ground || birdY == ceiling) isJumping = 0; //isJumping should change to be Game Over
    }

}

void loadTexture(void)
{
    static const char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Background.png";
    static const char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Foreground.png";
    static const char birdPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Bird.png";

    map.background = LoadTexture(backgroundPath);
    map.foreground = LoadTexture(foregroundPath);
    bird.birdSprite = LoadTexture(birdPath);
}

void unloadTexture(void)
{
    UnloadTexture(map.background);
    UnloadTexture(map.foreground);
    UnloadTexture(bird.birdSprite);
}