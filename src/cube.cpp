#include "cube.h"

#include <iostream>

void Cube::Render() {
	_render_scene();
}

void Cube::Update() {

	_update_data();
	if(_handle_key_input())
		if(IsSolved()) std::cout << "Solved!" << '\n';

}

bool Cube::IsEnded() {
	return closeRequested;
}
