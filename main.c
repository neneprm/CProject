#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

//------------------------------------------------------------------------------------
// Defines Variables
//------------------------------------------------------------------------------------

#define MAX_PIPES 5
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

    Rectangle topPipeRec;
    Rectangle bottomPipeRec;

    float x, topY, bottomY;

    bool active;

} Pipe;

typedef struct Effect
{
    Sound hit;
    Sound jump;
    Sound point;
    Sound bgMusic;

} Effect;

//------------------------------------------------------------------------------------
// Structure Variables
//------------------------------------------------------------------------------------

static Map map;
static Bird bird;
static Pipe pipe[MAX_PIPES];
static Effect effect;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

// Window Variables
//------------------------------------------
bool gameStart;
bool gameOver;
int gameRun;

static const int screenWidth = 490;
static const int screenHeight = 735;

// Graphic Variables
//------------------------------------------
int currentFrame = 0;
Texture2D gameOverSprite;
Texture2D scoreBoard;
Texture2D title;

// Scoring and Speed Variables
//------------------------------------------
static int score;
static int hiScore;
static float speed;

// Pipe Variables
//------------------------------------------
float topPipe_frameWidth;
float topPipe_frameHeight;

float bottomPipe_frameWidth;
float bottomPipe_frameHeight;

float topY_min;
float topY_max;

float max_x;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

static void InitGame(void);         // Initialize game variables
static void jump(void);             // Make character jumps
static void drawGame(void);         // Draw graphics in the game
static void updateGame(void);       // Update the game when a player runs the program

static void loadTexture(void);      // Load game textures from image data: map, bird, pipe, etc.
static void unloadTexture(void);    // Unload game textures from memory

static void randomPipe(int i);      // Random pipes to different position

static void loadSound(void);        // Load sound effects of the game
static void unloadSound(void);      // Unload sound effects
bool IsSoundPlaying(Sound sound);   // Check if a sound is playing

static void loadHiScore(void);      // Load high score
static void recHiScore(void);       // Record new high score

//------------------------------------------------------------------------------------
// Program Main Entry Point
//------------------------------------------------------------------------------------

int main()
{
    // Initialization
    //------------------------------------------
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    InitAudioDevice();

    loadTexture();
    loadSound();

    InitGame();
    SetTargetFPS(60);

    // Main Game Loop
    //------------------------------------------
    while (!WindowShouldClose())
    {
        updateGame();
        drawGame();
    }

    // De-Initialization
    //------------------------------------------
    unloadTexture();
    unloadSound();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

//------------------------------------------------------------------------------------
// Initialize Game Variables
//------------------------------------------------------------------------------------

void InitGame(void)
{
    // Main game, Score, and Sound
    //------------------------------------------
    gameStart = true;
    gameOver = false;
    gameRun = 0;

    score = 0;
    speed = 3.0;
    loadHiScore();
    PlaySound(effect.bgMusic);

    // Map
    //------------------------------------------
    map.backgroundY = -500;
    map.foregroundY = 630;

    map.scrollingBack = 0.0f;
    map.scrollingFore = 0.0f;
    map.framesCounter = 0;
    map.framesSpeed = 8;

    map.ceiling = 28.0f;
    map.ground = 625.0f;

    // Bird
    //------------------------------------------
    bird.frameWidth = (bird.birdSprite.width / 3);

    bird.x = 220.0f;
    bird.y = 362.5f;

    bird.rotation = 0.0f;

    bird.isJumping = 0;
    bird.velocity = 0.0f;
    bird.acceleration = 0.0f;
    bird.gravity = 100.0f;

    // Pipes
    //------------------------------------------
    topPipe_frameWidth = (float) pipe[0].topPipe.width * 2.5;
    topPipe_frameHeight = (float) pipe[0].topPipe.height * 2.5;

    bottomPipe_frameWidth = (float) pipe[0].bottomPipe.width * 2.5;
    bottomPipe_frameHeight = (float) pipe[0].bottomPipe.height * 2.5;

    topY_min = -(topPipe_frameHeight) + 145.0f;
    topY_max = 0;

    // Generate Pipes
    for(int i = 0; i < MAX_PIPES; i++)
    {
        pipe[i].x = 900.0f + (DIST_PIPE * i);
        randomPipe(i);
    }
}

//------------------------------------------------------------------------------------
// Character's Jump Function
//------------------------------------------------------------------------------------

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
        bird.rotation++;
    }

    if (bird.acceleration >= bird.gravity) bird.acceleration = bird.gravity;

    bird.velocity += bird.acceleration * GetFrameTime() * 10;
    bird.y += bird.velocity * GetFrameTime() * 5;
}

