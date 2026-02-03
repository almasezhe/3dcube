//for implement with real imported models

#include "raylib.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const int screenWidth = 450;
const int screenHeight = 450;
float degree=0;
float degree_sin=std::sin(degree);
float degree_cos=std::cos(degree);

struct CameraDegrees {
    float yaw=0;
    float yaw_sin=std::sin(yaw);
    float yaw_cos=std::cos(yaw);

    float pitch=0;
    float pitch_sin=std::sin(pitch);
    float pitch_cos=std::cos(pitch);
};

CameraDegrees camera;

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
    rotated_coordinates.x=(dot_coordinates.x*degree_cos)- (dot_coordinates.y* degree_sin);
    rotated_coordinates.y=(dot_coordinates.y*degree_cos)+ (dot_coordinates.x* degree_sin);
    rotated_coordinates.z=dot_coordinates.z;

    return rotated_coordinates;
};

Vector3 rotate_camera(Vector3 dot_coordinates, float yaw, float pitch){
    Vector3 rotated_coordinates;

    // camera.yaw
    rotated_coordinates.x =  dot_coordinates.x*camera.yaw_cos + dot_coordinates.z*camera.yaw_sin;
    float z1 = -dot_coordinates.x*camera.yaw_sin + dot_coordinates.z*camera.yaw_cos;
    float y1 =  dot_coordinates.y;
    // pitch
    rotated_coordinates.y =  y1*camera.pitch_cos - z1*camera.pitch_sin;
    rotated_coordinates.z =  y1*camera.pitch_sin + z1*camera.pitch_cos;
    return rotated_coordinates;
};


Vector2 project(Vector3 initial_cords){
    /* x'=x/z */
    Vector2 coordinates;
    if (initial_cords.z <= 0.1f) initial_cords.z=0.1;
    coordinates.x = initial_cords.x/initial_cords.z;
    coordinates.y = initial_cords.y/initial_cords.z;
    return coordinates;
}


std::vector<Vector3> parser() {
    std::ifstream file("models/skull.obj");
    std::string line;

    std::vector<Vector3> vertices;
    vertices.reserve(10000);

    
    while (std::getline(file, line)) {
        if (line.rfind("v ", 0) == 0) {
            char v;
            float x, y, z;
            std::stringstream ss(line);
            ss >> v >> x >> y >> z;
            vertices.push_back({x,y,z});

            std::cout << x << " " << y << " " << z << "\n";
        }
    }
    return vertices;
}


int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib basic 3d math");

/*    float x=0.1f;
    float y=0.1f;
    float z=0.1f;
    float z2=0.2f;
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
*/

    std::vector<Vector3> initial_cords= parser();
    
    SetTargetFPS(60);      


    while (!WindowShouldClose())    
    {

        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_UP))
        {
            for (int i = 0; i < initial_cords.size(); i++)
                initial_cords[i].z -= 1.5f;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            for (int i = 0; i < initial_cords.size(); i++)
                initial_cords[i].z += 1.5f;
        }   

        if (IsKeyDown(KEY_RIGHT)) {
            degree += 0.1f;
            degree_cos=std::cos(degree);
            degree_sin=std::sin(degree);
        }
        if (IsKeyDown(KEY_LEFT)) {
            degree -= 0.1f;
            degree_cos=std::cos(degree);
            degree_sin=std::sin(degree);}
        
        if (IsKeyDown(KEY_A)){
            camera.yaw += 0.05f;
            camera.yaw_cos=std::cos(camera.yaw);
            camera.yaw_sin=std::sin(camera.yaw);
        }
        if (IsKeyDown(KEY_D)){
            camera.yaw -= 0.05f;
            camera.yaw_cos=std::cos(camera.yaw);
            camera.yaw_sin=std::sin(camera.yaw);
        }
        if (IsKeyDown(KEY_W)){
            camera.pitch += 0.05f;
            camera.pitch_cos=std::cos(camera.pitch);
            camera.pitch_sin=std::sin(camera.pitch);
        }
        if (IsKeyDown(KEY_S)){
            camera.pitch -= 0.05f;
            camera.pitch_cos=std::cos(camera.pitch);
            camera.pitch_sin=std::sin(camera.pitch);}
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the cube edges with arrow keys", 10, 10, 20, DARKGRAY);

            for (int i = 0; i < initial_cords.size(); i++){
                Vector2 coordinates = {center_on_screen(project(rotate_camera(rotate_cube(initial_cords[i],degree),camera.yaw,camera.pitch)))};
                Vector2 ballPosition = {coordinates.x,coordinates.y};   
                DrawPixelV(ballPosition, MAROON);

//                screen_positions[i]=coordinates;
            }  
//            for (int i = 0; i < 12; i++){
//                DrawLineEx(screen_positions[edges[i][0]],screen_positions[edges[i][1]],5,MAROON);
//            }
            EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}