#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   //PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include <iostream>
#include <cstdlib>
#include "raylib.h"
//#include "Player.h"

//TODO: Move the Player class to its own files
class Player {
private:
    const float radius = 0.2f;
    const float speed = 0.1f;
    const float rotSpeed = 0.02f;
    const float targetOffset = 0.5f;
    const Vector3 cameraOffset = { -10.0f, 3.0f, 0.0f };
    const float rotRadius = sqrt(pow(cameraOffset.x, 2) + pow(cameraOffset.y, 2));

    float rotPos = 0.0f;

public:
    //Define the camera to look into our 3d world
    Vector3 position = { 10.0f, 20.0f, 10.0f };
    Camera3D camera = { 0 };

    Player() {
        camera.position = { position.x + cameraOffset.x, position.y + cameraOffset.y, position.z + cameraOffset.z }; // Camera position
        camera.target = { position.x, position.y + targetOffset, position.z};      // Camera looking at point
        camera.up = { 0.0f, 1.8f, 0.0f };          // Camera up vector (rotation towards target)
        camera.fovy = 10.0f;                                // Camera field-of-view Y
        camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
    }

    void Update() {
        //TODO: Set player position to be relative to the ground level (probably using rays)
        //TODO: Movement is relative to camera direction
        if (IsKeyDown(KEY_UP)) position.x += speed;
        if (IsKeyDown(KEY_DOWN)) position.x -= speed;
        if (IsKeyDown(KEY_RIGHT)) position.z += speed;
        if (IsKeyDown(KEY_LEFT)) position.z -= speed;

        if (IsKeyDown(KEY_Z)) rotPos -= rotSpeed;
        if (IsKeyDown(KEY_X)) rotPos += rotSpeed;

        float cdx = rotRadius * sin(rotPos);
        float cdz = rotRadius * cos(rotPos);

        camera.position = { position.x + cdx, position.y + cameraOffset.y, position.z + cdz };
        camera.target = { position.x, position.y + targetOffset, position.z};
    }

    void Draw() const {
        DrawSphere(position, radius, WHITE);
    }
};

int main() {
    //Initialization
    const int screenWidth = 512;
    const int screenHeight = 512;
    const int worldSize = 128;
    const int worldHeight = 16;

    SetConfigFlags(FLAG_MSAA_4X_HINT);      // Enable Multi Sampling Anti Aliasing 4x (if available)

    InitWindow(screenWidth, screenHeight, "Genisarboria");

    //TODO: Third person camera

    SetTargetFPS(60);                   //Set our game to run at 60 frames-per-second

    //Define the shaderPixels
    Shader shaderPixel = LoadShader(nullptr, FormatText("LRJ-2020/resources/assets/shaders/glsl%i/pixelizer.fs", GLSL_VERSION));

    //Create a RenderTexture2D to be used for render to texture512
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    //Generate world
    Image perlinNoise = GenImagePerlinNoise(worldSize, worldSize, std::rand(), std::rand(), 1);
    Texture2D worldTexture = LoadTextureFromImage(perlinNoise);

    Mesh worldMesh = GenMeshHeightmap(perlinNoise, { worldSize, worldHeight, worldSize});
    Model worldModel = LoadModelFromMesh(worldMesh);

    worldModel.materials[0].maps[MAP_DIFFUSE].texture = worldTexture;
    Vector3 worldPosition = { -worldSize / 2, 0.0, -worldSize / 2};

    //Define Player
    Player player;

    //Main game loop
    while (!WindowShouldClose()) { //Detect window close button or ESC key
        //Update
        player.Update();
        UpdateCamera(&player.camera);          //Update camera

        //Draw
        BeginDrawing();
            ClearBackground(ORANGE);

            BeginTextureMode(target);
                ClearBackground(ORANGE);

                BeginMode3D(player.camera);
                    DrawModel(worldModel, worldPosition, 1.0, YELLOW);
                    player.Draw();

                EndMode3D();

            EndTextureMode();

            //Draw rendertexture shaders
            BeginShaderMode(shaderPixel);
                //NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
                DrawTextureRec(target.texture, { 0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height) }, { 0, 0 }, WHITE);

            EndShaderMode();

        EndDrawing();
    }

    //De-Initialization
    UnloadRenderTexture(target);
    UnloadTexture(worldTexture);
    UnloadModel(worldModel);

    CloseWindow();        //Close window and OpenGL context

    return 0;
}