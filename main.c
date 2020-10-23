#include <stdio.h>
#include <string.h>
#include "raylib.h"

int main() {
    const int screenWidth = 288;
    const int screenHeight = 510;
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Background.png";
    char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/Foreground.png";

    Texture2D  background = LoadTexture(backgroundPath);
    Texture2D  foreground = LoadTexture(foregroundPath);
    float scrollingBack = 0.0f;
    float scrollingFore = 0.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        scrollingBack -= 0.1f;
        scrollingFore -= 3.0f;

        if (scrollingBack <= -background.width*2) scrollingBack = 0;
        if (scrollingFore <= -foreground.width*2) scrollingFore = 0;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureEx(background, (struct Vector2){scrollingBack, 0}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(background, (struct Vector2){background.width*2 + scrollingBack, 0}, 0.0f, 2.0f, WHITE);

            DrawTextureEx(foreground, (struct Vector2){scrollingFore, 440}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(foreground, (struct Vector2){foreground.width*2 + scrollingFore, 440}, 0.0f, 2.0f, WHITE);

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(foreground);

    CloseWindow();

    return 0;
}
