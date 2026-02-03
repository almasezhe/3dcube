//For testing on cube

#include "raylib.h"
#include <cmath>

const int screenWidth = 450;
const int screenHeight = 450;
float degree=0;
float yaw=0;
float pitch=0;

Vector2 center_on_screen(Vector2 coords){
    Vector2 v;

    v.x = (coords.x + 1) / 2 * screenWidth;
    v.y = (1 - (coords.y + 1) / 2) * screenHeight; 

    return v;
};

Vector3 rotate_cube(Vector3 dot_coordinates, float degree){
    /* x'= xcos(A)-ysin(A) */
    /* y'= ycos(A)-xsin(A) */
    Vector3 rotated_coordinates;
    rotated_coordinates.x=(dot_coordinates.x*std::cos((degree)))- (dot_coordinates.y* std::sin(degree));
    rotated_coordinates.y=(dot_coordinates.y*std::cos(degree))+ (dot_coordinates.x* std::sin(degree));
    rotated_coordinates.z=dot_coordinates.z;

    return rotated_coordinates;
};

Vector3 rotate_camera(Vector3 dot_coordinates, float yaw, float pitch){
    Vector3 rotated_coordinates;

    // yaw
    rotated_coordinates.x =  dot_coordinates.x*cos(yaw) + dot_coordinates.z*sin(yaw);
    float z1 = -dot_coordinates.x*sin(yaw) + dot_coordinates.z*cos(yaw);
    float y1 =  dot_coordinates.y;
    // pitch
    rotated_coordinates.y =  y1*cos(pitch) - z1*sin(pitch);
    rotated_coordinates.z =  y1*sin(pitch) + z1*cos(pitch);
    return rotated_coordinates;
};

Vector2 project(Vector3 initial_cords){
    /* x'=x/z */
    Vector2 coordinates;
    coordinates.x = initial_cords.x/initial_cords.z;
    coordinates.y = initial_cords.y/initial_cords.z;
    return coordinates;
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib basic 3d math");

    float x=0.1f;
    float y=0.1f;
    float z=0.1f;
    float z2=0.3f;
    Vector3 initial_cords[8] = {
    { x,  y, z},
    {-x,  y, z},
    {-x, -y, z},
    { x, -y, z},

    { x,  y, z2},
    {-x,  y, z2},
    {-x, -y, z2},
    { x, -y, z2}
    };  

    Vector2 screen_positions[8];

    int edges[][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};



    SetTargetFPS(60);      


    while (!WindowShouldClose())    
    {

        // Update
    float wheel = GetMouseWheelMove();
        
        if (IsKeyDown(KEY_RIGHT)) degree += 0.05f;
        if (IsKeyDown(KEY_LEFT))  degree -= 0.05f;

        if (IsKeyDown(KEY_UP))
        {
            for (int i = 0; i < 8; i++)
                initial_cords[i].z -= 0.01f;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            for (int i = 0; i < 8; i++)
                initial_cords[i].z += 0.01f;
        }   
        if (IsKeyDown(KEY_A)) yaw += 0.05f;
        if (IsKeyDown(KEY_D)) yaw -= 0.05f;
        if (IsKeyDown(KEY_W)) pitch += 0.05f;
        if (IsKeyDown(KEY_S)) pitch -= 0.05f;


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the cube edges with arrow keys", 10, 10, 20, DARKGRAY);

            for (int i = 0; i < 8; i++){
                //TODO
                // ADD YAW AND PITCH
                //Vector2 coordinates = {center_on_screen(project(rotate_cube(initial_cords[i],degree)))};
                Vector2 coordinates = {center_on_screen(project(rotate_camera(rotate_cube(initial_cords[i],degree),yaw,pitch)))};
                Vector2 ballPosition = {coordinates.x,coordinates.y};
                screen_positions[i]=coordinates;
                DrawCircleV(ballPosition, 10, MAROON);
            }  
            //for (int i = 0; i < 12; i++){
              //  DrawLineEx(screen_positions[edges[i][0]],screen_positions[edges[i][1]],5,MAROON);
            //}
            EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}