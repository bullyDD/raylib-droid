#include <raylib.h>
#include <malloc.h>

#define PLAYER_HOR_SPEED 200.0f
#define HOR_SPACING 10
#define VER_SPACING 10

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
void UpdatePadControllerState(PadControl* pd, TouchPoint* touch);
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

    // Player object
    Player player = {0};

    player.position = (Vector2){(float)width/2.0f, (float)height/2.0f};
    player.rect = (Rectangle){(float)width/2.0f - 50,
                              (float)height/2.0f - 50,
                              100,
                              100};
    player.speed = 0.0f;

    // Camera 2D object
    Camera2D camera = {0};

    camera.target = player.position;
    camera.offset = (Vector2){(float)width/2.0f, (float)height/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // PadController object
    PadControl pd = {0};;
    CreateGameController(&pd, width, height);

    // Touches on screen
    TouchPoint t1 = {0};

    while(!WindowShouldClose()) {

        // PROCESS INPUT
        //-------------

        // How many times user pressed the screen
        int touchCount = GetTouchPointCount();

        // Zero touch on screen
        /*if (touchCount == 0) {
            pd.leftKey.isPressed    = false;
            pd.upKey.isPressed      = false;
            pd.rightKey.isPressed   = false;
            pd.downKey.isPressed    = false;
        }
        // Pad control for player movement
        else if (touchCount == 1) {

            // Get the touch position for movement

            // GetTouchPointId return the touch point identifier for a given index
            t1.x = GetTouchPosition(GetTouchPointId(touchCount)).x;
            t1.y = GetTouchPosition(GetTouchPointId(touchCount)).y;

            // Check for collision with LEFT_KEY and update player position
            if ( ((t1.x >= pd.leftKey.shape.x) && (t1.x <= pd.leftKey.shape.width) ) ||
                 ((t1.y >= pd.leftKey.shape.y) && (t1.y <= pd.leftKey.shape.height))  ) {
                pd.leftKey.isPressed = true;
            }
            else {
                pd.leftKey.isPressed = false;
            }
        }
        // Pad control for player action
        else if(true) {

        }*/
        UpdatePadControllerState(&pd, &t1);

        // UPDATE
        //--------
        float deltaTime = GetFrameTime();

        int w = GetScreenWidth();
        int h = GetScreenHeight();

        // Update player
        UpdatePlayer(&player, &pd, deltaTime);


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

            DrawText(TextFormat("Touch X = %.2f\nTouch Y = %.2f\nTouch point = %d\n", t1.x, t1.y, touchCount),
                     10,
                     height/2,
                     20,
                     BLACK);
            DrawText(TextFormat("Key\nX = %.2f\nY = %.2f\nwidth = %.2f\nheight = %.2f\n", pd.leftKey.shape.x, pd.leftKey.shape.y, pd.leftKey.shape.width, pd.leftKey.shape.height),
                     10,
                     height/4,
                     20,
                     BLACK);

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
    pd->leftKey.shape = (Rectangle) {50, (float)height * 3.0f/4.0f, 100.0f, 100.0f};
    pd->leftKey.isPressed = false;

    // Create up key
    pd->upKey.shape = (Rectangle){0};
    pd->upKey.isPressed = false;

    // Create right key
    pd->rightKey.shape = (Rectangle){0};
    pd->rightKey.isPressed = false;

    // Create down key
    pd->downKey.shape = (Rectangle){0};
    pd->downKey.isPressed = false;
}

void DrawPadController(const PadControl *pd) {
    // Draw the left key on pad controller
    DrawCube((Vector3){pd->leftKey.shape.x + 20.0f, pd->leftKey.shape.y, 0.0f},
             pd->leftKey.shape.width,
             pd->leftKey.shape.height,
            0.0f,
            BLACK);
}

void UpdatePadControllerState(PadControl* pd, TouchPoint* touch) {
    // How many times user pressed the screen
    int touchIndex = GetTouchPointCount();

    // Zero touch on screen
    if (touchIndex == 0) {
        pd->leftKey.isPressed    = false;
        pd->upKey.isPressed      = false;
        pd->rightKey.isPressed   = false;
        pd->downKey.isPressed    = false;
    }
        // Pad control for player movement
    if (touchIndex == 1) {

        // Get the touch position for movement

        // GetTouchPointId return the touch point identifier for a given index
        touch->x = GetTouchPosition(GetTouchPointId(touchIndex)).x;
        touch->y = GetTouchPosition(GetTouchPointId(touchIndex)).y;

        // Check for collision with LEFT_KEY and update player position
        if ( ((touch->x >= pd->leftKey.shape.x) && (touch->x <= pd->leftKey.shape.width) ) ||
             ((touch->y >= pd->leftKey.shape.y) && (touch->y <= pd->leftKey.shape.height))  ) {
            pd->leftKey.isPressed = true;
        }
        else {
            pd->leftKey.isPressed = false;
        }
    }
    // Pad control for player action
}

void UpdatePlayer(Player *player, PadControl *pd, float dt) {

    if(pd->leftKey.isPressed) {
        player->position.x -= PLAYER_HOR_SPEED * dt;
    }

}

void UpdateCameraCenter(Camera2D* camera, Player *player, float dt, int width, int height) {

}