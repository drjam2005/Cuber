#include "raylib.h"
#include "cube.h"


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;


	SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Cuber");
	SetExitKey(KEY_NULL);

    int screenWidthActual = GetScreenWidth();
    int screenHeightActual = GetScreenHeight();

    int posX = (screenWidthActual - screenWidth) / 2;
    int posY = (screenHeightActual - screenHeight) / 2;

    SetWindowPosition(posX, posY);

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
    while (!WindowShouldClose() && !cube.IsEnded()) {
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
