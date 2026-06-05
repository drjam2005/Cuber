#include "cube.h"
#include "cube_defines.h"
#include "helpers.h"
#include "pieces.h"
#include "raylib.h"
#include "raymath.h"

void Cube::_update_data(){
	cubeEdgeSides.clear();
	cubeCornerSides.clear();
	cubeCenterSides.clear();

	for(auto& edge : edgePieces){
		Vector3& pos = edge.position;

		if(Vector3Equals(_RIGHT_FACE, Vector3AND(pos, _RIGHT_FACE)))
			cubeEdgeSides[_RIGHT_FACE].insert(&edge);
		if(Vector3Equals(_LEFT_FACE, Vector3AND(pos, _LEFT_FACE)))
			cubeEdgeSides[_LEFT_FACE].insert(&edge);
		if(Vector3Equals(_FRONT_FACE, Vector3AND(pos, _FRONT_FACE)))
			cubeEdgeSides[_FRONT_FACE].insert(&edge);
		if(Vector3Equals(_BACK_FACE, Vector3AND(pos, _BACK_FACE)))
			cubeEdgeSides[_BACK_FACE].insert(&edge);
		if(Vector3Equals(_TOP_FACE, Vector3AND(pos, _TOP_FACE)))
			cubeEdgeSides[_TOP_FACE].insert(&edge);
		if(Vector3Equals(_BOTTOM_FACE, Vector3AND(pos, _BOTTOM_FACE)))
			cubeEdgeSides[_BOTTOM_FACE].insert(&edge);
		
	}
	for(auto& corner : cornerPieces){
		Vector3& pos = corner.position;

		if(Vector3Equals(_RIGHT_FACE, Vector3AND(pos, _RIGHT_FACE)))
			cubeCornerSides[_RIGHT_FACE].insert(&corner);
		if(Vector3Equals(_LEFT_FACE, Vector3AND(pos, _LEFT_FACE)))
			cubeCornerSides[_LEFT_FACE].insert(&corner);
		if(Vector3Equals(_FRONT_FACE, Vector3AND(pos, _FRONT_FACE)))
			cubeCornerSides[_FRONT_FACE].insert(&corner);
		if(Vector3Equals(_BACK_FACE, Vector3AND(pos, _BACK_FACE)))
			cubeCornerSides[_BACK_FACE].insert(&corner);
		if(Vector3Equals(_TOP_FACE, Vector3AND(pos, _TOP_FACE)))
			cubeCornerSides[_TOP_FACE].insert(&corner);
		if(Vector3Equals(_BOTTOM_FACE, Vector3AND(pos, _BOTTOM_FACE)))
			cubeCornerSides[_BOTTOM_FACE].insert(&corner);
	}

	for(auto& center : centerPieces){
		Vector3& pos = center.position;

		if(Vector3Equals(_RIGHT_FACE, Vector3AND(pos, _RIGHT_FACE)))
			cubeCenterSides[_RIGHT_FACE].insert(&center);
		if(Vector3Equals(_LEFT_FACE, Vector3AND(pos, _LEFT_FACE)))
			cubeCenterSides[_LEFT_FACE].insert(&center);
		if(Vector3Equals(_FRONT_FACE, Vector3AND(pos, _FRONT_FACE)))
			cubeCenterSides[_FRONT_FACE].insert(&center);
		if(Vector3Equals(_BACK_FACE, Vector3AND(pos, _BACK_FACE)))
			cubeCenterSides[_BACK_FACE].insert(&center);
		if(Vector3Equals(_TOP_FACE, Vector3AND(pos, _TOP_FACE)))
			cubeCenterSides[_TOP_FACE].insert(&center);
		if(Vector3Equals(_BOTTOM_FACE, Vector3AND(pos, _BOTTOM_FACE)))
			cubeCenterSides[_BOTTOM_FACE].insert(&center);
	}
}

void Cube::_apply_move(const Move& move) {
	switch (move.moveType) {
		case FACE:
			_rotate_face(move.axis, move.angle);
			break;
		case SLICE:
			_rotate_slice(move.axis, move.angle);
			break;
		case WIDE:
			_rotate_wide(move.axis, move.angle);
			break;
		case WHOLE:
			_rotate_cube(move.axis, move.angle);
			break;
	}
}

