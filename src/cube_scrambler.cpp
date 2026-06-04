#include "cube.h"
#include "defines.h"

void Cube::_generate_scramble(int length){
	Vector3 faces[] = {
		_RIGHT_FACE,
		_LEFT_FACE,
		_TOP_FACE,
		_BOTTOM_FACE,
		_FRONT_FACE,
		_BACK_FACE,
	};

	int lastAxis = -1;

	for (int i = 0; i < length; i++) {
		int faceIndex = GetRandomValue(0, 5);
		int axis = faceIndex / 2;

		while (axis == lastAxis) {
			faceIndex = GetRandomValue(0, 5);
			axis = faceIndex / 2;
		}

		moves.push_back({FACE, faces[faceIndex], GetRandomValue(0, 1) ? _C_WISE : _CC_WISE});
		lastAxis = axis;
	}
}
