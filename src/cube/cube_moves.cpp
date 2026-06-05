#include "cube.h"
#include "helpers.h"
#include "raymath.h"

void Cube::_rotate_slice(Vector3 axis, float angle) {
	for (auto& center : centerPieces) {
		bool inSlice = false;

		if (axis.x != 0 && approxEqual(center.position.x, 0)) inSlice = true; // M slice
		if (axis.y != 0 && approxEqual(center.position.y, 0)) inSlice = true; // E slice
		if (axis.z != 0 && approxEqual(center.position.z, 0)) inSlice = true; // S slice

		if (!inSlice) continue;

		center.position = Snap(Vector3RotateByAxisAngle(center.position, axis, angle * DEG2RAD));
		center.normal = Snap(Vector3RotateByAxisAngle(center.normal,  axis, angle * DEG2RAD));
	}
	for (auto& edge : edgePieces) {
		bool inSlice = false;

		if (axis.x != 0 && approxEqual(edge.position.x, 0)) inSlice = true; // M slice
		if (axis.y != 0 && approxEqual(edge.position.y, 0)) inSlice = true; // E slice
		if (axis.z != 0 && approxEqual(edge.position.z, 0)) inSlice = true; // S slice

		if (!inSlice) continue;

		edge.position = Snap(Vector3RotateByAxisAngle(edge.position, axis, angle * DEG2RAD));
		edge.normal1  = Snap(Vector3RotateByAxisAngle(edge.normal1,  axis, angle * DEG2RAD));
		edge.normal2  = Snap(Vector3RotateByAxisAngle(edge.normal2,  axis, angle * DEG2RAD));
	}

	for (auto& corner : cornerPieces) {
		bool inSlice = false;

		if (axis.x != 0 && approxEqual(corner.position.x, 0)) inSlice = true;
		if (axis.y != 0 && approxEqual(corner.position.y, 0)) inSlice = true;
		if (axis.z != 0 && approxEqual(corner.position.z, 0)) inSlice = true;

		if (!inSlice) continue;

		corner.position = Snap(Vector3RotateByAxisAngle(corner.position, axis, angle * DEG2RAD));
		corner.normal1  = Snap(Vector3RotateByAxisAngle(corner.normal1,  axis, angle * DEG2RAD));
		corner.normal2  = Snap(Vector3RotateByAxisAngle(corner.normal2,  axis, angle * DEG2RAD));
		corner.normal3  = Snap(Vector3RotateByAxisAngle(corner.normal3,  axis, angle * DEG2RAD));
	}
}

void Cube::_rotate_face(Vector3 face, float angle) {
	for(auto& edge : cubeEdgeSides[face]){
		edge->position = Snap(Vector3RotateByAxisAngle(edge->position, face, angle * DEG2RAD));
		edge->normal1 = Snap(Vector3RotateByAxisAngle(edge->normal1, face, angle * DEG2RAD));
		edge->normal2 = Snap(Vector3RotateByAxisAngle(edge->normal2, face, angle * DEG2RAD));
	}
	for(auto& corner : cubeCornerSides[face]){
		corner->position = Snap(Vector3RotateByAxisAngle(corner->position, face, angle * DEG2RAD));
		corner->normal1 = Snap(Vector3RotateByAxisAngle(corner->normal1, face, angle * DEG2RAD));
		corner->normal2 = Snap(Vector3RotateByAxisAngle(corner->normal2, face, angle * DEG2RAD));
		corner->normal3 = Snap(Vector3RotateByAxisAngle(corner->normal3, face, angle * DEG2RAD));
	}
}

void Cube::_rotate_wide(Vector3 face, float angle){
	_rotate_slice(face, angle);
	_rotate_face(face, angle);
}

void Cube::_rotate_cube(Vector3 direction, float angle) {
	for (auto& center : centerPieces) {
		center.position = Snap(Vector3RotateByAxisAngle(center.position, direction, angle * DEG2RAD));
		center.normal   = Snap(Vector3RotateByAxisAngle(center.normal,   direction, angle * DEG2RAD));
	}

	for (auto& edge : edgePieces) {
		edge.position = Snap(Vector3RotateByAxisAngle(edge.position, direction, angle * DEG2RAD));
		edge.normal1  = Snap(Vector3RotateByAxisAngle(edge.normal1,  direction, angle * DEG2RAD));
		edge.normal2  = Snap(Vector3RotateByAxisAngle(edge.normal2,  direction, angle * DEG2RAD));
	}

	for (auto& corner : cornerPieces) {
		corner.position = Snap(Vector3RotateByAxisAngle(corner.position, direction, angle * DEG2RAD));
		corner.normal1  = Snap(Vector3RotateByAxisAngle(corner.normal1,  direction, angle * DEG2RAD));
		corner.normal2  = Snap(Vector3RotateByAxisAngle(corner.normal2,  direction, angle * DEG2RAD));
		corner.normal3  = Snap(Vector3RotateByAxisAngle(corner.normal3,  direction, angle * DEG2RAD));
	}
}