//------------------------------------------------------------------------------------
// Draw Game Graphics Function
//------------------------------------------------------------------------------------

void drawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!gameOver)
    {
        DrawTextureEx(map.background, (Vector2) {map.scrollingBack, map.backgroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY},
                      0.0f, 2.5f, WHITE);

        if(gameStart && gameRun == 0) {
            DrawTextureEx(title, (Vector2) {screenWidth / 4.5, screenHeight / 4}, 0.0f, 3.0f, WHITE);
            DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(map.foreground,
                          (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},
                          0.0f, 2.5f, WHITE);
            DrawTexturePro(bird.birdSprite,
                           (Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth, bird.birdSprite.height},
                           (Rectangle) {bird.x + (bird.x / 3), bird.y, bird.frameWidth, bird.birdSprite.height},
                           (Vector2) {bird.frameWidth, bird.birdSprite.height}, bird.rotation, WHITE);
            DrawText(TextFormat("Press SPACEBAR to jump"), GetScreenWidth()/2 - MeasureText(TextFormat("Press ENTER to restart"), 15)/2, screenHeight/2 + 50, 15, BLACK);
        }

        else if(gameRun == 1)
        {
            for (int i = 0; i < MAX_PIPES; i++)
            {
                DrawTextureEx(pipe[i].topPipe, (Vector2) {pipe[i].x, pipe[i].topY}, 0.0f, 2.5f, WHITE);
                DrawTextureEx(pipe[i].bottomPipe, (Vector2) {pipe[i].x, pipe[i].bottomY}, 0.0f, 2.5f, WHITE);
                  // Pipes Hitblock Check
//                DrawRectangle(pipe[i].topPipeRec.x, pipe[i].topPipeRec.y, topPipe_frameWidth, topPipe_frameHeight, BLUE);
//                DrawRectangle(pipe[i].bottomPipeRec.x, pipe[i].bottomPipeRec.y, bottomPipe_frameWidth, bottomPipe_frameHeight, MAROON);
            }

            DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(map.foreground,
                          (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},
                          0.0f, 2.5f, WHITE);

            DrawTexturePro(bird.birdSprite,
                           (Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth, bird.birdSprite.height},
                           (Rectangle) {bird.x + (bird.x / 3), bird.y, bird.frameWidth, bird.birdSprite.height},
                           (Vector2) {bird.frameWidth, bird.birdSprite.height}, bird.rotation, WHITE);
              // Bird Hitblock Check
//            DrawRectangle(bird.x+5, bird.y - bird.birdSprite.height+15, bird.frameWidth-10, bird.birdSprite.height-10, RED);
              // Ground and Ceiling Hitblock Check
//            DrawRectangle(bird.x, map.foregroundY, bird.frameWidth-10, map.foreground.height, PURPLE);
//            DrawRectangle(bird.x, -50, bird.frameWidth-10, map.foreground.height, PURPLE);

            DrawText(TextFormat("Score %d", score), 5, 5, 20, BLACK);
            DrawText(TextFormat("Hi-Score %d", hiScore), 5, 30, 20, BLACK);
        }
    }
    else
    {
        DrawTextureEx(map.background, (Vector2) {map.scrollingBack, map.backgroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.background, (Vector2) {(float) map.background.width * 2 + map.scrollingBack, map.backgroundY},0.0f, 2.5f, WHITE);

        for (int i = 0; i < MAX_PIPES; i++)
        {
            DrawTextureEx(pipe[i].topPipe, (Vector2) {pipe[i].x, pipe[i].topY}, 0.0f, 2.5f, WHITE);
            DrawTextureEx(pipe[i].bottomPipe, (Vector2) {pipe[i].x, pipe[i].bottomY}, 0.0f, 2.5f, WHITE);
        }

        DrawTextureEx(map.foreground, (Vector2) {map.scrollingFore, map.foregroundY}, 0.0f, 2.5f, WHITE);
        DrawTextureEx(map.foreground, (Vector2) {(float) map.foreground.width * 2 + map.scrollingFore, map.foregroundY},0.0f, 2.5f, WHITE);

        DrawTexturePro(bird.birdSprite,(Rectangle) {currentFrame * bird.frameWidth, 0, bird.frameWidth, bird.birdSprite.height},
                       (Rectangle) {bird.x + (bird.x / 3), bird.y, bird.frameWidth, bird.birdSprite.height},
                       (Vector2) {bird.frameWidth, bird.birdSprite.height}, bird.rotation, WHITE);

        DrawTextureEx(gameOverSprite, (Vector2) {screenWidth/5,screenHeight/4}, 0.0f, 3.0f, WHITE);

        DrawTextureEx(scoreBoard, (Vector2) {screenWidth/5 + 2,screenHeight/3 + 15}, 0.0f, 2.5f, WHITE);
        DrawText(TextFormat("%d",score),GetScreenWidth()/2 - MeasureText(TextFormat("%d",score),25)/2,screenHeight/3 + 60,25, BLACK);
        DrawText(TextFormat("%d",hiScore),GetScreenWidth()/2 - MeasureText(TextFormat("%d",hiScore),25)/2, screenHeight/3 + 115,25, BLACK);
        DrawText(TextFormat("Press ENTER to restart"), GetScreenWidth()/2 - MeasureText(TextFormat("Press ENTER to restart"), 15)/2, screenHeight/2 + 50, 15, BLACK);
    }
    EndDrawing();
}

