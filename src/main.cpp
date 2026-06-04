#include "raylib.h"
#include "cube.h"


int main() {
    InitWindow(1024, 768, "cuber");
    //SetTargetFPS(60);

    Camera3D camera = {0};
    camera.position = { 0.0f, 4.0f, 4.0f };
    camera.target   = { 0.0f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float rotation = 10.0f;
	rlDisableBackfaceCulling();

	Vector3 pos1 = {0.0f, 0.f, 0.0f};
	Vector3 rotations = {0.0f, 0.0f, 0.0f};

	Cube cube;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
			BeginMode3D(camera);
				cube.Update();
				cube.Render();
				DrawGrid(10, 1.0f);
			EndMode3D();


		DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
