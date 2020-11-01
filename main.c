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

float birdX, birdY;

bool jump;
int jumpTimer;

int counter = 15;

//int isJumping;
//int jump_delay;
//int jump_check;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

static void InitGame(void);
//static void jump(void);
static void drawGame(void);
static void updateGame(void);
static void loadTexture(void);
static void unloadTexture(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
//    Rectangle frameRec = {0.0f, 0.0f, (float) bird.birdSprite.width / 3, (float) bird.birdSprite.height};
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

void InitGame(void)
{
    birdX = 220.0f;
    birdY = 362.5f;

    Vector2 position = {birdX, birdY};

//    isJumping = 0;
//    jump_delay = 25;
//
//    jump_check = 0;
}

//void jump()
//{
//    if ( isJumping == 0 ) { birdY -= jump_delay; jump_delay--;}

//    falling
//    else if( isJumping == 1 )
//    {
//        if (birdY >= foregroundY) isJumping = 0;
//        birdY += jump_delay;
//        jump_delay++;
//    }

//    if( birdY == foregroundY)
//    {  //Floor position -> at floor reset everything
//        jump_delay = 30;
//        isJumping = 0;
//        jump_check = 0;
//    }
//}

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

        DrawTextureRec(bird.birdSprite, (Rectangle) {currentFrame * frameWidth, 0, frameWidth,
                                                     bird.birdSprite.height}, (Vector2) {birdX, birdY}, WHITE);

    EndDrawing();
}

void updateGame(void)
{
    float frameWidth = (bird.birdSprite.width / 3);
    Rectangle birdRec = {birdX, birdY, frameWidth, frameWidth };

    Vector2 position = {birdX, birdY};

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

    if (IsKeyPressed(KEY_SPACE))
    {
        jump = true;
    }

    if (jump)
    {
        jumpTimer ++;
        if (jumpTimer < 80)
        {
            birdY -= (1 * counter--);
        }
        else
        {
            birdY += (1 * counter--);
        }
    }

    if (jumpTimer >= 160)
    {
        jumpTimer = 0;
        jump = false;
        counter = 15;
    }

    //    if (IsKeyPressed(KEY_SPACE))
//    {
//        birdY -= 30.0f;
//    }
//        jump_check = 1;
//    if (jump_check == 1) jump();
//    birdY -= GetFrameTime() * 500.0f;
//    birdY += GetFrameTime() * 100.0f;
//    if (IsKeyPressed(KEY_SPACE)) birdY -= GetFrameTime() * 2000.0f;
}

void loadTexture(void)
{
    static const char backgroundPath[] = "/Users/pitchakorn/Desktop/prmproj/CProject-1/assets/Background.png";
    static const char foregroundPath[] = "/Users/pitchakorn/Desktop/prmproj/CProject-1/Foreground.png";
    static const char birdPath[] = "/Users/pitchakorn/Desktop/prmproj/CProject-1/assets/Bird.png";

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