//------------------------------------------------------------------------------------
// Update Game Function
//------------------------------------------------------------------------------------

void updateGame(void)
{
    // Local Variables
    //------------------------------------------
    map.scrollingBack -= 0.1f;
    map.scrollingFore -= 3.0f;
    if (map.scrollingBack <= -(float) map.background.width * 2) map.scrollingBack = 0;
    if (map.scrollingFore <= -(float) map.foreground.width * 2) map.scrollingFore = 0;

    Rectangle birdRec = {bird.x + 5, bird.y - bird.birdSprite.height + 15, bird.frameWidth - 10,bird.birdSprite.height - 10};
    Rectangle topRec = {bird.x, -50, bird.frameWidth-10, map.foreground.height};
    Rectangle bottomRec = {bird.x, map.foregroundY, bird.frameWidth-10, map.foreground.height};

    if (IsSoundPlaying(effect.bgMusic) == false) PlaySound(effect.bgMusic);

    if (!gameOver)
    {
        // Map Scrolling, Character and Pipe Position
        //------------------------------------------
        map.framesCounter++;
        if (map.framesCounter >= (60 / map.framesSpeed))
        {
            map.framesCounter = 0;
            currentFrame++;

            if (currentFrame > 2) currentFrame = 0;

            birdRec.x = (float) currentFrame * (float) bird.birdSprite.width / 3;
        }

        // Character jumps and falls
        //------------------------------------------
        if (!gameOver && IsKeyPressed(KEY_SPACE))
        {
            bird.isJumping = 1;
            PlaySound(effect.jump);
        }

        if (bird.isJumping == 1)
        {
            gameRun = 1;
            for (int i = 0; i < MAX_PIPES; i++)
            {
                pipe[i].x -= speed;
                pipe[i].topPipeRec.x -= speed;
                pipe[i].bottomPipeRec.x -= speed;
            }
            if (bird.y < map.ground && bird.y > map.ceiling) jump();
        }

        // Check for Collision and Regenerate pipes
        //------------------------------------------

        // Collision between the character and map's ground/ceiling
        if (CheckCollisionRecs(birdRec, topRec) || CheckCollisionRecs(birdRec, bottomRec))
        {
            PlaySound(effect.hit);
            StopSound(effect.bgMusic);
            gameOver = true;
        }

        for(int i = 0; i < MAX_PIPES; i++)
        {
            // Regenerate pipes
            if (pipe[i].x < -topPipe_frameWidth)
            {
                for( int j = 0; j < MAX_PIPES; j++ ) if ( pipe[j].x > max_x )max_x = pipe[j].x;     // Find maximum x of pipes
                if ( pipe[i].x > max_x )max_x = pipe[i].x;
                pipe[i].x = max_x + DIST_PIPE;
                randomPipe(i);
            }

            // Collision between the character and pipes
            if ((CheckCollisionRecs(birdRec, pipe[i].topPipeRec) || CheckCollisionRecs(birdRec, pipe[i].bottomPipeRec)) && pipe[i].active)
            {
                PlaySound(effect.hit);
                StopSound(effect.bgMusic);
                gameOver = true;
            }
            else if ((pipe[i].topPipeRec.x + pipe[i].topPipeRec.width < birdRec.x) && (pipe[i].bottomPipeRec.x + pipe[i].bottomPipeRec.width< bird.x) && !gameOver && pipe[i].active)
            {
                PlaySound(effect.point);
                score++;
                pipe[i].active = false;
            }
        }

        // Scoring and Speed
        //------------------------------------------
        if (score % 5 == 0 && score != 0) speed += 0.005f;        // Increases speed every 5 points gain
        if (score > hiScore)                                      // Set and record high score
        {
            hiScore = score;
            recHiScore();
        }
    }
    else
    {
        // Game Over
        //------------------------------------------
        if (bird.rotation <= 30) bird.rotation += 4;
        bird.acceleration += bird.gravity * GetFrameTime();

        if (bird.acceleration >= bird.gravity) bird.acceleration = bird.gravity;

        bird.velocity += bird.acceleration * GetFrameTime() * 10;
        bird.y += bird.velocity * GetFrameTime() * 5;

        if (CheckCollisionRecs(birdRec, bottomRec)) bird.y = bottomRec.y + 15;

        // Restart the Game
        //------------------------------------------
        if (IsKeyPressed(KEY_ENTER)) InitGame();
    }
}

