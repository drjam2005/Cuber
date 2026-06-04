#include "cube.h"

#include "raymath.h"
#include "helpers.h"
#include "defines.h"

#include <iostream>

void Cube::Render() {
	DrawCubeV({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, LIGHTGRAY);
	for(auto& edge : edgePieces){
		_render_edge(edge);
	}
	for(auto& corner : cornerPieces){
		_render_corner(corner);
	}
	for(auto& center : centerPieces){
		_render_center(center);
	}
}
void Cube::Update() {

	_update_data();
	if(_handle_key_input()){
		if(IsSolved())
			std::cout << "Solved!" << '\n';
	}

	uint64_t currentSecond = (uint64_t)GetTime();
	if(currentSecond > lastSecond){
		lastSecond = currentSecond;
	}



}
bool Cube::_handle_key_input() {
	std::vector<Move> moves;

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
		moves.push_back({FACE, _BOTTOM_FACE, _C_WISE});
	if (IsKeyPressed(KEY_S))
		moves.push_back({FACE, _BOTTOM_FACE, _CC_WISE});

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

	return true;
}

void Cube::_update_data(){
	cubeEdgeSides.clear();
	cubeCornerSides.clear();
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
}

void Cube::_render_edge(Edge& edge) {
	_draw_sticker(edge.position, edge.normal1, edge.clrs.clr1);
	_draw_sticker(edge.position, edge.normal2, edge.clrs.clr2);
}

void Cube::_render_corner(Corner& corner) {
	_draw_sticker(corner.position, corner.normal1, corner.clrs.clr1);
	_draw_sticker(corner.position, corner.normal2, corner.clrs.clr2);
	_draw_sticker(corner.position, corner.normal3, corner.clrs.clr3);
}

void Cube::_render_center(Center& center){
	_draw_sticker(center.position, center.normal, center.clr);
}

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
		edge->position = Vector3RotateByAxisAngle(edge->position, face, angle * DEG2RAD);
		edge->normal1 = Vector3RotateByAxisAngle(edge->normal1, face, angle * DEG2RAD);
		edge->normal2 = Vector3RotateByAxisAngle(edge->normal2, face, angle * DEG2RAD);
	}
	for(auto& corner : cubeCornerSides[face]){
		corner->position = Vector3RotateByAxisAngle(corner->position, face, angle * DEG2RAD);
		corner->normal1 = Vector3RotateByAxisAngle(corner->normal1, face, angle * DEG2RAD);
		corner->normal2 = Vector3RotateByAxisAngle(corner->normal2, face, angle * DEG2RAD);
		corner->normal3 = Vector3RotateByAxisAngle(corner->normal3, face, angle * DEG2RAD);
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
void Cube::_collect_move_pieces(
	Move move,
	std::set<Edge*>& outEdges,
	std::set<Corner*>& outCorners,
	std::set<Center*>& outCenters
) {
	switch (move.moveType) {
		case FACE: {
			for (Edge* edge : cubeEdgeSides[move.axis])
				outEdges.insert(edge);

			for (Corner* corner : cubeCornerSides[move.axis])
				outCorners.insert(corner);

			for (Center* center : cubeCenterSides[move.axis])
				outCenters.insert(center);

			break;
		}

		case SLICE: {
			for (auto& edge : edgePieces) {
				if (move.axis.x != 0 && approxEqual(edge.position.x, 0)) outEdges.insert(&edge);
				if (move.axis.y != 0 && approxEqual(edge.position.y, 0)) outEdges.insert(&edge);
				if (move.axis.z != 0 && approxEqual(edge.position.z, 0)) outEdges.insert(&edge);
			}

			for (auto& corner : cornerPieces) {
				if (move.axis.x != 0 && approxEqual(corner.position.x, 0)) outCorners.insert(&corner);
				if (move.axis.y != 0 && approxEqual(corner.position.y, 0)) outCorners.insert(&corner);
				if (move.axis.z != 0 && approxEqual(corner.position.z, 0)) outCorners.insert(&corner);
			}

			for (auto& center : centerPieces) {
				if (move.axis.x != 0 && approxEqual(center.position.x, 0)) outCenters.insert(&center);
				if (move.axis.y != 0 && approxEqual(center.position.y, 0)) outCenters.insert(&center);
				if (move.axis.z != 0 && approxEqual(center.position.z, 0)) outCenters.insert(&center);
			}

			break;
		}

		case WIDE: {
			// wide = face + matching middle slice
			Move faceMove = {FACE, move.axis, move.angle};
			Move sliceMove = {SLICE, move.axis, move.angle};

			_collect_move_pieces(faceMove, outEdges, outCorners, outCenters);
			_collect_move_pieces(sliceMove, outEdges, outCorners, outCenters);

			break;
		}

		case WHOLE: {
			for (auto& edge : edgePieces)
				outEdges.insert(&edge);

			for (auto& corner : cornerPieces)
				outCorners.insert(&corner);

			for (auto& center : centerPieces)
				outCenters.insert(&center);

			break;
		}
	}
}

void Cube::_draw_sticker(Vector3 position, Vector3 normal, Color clr) {
	Vector3 center = Vector3Add(position, Vector3Scale(normal, 0.51f));

	rlPushMatrix();
		rlTranslatef(center.x, center.y, center.z);

		if (Vector3Equals(normal, _BOTTOM_FACE))
			rlRotatef(180, 1, 0, 0);
		else if (Vector3Equals(normal, _FRONT_FACE))
			rlRotatef(90, 1, 0, 0);
		else if (Vector3Equals(normal, _BACK_FACE))
			rlRotatef(-90, 1, 0, 0);
		else if (Vector3Equals(normal, _RIGHT_FACE))
			rlRotatef(90, 0, 0, 1);
		else if (Vector3Equals(normal, _LEFT_FACE))
			rlRotatef(-90, 0, 0, 1);

		DrawPlane({0, 0, 0}, {0.8f, 0.8f}, clr);
	rlPopMatrix();
}

bool Cube::IsSolved() {
    std::vector<Vector3> axes = {
        _TOP_FACE,
        _RIGHT_FACE,
        _FRONT_FACE
    };

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                std::vector<Edge> testEdges = edgePieces;
                std::vector<Corner> testCorners = cornerPieces;
                std::vector<Center> testCenters = centerPieces;

                for (int i = 0; i < x; i++) {
                    for (auto& e : testEdges) e = RotatedEdge(e, _RIGHT_FACE, 90);
                    for (auto& c : testCorners) c = RotatedCorner(c, _RIGHT_FACE, 90);
                    for (auto& ce : testCenters) ce = RotatedCenter(ce, _RIGHT_FACE, 90);
                }

                for (int i = 0; i < y; i++) {
                    for (auto& e : testEdges) e = RotatedEdge(e, _TOP_FACE, 90);
                    for (auto& c : testCorners) c = RotatedCorner(c, _TOP_FACE, 90);
                    for (auto& ce : testCenters) ce = RotatedCenter(ce, _TOP_FACE, 90);
                }

                for (int i = 0; i < z; i++) {
                    for (auto& e : testEdges) e = RotatedEdge(e, _FRONT_FACE, 90);
                    for (auto& c : testCorners) c = RotatedCorner(c, _FRONT_FACE, 90);
                    for (auto& ce : testCenters) ce = RotatedCenter(ce, _FRONT_FACE, 90);
                }

                bool solved = true;

                for (int i = 0; i < edgePieces.size(); i++) {
                    if (!EdgeMatches(testEdges[i], standardEdgePieces[i])) {
                        solved = false;
                        break;
                    }
                }

                if (!solved) continue;

                for (int i = 0; i < cornerPieces.size(); i++) {
                    if (!CornerMatches(testCorners[i], standardCornerPieces[i])) {
                        solved = false;
                        break;
                    }
                }

                if (!solved) continue;

                for (int i = 0; i < centerPieces.size(); i++) {
                    if (!CenterMatches(testCenters[i], standardCenterPieces[i])) {
                        solved = false;
                        break;
                    }
                }

                if (solved)
                    return true;
            }
        }
    }

    return false;
}
