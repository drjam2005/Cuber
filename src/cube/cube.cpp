#include "cube.h"

Cube::Cube() {
    camera = {0};
    camera.position = { 0.0f, 4.0f, 4.0f };
    camera.target   = { 0.0f, 0.0f, 0.0f };
    camera.up       = { 0.0f, 1.0f, 0.0f };
    camera.fovy     = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void Cube::Render() {
	BeginMode3D(camera);
	_render_scene();
	EndMode3D();
}

void Cube::Update() {

	_update_data();
	_handle_key_input();

}

bool Cube::IsMoving(){
	return isMoving;
}

bool Cube::IsEnded() {
	return closeRequested;
}

std::string Cube::GetScramble() const {
	return scrambleText;
}
