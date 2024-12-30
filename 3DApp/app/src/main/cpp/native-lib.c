#include "raylib.h"
#include "rcamera.h"


#include <math.h>


/* GLOBALS DATA TYPE */

typedef enum {
    BUTTON_NONE = -1,
    BUTTON_UP,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_DOWN,
    BUTTON_MAX
}PadButton;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------

    InitWindow(0, 0, "3D game");;
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    SetWindowSize(width, height);

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    // Define camera object
    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Define player object

    Vector3 playerPosition = {0.0f,1.0f, 0.0f};
    Vector2 playerScreenPosition = {0.0f, 0.0f};

    float playerSpeed = 5.4f;   // units per second


    // Define pad object
    Vector2 padPosition = {100, (float)height * 3.0f /4.0f};
    float buttonRadius = 30.0f;

    // Array of buttons
    Vector2 buttonPositions[BUTTON_MAX] =
    {
            [0]={padPosition.x, padPosition.y - buttonRadius * 1.5f},   // Up
            [1]={padPosition.x - buttonRadius * 1.5f, padPosition.y},   // Left
            [2]={padPosition.x + buttonRadius * 1.5f, padPosition.y},   // right
            [3]={padPosition.x, padPosition.y + buttonRadius * 1.5f}    // Down
    };

    const char* buttonLabels[BUTTON_MAX] =
    {
            [0] = "U",  // Up
            [1] = "L",  // Left
            [2] = "R",  // Right
            [3] = "D"   // Down
    };

    Color buttonLabelColors[BUTTON_MAX] =
    {
        [0] = YELLOW,   // Up
        [1] = BLUE,     // Left
        [2] = RED,      // Right
        [3] = GREEN     // Down
    };

    int pressedButton = BUTTON_NONE;
    Vector2 inputPosition = {0.0f, 0.0f};

    // Game loop
    //-------------------------------------------------------------------------------

    while(!WindowShouldClose()) {

        // PROCESS INPUT
        //----------------------------------------------------------------------------
        if(GetTouchPointCount() > 0){
            // use touch position
            inputPosition = GetTouchPosition(0);
        }
        else {
            // use mouse position
            inputPosition = GetMousePosition();
        }

        // Reset pressed button  to none
        pressedButton = BUTTON_NONE;

        // Make sure user is pressing left mouse button if they're from desktop
        if ((GetTouchPointCount() > 0) || ((GetTouchPointCount() == 0) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
        {
            // Find nearest D-Pad button to the input position
            for (int i = 0; i < BUTTON_MAX; i++)
            {
                float distX = fabsf(buttonPositions[i].x - inputPosition.x);
                float distY = fabsf(buttonPositions[i].y - inputPosition.y);

                if ((distX + distY < buttonRadius))
                {
                    pressedButton = i;
                    break;
                }
            }
        }

        // UPDATE
        //----------------------------------------------------------------------------

        // Calculate player screen space position (with little offset to be in top)
        playerScreenPosition = GetWorldToScreen((Vector3){playerPosition.x, playerPosition.y + 1.5f, playerPosition.z}, camera);

        // Move player according to pressed button
        switch (pressedButton)
        {
            case BUTTON_UP:
            {
                playerPosition.z -= playerSpeed*GetFrameTime();
                CameraMoveForward(&camera, playerSpeed * GetFrameTime(), true);
                break;
            }
            case BUTTON_LEFT:
            {
                playerPosition.x -= playerSpeed*GetFrameTime();
                CameraYaw(&camera, 0.261799f * GetFrameTime(), false);
                break;
            }
            case BUTTON_RIGHT:
            {
                playerPosition.x += playerSpeed*GetFrameTime();
                CameraYaw(&camera, -0.261799f * GetFrameTime(), false);
                break;
            }
            case BUTTON_DOWN:
            {
                playerPosition.z += playerSpeed*GetFrameTime();
                CameraMoveForward(&camera, -playerSpeed * GetFrameTime(), true);
                break;
            }
            default: break;
        };

        // DRAW
        //---------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawCube(playerPosition, 2.0f, 2.0f, 2.0f, RED);
                DrawGrid(10, 1.0f);
            EndMode3D();


            // Draw GUI
            for (int i = 0; i < BUTTON_MAX; i++)
            {
                DrawCircleV(buttonPositions[i],
                            buttonRadius,
                            (i == pressedButton)? DARKGRAY : BLACK);

                DrawText(buttonLabels[i],
                         (int)buttonPositions[i].x - 7,
                         (int)buttonPositions[i].y - 8,
                         20,
                         buttonLabelColors[i]);
            }

            DrawText("move the player with D-Pad buttons",
                     10,
                     10,
                     20,
                     DARKGRAY);

            // Draw a Gizmo
            //------------------------------------

            // Draw Blue Arrow on top of the player (y axis)
            DrawLine((int)playerScreenPosition.x,
                     (int)playerScreenPosition.y,
                     (int)playerScreenPosition.x + 1,
                     (int)playerScreenPosition.y - 100,
                     BLUE);
            // Draw Green arrow on top of the player (x axis)
            DrawLine((int)playerScreenPosition.x,
                     (int)playerScreenPosition.y,
                     (int)playerScreenPosition.x + 100,
                     (int)playerScreenPosition.y + 1,
                     GREEN);
            // Draw yellow arrow on top of the player (z axis)

            DrawFPS(20, height/4);

        EndDrawing();
    }

    // FREE RESOURCES
    //-------------------------------------------------------------------------------

    CloseWindow();

    return 0;
}