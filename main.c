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

    Rectangle topPipeRec;       //hitboxtoppipe
    Rectangle bottomPipeRec;    //hitboxbottompipe

    float x, topY, bottomY;

    bool active;

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

bool gameOver;

Texture2D gameOverSprite;
Texture2D scoreBoard;

static int score = 0;
static int hiScore = 0;

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
    gameOver = false;

    map.backgroundY = -500;
    map.foregroundY = 630;

    map.scrollingBack = 0.0f;
    map.scrollingFore = 0.0f;
    map.framesCounter = 0;
    map.framesSpeed = 8;

    map.ceiling = 28.0f;
    map.ground = 625.0f;

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

        pipe[i].topPipeRec.x = pipe[i].x;
        pipe[i].topPipeRec.y = pipe[i].topY;
        pipe[i].topPipeRec.height = topPipe_frameHeight;
        pipe[i].topPipeRec.width = topPipe_frameWidth;

        pipe[i].bottomPipeRec.x = pipe[i].x;
        pipe[i].bottomPipeRec.y = pipe[i].bottomY;
        pipe[i].bottomPipeRec.height = bottomPipe_frameHeight;
        pipe[i].bottomPipeRec.width = bottomPipe_frameWidth;

        pipe[i].active = true;
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
    if (!gameOver)
    {
        DrawTextureEx(map.background, (Vector2) {map.scrollingBack, map.backgroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY},
                      0.0f, 2.5f, WHITE);

        for (int i = 0; i < MAX_PIPES; i++)
        {
            DrawTextureEx(pipe[i].topPipe, (Vector2) {pipe[i].x, pipe[i].topY}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(pipe[i].bottomPipe, (Vector2) {pipe[i].x, pipe[i].bottomY}, 0.0f, 2.5f, WHITE);

//            DrawRectangle(pipe[i].topPipeRec.x, pipe[i].topPipeRec.y, topPipe_frameWidth, topPipe_frameHeight, BLUE);
//            DrawRectangle(pipe[i].bottomPipeRec.x, pipe[i].bottomPipeRec.y, bottomPipe_frameWidth, bottomPipe_frameHeight, MAROON);
        }

        DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},
                      0.0f, 2.5f, WHITE);

        DrawTexturePro(bird.birdSprite,(Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth, bird.birdSprite.height},
                                         (Rectangle) {bird.x + (bird.x / 3), bird.y, bird.frameWidth, bird.birdSprite.height},
                                           (Vector2) {bird.frameWidth, bird.birdSprite.height}, bird.rotation, WHITE);

//        DrawRectangle(bird.x+5, bird.y - bird.birdSprite.height+15, bird.frameWidth-10, bird.birdSprite.height-10, RED);
//
//        DrawRectangle(bird.x, map.foregroundY, bird.frameWidth-10, map.foreground.height, PURPLE);
//        DrawRectangle(bird.x, -50, bird.frameWidth-10, map.foreground.height, PURPLE);

        DrawText(TextFormat("Score = %d",score),5,5,15,BLACK);
        DrawText(TextFormat("Hi-Score = %d",hiScore),5,25,15,BLACK);
    }
    else
    {
        DrawTextureEx(map.background, (Vector2) {map.scrollingBack, map.backgroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY},0.0f, 2.5f, WHITE);
        DrawTextureEx(gameOverSprite, (Vector2) {screenWidth/5,screenHeight/4}, 0.0f, 3.0f, WHITE);
        DrawTexturePro(bird.birdSprite,(Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth, bird.birdSprite.height},
                       (Rectangle) {bird.x + (bird.x / 3), bird.y, bird.frameWidth, bird.birdSprite.height},
                       (Vector2) {bird.frameWidth, bird.birdSprite.height}, bird.rotation, WHITE);
        DrawTextureEx(scoreBoard, (Vector2) {screenWidth/5,screenHeight/3 + 15}, 0.0f, 2.5f, WHITE);
        DrawText(TextFormat("%d",score),GetScreenWidth()/2 - MeasureText(TextFormat("%d",score),20)/2,screenHeight/3 + 60,25, BLACK);
        DrawText(TextFormat("%d",hiScore),GetScreenWidth()/2 - MeasureText(TextFormat("%d",hiScore),20)/2, screenHeight/3 + 115,2, BLACK);
        DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},0.0f, 2.5f, WHITE);
    }
    EndDrawing();
}

void updateGame(void)
{
    map.scrollingBack -= 0.1f;
    map.scrollingFore -= 3.0f;
    if (map.scrollingBack <= -(float) map.background.width * 2) map.scrollingBack = 0;
    if (map.scrollingFore <= -(float) map.foreground.width * 2) map.scrollingFore = 0;

    if (!gameOver)
    {
        Rectangle birdRec = {bird.x + 5, bird.y - bird.birdSprite.height + 15, bird.frameWidth - 10,bird.birdSprite.height - 10};
        Rectangle topRec = {bird.x, -50, bird.frameWidth-10, map.foreground.height};
        Rectangle bottomRec = {bird.x, map.foregroundY, bird.frameWidth-10, map.foreground.height};

        map.framesCounter++;
        if (map.framesCounter >= (60 / map.framesSpeed)) {
            map.framesCounter = 0;
            currentFrame++;

            if (currentFrame > 2) currentFrame = 0;

            birdRec.x = (float) currentFrame * (float) bird.birdSprite.width / 3;
        }

        if (!gameOver && IsKeyPressed(KEY_SPACE))bird.isJumping = 1;

        if (bird.isJumping == 1)
        {
            for (int i = 0; i < MAX_PIPES; i++){
                pipe[i].x -= 3;
                pipe[i].topPipeRec.x -= 3;
                pipe[i].bottomPipeRec.x -=3;
            }
            if (bird.y < map.ground && bird.y > map.ceiling) jump();
        }

        if (CheckCollisionRecs(birdRec, topRec) || CheckCollisionRecs(birdRec, bottomRec)) gameOver = true;

        for(int i = 0; i < MAX_PIPES; i++)
        {
            if ((CheckCollisionRecs(birdRec, pipe[i].topPipeRec) || CheckCollisionRecs(birdRec, pipe[i].bottomPipeRec)) && pipe[i].active) gameOver = true;
            else if ((pipe[i].topPipeRec.x + pipe[i].topPipeRec.width < birdRec.x) && (pipe[i].bottomPipeRec.x + pipe[i].bottomPipeRec.width< bird.x) && !gameOver && pipe[i].active)
            {
                score++;
                pipe[i].active = false;
            }
        }
        if (score > hiScore) hiScore = score;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            score = 0;
            gameOver = false;
        }
    }
}

void loadTexture(void)
{
    static const char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/background.png";
    static const char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/foreground.png";
    static const char birdPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/bird.png";
    static const char topPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/topPipe.png";
    static const char bottomPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/bottomPipe.png";
    static const char gameOverPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/gameOver.png";
    static const char scoreBoardPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/scoreBoard.png";

    map.background = LoadTexture(backgroundPath);
    map.foreground = LoadTexture(foregroundPath);
    bird.birdSprite = LoadTexture(birdPath);
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipe[i].topPipe = LoadTexture(topPipePath);
        pipe[i].bottomPipe = LoadTexture(bottomPipePath);
    }
    gameOverSprite = LoadTexture(gameOverPath);
    scoreBoard = LoadTexture(scoreBoardPath);

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
    UnloadTexture(gameOverSprite);
    UnloadTexture(scoreBoard);
}