#include <raylib.h>
#include <malloc.h>

#define PLAYER_HOR_SPEED 200.0f
#define HOR_SPACING 10
#define VER_SPACING 10
#define MAX_TOUCH_POINTS 10

/* LOCALES DATA STRUCT */

typedef struct Player {
    Vector2 position;
    Rectangle rect;
    float speed;
}Player;

typedef struct TouchPoint {
        float x;
        float y;
}TouchPoint;

typedef struct Key {
    Rectangle shape;
    bool isPressed;
}Key;

typedef struct PadControl {
    Key upKey;
    Key downKey;
    Key leftKey;
    Key rightKey;
}PadControl;

/* LOCALES HELPERS FUNCTIONS */
void CreateGameController (PadControl* pd, int width, int height);
void DrawPadController(const PadControl* pd);
void UpdatePadControllerState(PadControl* pd, Vector2* touch);
void UpdatePlayer(Player *player, PadControl *pd, float dt);
void UpdateCameraCenter(Camera2D* camera, Player *player, float dt, int width, int height);

/* MAIN PROGRAM ENTRY POINT */

int main(void) {

    InitWindow(0, 0, "Raylib Android");

    const int width = GetScreenWidth();
    const int height = GetScreenHeight();

    SetWindowSize(width, height);
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    //DisableCursor();        // Doesn't work on mobile
    SetTargetFPS(60);

    // GAME OBJECTS
    // ------------
    Vector2 touchPositions[MAX_TOUCH_POINTS] = {0};

    // Player object
    Player player = {0};

    player.position = (Vector2){(float)width/2.0f, (float)height/2.0f};
    player.rect = (Rectangle){(float)width/2.0f - 25,
                              (float)height/2.0f - 25,
                              50,
                              50};
    player.speed = 0.0f;

    // Camera 2D object
    Camera2D camera = {0};

    camera.target = player.position;
    camera.offset = (Vector2){(float)width/2.0f, (float)height/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // PadController object
    PadControl pd = {0};
    CreateGameController(&pd, width, height);

    while(!WindowShouldClose()) {

        // PROCESS INPUT
        //-------------

        // How many times user pressed the screen
        int tCount = GetTouchPointCount();

        // Clamp the set points available
        if(tCount > MAX_TOUCH_POINTS) {tCount = MAX_TOUCH_POINTS;}

        // Get touch points positions
        for (int i = 0; i < tCount; ++i) {
            touchPositions[i] = GetTouchPosition(i);
        }

        // UPDATE
        //--------

        float deltaTime = GetFrameTime();

        int w = GetScreenWidth();
        int h = GetScreenHeight();

        // Update player and pad controller state

        if(tCount > 0) {
            for (int i = 0; i < tCount; i++) {
                UpdatePadControllerState(&pd, &touchPositions[i]);
                UpdatePlayer(&player, &pd, deltaTime);
            }
        }

        // Draw
        //-----
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                // Draw Pad controller
                DrawPadController(&pd);

                // Draw player
                DrawCube((Vector3){player.position.x, player.position.y, 0.0f},
                                 player.rect.width,
                                 player.rect.height,
                                 0.0f,
                                 RED);
            EndMode2D();

            DrawText(TextFormat("Touch count = %d\n",tCount),
                     10,
                     height/4,
                     20,
                     BLACK);

            if (tCount > 0) {
                for (int i = 0; i < tCount; i++) {
                    DrawText(TextFormat("Touch %d :\n X = %.2f\n Y = %.2f\n",
                                        i,
                                        touchPositions[i].x,
                                        touchPositions[i].y),
                             10,
                             height/2,
                             20, BLACK);
                }
            }

        EndDrawing();
    }

    // De-initialization and free unused memory space
    //free(padControl);
    CloseWindow();

    return 0;
}

/* LOCALES FUNCTIONS DEFINITION */

void CreateGameController (PadControl *pd, int width, int height) {
    // Create left key
    pd->leftKey.shape = (Rectangle) {100, (float)height * 3.0f/4.0f, 100.0f, 100.0f};
    pd->leftKey.isPressed = false;

    // Create up key
    pd->upKey.shape = (Rectangle){222, (float)height /2.0f + 78.0f, 100.0f, 100.0f };
    pd->upKey.isPressed = false;

    // Create right key
    pd->rightKey.shape = (Rectangle){322, (float)height * 3.0f/4.0f, 100.0f, 100.0f};
    pd->rightKey.isPressed = false;

    // Create down key
    pd->downKey.shape = (Rectangle){222, (float)height * 3.0f/4.0f + 100.0f, 100.0f, 100.0f};
    pd->downKey.isPressed = false;
}

void DrawPadController(const PadControl *pd) {
    // Draw the left key on pad controller
    DrawCube((Vector3){pd->leftKey.shape.x + 20.0f, pd->leftKey.shape.y, 0.0f},
             pd->leftKey.shape.width,
             pd->leftKey.shape.height,
            0.0f,
            BLACK);
    // Draw the up key
    DrawCube((Vector3){pd->upKey.shape.x, pd->upKey.shape.y, 0.0f},
             pd->upKey.shape.width,
             pd->upKey.shape.height,
             0.0f,
             BLACK);
    // Draw right key
    DrawCube((Vector3){pd->rightKey.shape.x, pd->rightKey.shape.y, 0.0f},
             pd->rightKey.shape.width,
             pd->rightKey.shape.height,
             0.0f,
             BLACK);
    // Draw down key
    DrawCube((Vector3){pd->downKey.shape.x, pd->downKey.shape.y, 0.0f},
             pd->downKey.shape.width,
             pd->downKey.shape.height,
             0.0f,
             BLACK);
}

void UpdatePadControllerState(PadControl* pd, Vector2* touch) {
    // When left key is pressed
    if (
        ((touch->x >= pd->leftKey.shape.x) && (touch->x <= (pd->leftKey.shape.x + pd->leftKey.shape.width)))
        &&
        ((touch->y >= pd->leftKey.shape.y) && (touch->y <= (pd->leftKey.shape.y + pd->leftKey.shape.height)))
        )
    {
        pd->leftKey.isPressed =  true;
    }
    else {
        pd->leftKey.isPressed = false;
    }

    // when right key is pressed
    if (
            ((touch->x >= pd->rightKey.shape.x) && (touch->x <= (pd->rightKey.shape.x + pd->rightKey.shape.width)))
            &&
            ((touch->y >= pd->rightKey.shape.y) && (touch->y <= (pd->rightKey.shape.y + pd->rightKey.shape.height)))
        )
    {
        pd->rightKey.isPressed = true;
    }
    else {
        pd->rightKey.isPressed = false;
    }

    // when up key is pressed
    if (
            ((touch->x >= pd->upKey.shape.x) && (touch->x <= (pd->upKey.shape.x + pd->upKey.shape.width)))
            &&
            ((touch->y >= pd->upKey.shape.y) && (touch->y <= (pd->upKey.shape.y + pd->upKey.shape.height)))
        )
    {
        pd->upKey.isPressed = true;
    }
    else {
        pd->upKey.isPressed = false;
    }

    // when down key is pressed
    if (
            ((touch->x >= pd->downKey.shape.x) && (touch->x <= (pd->downKey.shape.x + pd->downKey.shape.width)))
            &&
            ((touch->y >= pd->downKey.shape.y) && (touch->y <= (pd->downKey.shape.y + pd->downKey.shape.height)))
        )
    {
        pd->downKey.isPressed = true;
    }
    else {
        pd->downKey.isPressed = false;
    }
}

void UpdatePlayer(Player *player, PadControl *pd, float dt) {
    if (pd->leftKey.isPressed) {player->position.x -= PLAYER_HOR_SPEED * dt;}
    if (pd->rightKey.isPressed) {player->position.x += PLAYER_HOR_SPEED * dt;}
    if (pd->upKey.isPressed) {player->position.y -= PLAYER_HOR_SPEED * dt;}
    if (pd->downKey.isPressed) {player->position.y += PLAYER_HOR_SPEED * dt;}
}

void UpdateCameraCenter(Camera2D* camera, Player *player, float dt, int width, int height) {

}