bool Cube::_handle_key_input() {
	moves.clear();
	isMoving = false;
	if(IsKeyPressed(KEY_Q)){
		if(IsKeyDown(KEY_LEFT_CONTROL)) {
			closeRequested = true;
			return false;
		}
	}

	if(IsKeyPressed(KEY_ESCAPE)){
		moveAnimations.clear();
		edgePieces = standardEdgePieces;
		cornerPieces = standardCornerPieces;
		centerPieces = standardCenterPieces;
	}

	if(IsKeyPressed(KEY_SPACE)) {
		if(!IsSolved())
			return true;

		moveAnimations.clear();
		edgePieces = standardEdgePieces;
		cornerPieces = standardCornerPieces;
		centerPieces = standardCenterPieces;

		_update_data();
		_generate_scramble();

		for (const Move& move : moves) {
			_apply_move(move);
			_update_data();
		}

		return true;
	}

	if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_SIX))
		moves.push_back({SLICE, _LEFT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_PERIOD))
		moves.push_back({SLICE, _LEFT_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_U))
		moves.push_back({WIDE, _RIGHT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_M))
		moves.push_back({WIDE, _RIGHT_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_V))
		moves.push_back({WIDE, _LEFT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_R))
		moves.push_back({WIDE, _LEFT_FACE, _CC_WISE});

	// rotate upwards / downwards
	if(IsKeyPressed(KEY_T) || IsKeyPressed(KEY_Y))
		moves.push_back({WHOLE, _RIGHT_FACE, _C_WISE});
	if(IsKeyPressed(KEY_B) || IsKeyPressed(KEY_N))
		moves.push_back({WHOLE, _RIGHT_FACE, _CC_WISE});

	// rotate whole clockwise/counter-clockwise from front
	if(IsKeyPressed(KEY_P))
		moves.push_back({WHOLE, _FRONT_FACE, _C_WISE});
	if(IsKeyPressed(KEY_Q))
		moves.push_back({WHOLE, _FRONT_FACE, _CC_WISE});

	// rotate whole clockwise/counter-clockwise from top
	if(IsKeyPressed(KEY_SEMICOLON))
		moves.push_back({WHOLE, _TOP_FACE, _C_WISE});
	if(IsKeyPressed(KEY_A))
		moves.push_back({WHOLE, _TOP_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_H))
		moves.push_back({FACE, _FRONT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_G))
		moves.push_back({FACE, _FRONT_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_J))
		moves.push_back({FACE, _TOP_FACE, _C_WISE});
	if (IsKeyPressed(KEY_F))
		moves.push_back({FACE, _TOP_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_L))
		moves.push_back({FACE, _BOTTOM_FACE, _CC_WISE});
	if (IsKeyPressed(KEY_S))
		moves.push_back({FACE, _BOTTOM_FACE, _C_WISE});

	if (IsKeyPressed(KEY_D))
		moves.push_back({FACE, _LEFT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_E))
		moves.push_back({FACE, _LEFT_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_I))
		moves.push_back({FACE, _RIGHT_FACE, _C_WISE});
	if (IsKeyPressed(KEY_K))
		moves.push_back({FACE, _RIGHT_FACE, _CC_WISE});

	if (IsKeyPressed(KEY_W))
		moves.push_back({FACE, _BACK_FACE, _C_WISE});
	if (IsKeyPressed(KEY_O))
		moves.push_back({FACE, _BACK_FACE, _CC_WISE});

	std::set<Edge*> usedEdges;
	std::set<Corner*> usedCorners;
	std::set<Center*> usedCenters;

	if(moves.empty())
		return false;

	bool isJustWholes = true;
	for(Move& move : moves){
		if(move.moveType != WHOLE)
			isJustWholes = false;
	}


	for (Move& move : moves) {
		std::set<Edge*> moveEdges;
		std::set<Corner*> moveCorners;
		std::set<Center*> moveCenters;

		_collect_move_pieces(move, moveEdges, moveCorners, moveCenters);

		bool overlaps = false;

		for (Edge* edge : moveEdges) {
			if (usedEdges.find(edge) != usedEdges.end()) {
				overlaps = true;
				break;
			}
		}

		for (Corner* corner : moveCorners) {
			if (usedCorners.find(corner) != usedCorners.end()) {
				overlaps = true;
				break;
			}
		}

		for (Center* center : moveCenters) {
			if (usedCenters.find(center) != usedCenters.end()) {
				overlaps = true;
				break;
			}
		}

		if (overlaps)
			continue;

		usedEdges.insert(moveEdges.begin(), moveEdges.end());
		usedCorners.insert(moveCorners.begin(), moveCorners.end());
		usedCenters.insert(moveCenters.begin(), moveCenters.end());

		_start_move_animation(move, moveEdges, moveCorners, moveCenters);
		_apply_move(move);
	}

	if(isJustWholes)
		return false;

	isMoving = true;
	return true;
}
