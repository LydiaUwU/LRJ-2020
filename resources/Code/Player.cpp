#include "Player.h"

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