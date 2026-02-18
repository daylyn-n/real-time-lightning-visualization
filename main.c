#include <raylib.h>
#include <raymath.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

void PreDraw()
{
    // setting up the window
    InitWindow(WIDTH, HEIGHT, "Lighnting Visualizer");
    SetTargetFPS(60);
}
static float ClampFloat(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void UpdateGlobe(float *yaw, float *pitch)
{
    // settings for how we move the globe
    const float sensitivity = 0.004f;
    const float pitchLimit = DEG2RAD*85.0f; // everything is in radians.
    const float zoomSpeed = 1.10f; 

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 d = GetMouseDelta();
        
        *yaw += d.x * sensitivity;
        *pitch += d.y * sensitivity;
        *pitch = ClampFloat(*pitch, -pitchLimit, pitchLimit);
    }

}

void UpdateCam(Camera *cam, float *camDistance)
{
    float wheel = GetMouseWheelMove();


    const float zoomSpeed = 1.10f;
    if(wheel != 0.0f)
    {
        if (wheel > 0)
           *camDistance /= zoomSpeed;
        else
            *camDistance *= zoomSpeed;

        *camDistance = ClampFloat(*camDistance, 2.5f, 50.0f);

    }
    // Fixed camera looking at origin
    cam->position = (Vector3){ 0.0f, 1.5f, *camDistance };

}
void Draw()
{
    // setting up basic camera stuff
    Camera3D camera = {0};
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Globe rotation (what you drag)
    float globeYaw = 0.0f;
    float globePitch = 0.0f;


    // Camera zoom (wheel) 
    float camDistance = 10.0f;

    // settings for how we move the globe
    const float sensitivity = 0.004f;
    const float pitchLimit = DEG2RAD*85.0f; // everything is in radians.
    const float zoomSpeed = 1.10f;

    while(!WindowShouldClose())
    {
        UpdateGlobe(&globeYaw, &globePitch);
        
        UpdateCam(&camera, &camDistance);
        // Globe rotation matrix
        Matrix rotY = MatrixRotateY(globeYaw);
        Matrix rotX = MatrixRotateX(globePitch);
        Matrix globeRot = MatrixMultiply(rotY, rotX);

        BeginDrawing();
           ClearBackground(WHITE);

            BeginMode3D(camera);
            // Sphere itself doesn't show rotation (symmetric),
            // so we draw a marker point on the surface and rotate it.
            DrawSphere((Vector3){0,0,0}, 1.0f, GREEN);
            DrawSphereWires((Vector3){0,0,0}, 1.01f, 24, 24, DARKGREEN);

            // Marker at "equator, 0 lon" before rotation
            Vector3 markerLocal = (Vector3){ 1.0f, 0.0f, 0.0f };
            Vector3 markerWorld = Vector3Transform(markerLocal, globeRot);
            DrawSphere(markerWorld, 0.05f, RED);


            DrawGrid(20, 1.0f);

        EndMode3D();


        DrawText("LMB drag: spin globe | Wheel: zoom", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 40);

        EndDrawing();
    }

    
}
void CleanUp()
{
    CloseWindow();
}
void InitGlobe()
{
    PreDraw();
    Draw();
    CleanUp();
}
int main(void)
{
    
    InitGlobe();
}
