#include "raylib.h"
#include "cube.h"


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;


	SetTraceLogLevel(LOG_NONE);
    InitWindow(screenWidth, screenHeight, "Cuber");
	SetExitKey(KEY_NULL);

	int monitor = GetCurrentMonitor();

    int screenWidthActual = GetMonitorWidth(monitor);
    int screenHeightActual = GetMonitorHeight(monitor);

    int posX = (screenWidthActual - screenWidth) / 2;
    int posY = (screenHeightActual - screenHeight) / 2;

    SetWindowPosition(posX, posY);


    float rotation = 10.0f;
	rlDisableBackfaceCulling();

	Vector3 pos1 = {0.0f, 0.f, 0.0f};
	Vector3 rotations = {0.0f, 0.0f, 0.0f};

	Cube cube;
    while (!WindowShouldClose() && !cube.IsEnded()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

				cube.Update();
				cube.Render();

				DrawGrid(10, 1.0f);

		DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
