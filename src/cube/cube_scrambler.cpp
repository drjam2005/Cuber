#include "cube.h"
#include "cube_defines.h"

std::string Cube::_generate_scramble(int length){
	Vector3 faces[] = {
		_RIGHT_FACE,
		_LEFT_FACE,
		_TOP_FACE,
		_BOTTOM_FACE,
		_FRONT_FACE,
		_BACK_FACE,
	};
	const char* faceNames[] = {"R", "L", "U", "D", "F", "B"};
	float angles[] = {_C_WISE, 180.0f, _CC_WISE};
	const char* suffixes[] = {"", "2", "'"};

	int lastAxis = -1;
	std::string scramble;

	for (int i = 0; i < length; i++) {
		int faceIndex = GetRandomValue(0, 5);
		int axis = faceIndex / 2;

		while (axis == lastAxis) {
			faceIndex = GetRandomValue(0, 5);
			axis = faceIndex / 2;
		}

		int turnIndex = GetRandomValue(0, 2);
		moves.push_back({FACE, faces[faceIndex], angles[turnIndex]});

		if (!scramble.empty())
			scramble += " ";

		scramble += faceNames[faceIndex];
		scramble += suffixes[turnIndex];

		lastAxis = axis;
	}

	scrambleText = scramble;
	return scramble;
}
