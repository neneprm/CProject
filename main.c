#include <stdio.h>
#include <string.h>
#include "raylib.h"

static const char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Background.png";
static const char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Foreground.png";
static const char birdPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Bird.png";

Texture2D background;
Texture2D foreground;
Texture2D bird;

void loadTexture()
{
    background = LoadTexture(backgroundPath);
    foreground = LoadTexture(foregroundPath);
    bird = LoadTexture(birdPath);
}

void unloadTexture()
{
    UnloadTexture(background);
    UnloadTexture(foreground);
    UnloadTexture(bird);
}

int main()
{
    const int screenWidth = 490;
    const int screenHeight = 735;
    InitWindow(screenWidth, screenHeight, "Flappy Bird");

    float scrollingBack = 0.0f;
    float scrollingFore = 0.0f;

    Vector2 position = {215.0f,362.5f};
    Rectangle  frameRec = {0.0f, 0.0f, (float)bird.width/3, (float)bird.height};
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;

    SetTargetFPS(60);
    loadTexture();

    while (!WindowShouldClose())
    {
        scrollingBack -= 0.1f;
        scrollingFore -= 3.0f;
        if (scrollingBack <= -(float)background.width*2) scrollingBack = 0;
        if (scrollingFore <= -(float)foreground.width*2) scrollingFore = 0;

        framesCounter++;
        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 2) currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)bird.width/3;
        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureEx(background, (struct Vector2){scrollingBack, -500}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(background, (struct Vector2){(float)background.width*2 + scrollingBack, -500}, 0.0f, 2.5f, WHITE);

            DrawTextureEx(foreground, (struct Vector2){scrollingFore, 630}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(foreground, (struct Vector2){(float)foreground.width*2 + scrollingFore, 630}, 0.0f, 2.5f, WHITE);

            DrawTextureRec(bird, (Rectangle){currentFrame * (bird.width / 3),0,bird.width/3,bird.height}, position, WHITE);

        EndDrawing();

    }

    unloadTexture();

    CloseWindow();

    return 0;
}