//------------------------------------------------------------------------------------
// Game Textures Functions
//------------------------------------------------------------------------------------

void loadTexture(void)
{
    static const char backgroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/background.png";
    static const char foregroundPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/foreground.png";
    static const char birdPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/bird.png";
    static const char topPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/topPipe.png";
    static const char bottomPipePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/bottomPipe.png";
    static const char gameOverPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/gameOver.png";
    static const char scoreBoardPath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/scoreBoard.png";
    static const char titlePath[] = "/Users/neneprm/Desktop/C/FlappyBird/assets/title.png";

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
    title = LoadTexture(titlePath);
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
    UnloadTexture(title);
}

//------------------------------------------------------------------------------------
// Pipe Functions
//------------------------------------------------------------------------------------

void randomPipe(int i)
{
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

//------------------------------------------------------------------------------------
// Sound Effects Functions
//------------------------------------------------------------------------------------

void loadSound(void)
{
    static const char hitPath[] = "/Users/neneprm/Desktop/C/FlappyBird/effect/hit.mp3";
    static const char jumpPath[] = "/Users/neneprm/Desktop/C/FlappyBird/effect/jump.mp3";
    static const char pointPath[] = "/Users/neneprm/Desktop/C/FlappyBird/effect/point.mp3";
    static const char bgMusicPath[] = "/Users/neneprm/Desktop/C/FlappyBird/effect/bgMusic.mp3";

    effect.hit = LoadSound(hitPath);
    effect.jump = LoadSound(jumpPath);
    effect.point = LoadSound(pointPath);
    effect.bgMusic = LoadSound(bgMusicPath);

    SetSoundVolume(effect.jump, 0.3);
}

void unloadSound(void)
{
    UnloadSound(effect.hit);
    UnloadSound(effect.jump);
    UnloadSound(effect.point);
    UnloadSound(effect.bgMusic);
}

//------------------------------------------------------------------------------------
// High Score Functions
//------------------------------------------------------------------------------------

void loadHiScore(void)
{
    FILE *inFile;
    inFile = fopen("hiScore.txt", "r");
    if (inFile == NULL)
    {
        printf("Could Not Open File!\n");
        exit(-1);
    }
    else
    {
        fscanf(inFile, "%d", &hiScore);
        printf("%d\n",hiScore);
    }
    fclose(inFile);
}

void recHiScore(void)
{
    FILE *outFile;
    outFile = fopen("hiScore.txt", "w");
    if (outFile != NULL) fprintf(outFile, "%d", hiScore);
    fclose(outFile);
}