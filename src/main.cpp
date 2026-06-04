#include "raylib.h"
#include "set"
#include "rlgl.h"
#include "raymath.h"

#include <unordered_map>
#include <cmath>
#include <cstdio>
#include <vector>
#include <cstdint>

#define _SPLIT_V3(v) v.x, v.y, v.z

#define _TOP_FACE ((Vector3){0.0f, 1.0f, 0.0f})
#define _BOTTOM_FACE ((Vector3){0.0f, -1.0f, 0.0f})
#define _LEFT_FACE ((Vector3){-1.0f, 0.0f, 0.0f})
#define _RIGHT_FACE ((Vector3){1.0f, 0.0f, 0.0f})
#define _FRONT_FACE ((Vector3){0.0f, 0.0f, 1.0f})
#define _BACK_FACE ((Vector3){0.0f, 0.0f, -1.0f})

#define _M_SLICE _LEFT_FACE
#define _E_SLICE _BOTTOM_FACE
#define _S_FACE _FRONT_FACE

#define _C_WISE (-90.0f)
#define _CC_WISE (90.0f)

bool approxEqual(float a, float b, float epsilon=0.1f){
	return fabs(a - b) < epsilon;
}

Vector3 Vector3AND(Vector3 vec1, Vector3 vec2){
	Vector3 result;

	result.x = (approxEqual(vec1.x, vec2.x)) ? vec2.x : 0.0f;
	result.y = (approxEqual(vec1.y, vec2.y)) ? vec2.y : 0.0f;
	result.z = (approxEqual(vec1.z, vec2.z)) ? vec2.z : 0.0f;

	return result;
}

Vector3 Snap(Vector3 v) {
    return {
        roundf(v.x),
        roundf(v.y),
        roundf(v.z)
    };
}

struct CornerColors {
	Color clr1;
	Color clr2;
	Color clr3;
};

struct EdgeColors {
	Color clr1;
	Color clr2;
};

enum TYPE {
	CORNER,
	EDGE,
	CENTER
};

struct Corner {
	Vector3 position;

	Vector3 normal1;
	Vector3 normal2;
	Vector3 normal3;

	CornerColors clrs;
};

struct Edge {
	Vector3 position;

	Vector3 normal1;
	Vector3 normal2;

	EdgeColors clrs;
};

struct Center {
	Vector3 position;
	Vector3 normal;
	Color clr;

};

struct Vector3Hash {
    std::size_t operator()(const Vector3& v) const {
        std::size_t hx = std::hash<float>{}(v.x);
        std::size_t hy = std::hash<float>{}(v.y);
        std::size_t hz = std::hash<float>{}(v.z);

        return hx ^ (hy << 1) ^ (hz << 2);
    }
};

struct Vector3Equal {
    bool operator()(const Vector3& a, const Vector3& b) const {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

enum MOVE_TYPE {
	FACE,
	SLICE,
	WIDE,
	WHOLE
};

struct Move {
	MOVE_TYPE moveType;
    Vector3 axis;
    float angle;
};

// struct Center {
// 	Vector3 position;
// 	Vector3 normal;
// 	Color clr;
// 
// };
std::vector<Center> standardCenterPieces = {
    { { 0,  1,  0}, _TOP_FACE,    WHITE  }, // U
    { { 0, -1,  0}, _BOTTOM_FACE, YELLOW }, // D

    { {-1,  0,  0}, _LEFT_FACE,   ORANGE }, // L
    { { 1,  0,  0}, _RIGHT_FACE,  RED    }, // R

    { { 0,  0,  1}, _FRONT_FACE,  GREEN  }, // F
    { { 0,  0, -1}, _BACK_FACE,   BLUE   }  // B
};

std::vector<Edge> standardEdgePieces = {
    // Up layer
    {{ 0,  1,  1}, _TOP_FACE,    _FRONT_FACE,  {WHITE, GREEN}},  // UF
    {{ 1,  1,  0}, _TOP_FACE,    _RIGHT_FACE,  {WHITE, RED}},    // UR
    {{ 0,  1, -1}, _TOP_FACE,    _BACK_FACE,   {WHITE, BLUE}},   // UB
    {{-1,  1,  0}, _TOP_FACE,    _LEFT_FACE,   {WHITE, ORANGE}}, // UL

    // Middle layer
    {{ 1,  0,  1}, _RIGHT_FACE,  _FRONT_FACE,  {RED, GREEN}},    // RF
    {{ 1,  0, -1}, _RIGHT_FACE,  _BACK_FACE,   {RED, BLUE}},     // RB
    {{-1,  0, -1}, _LEFT_FACE,   _BACK_FACE,   {ORANGE, BLUE}},  // LB
    {{-1,  0,  1}, _LEFT_FACE,   _FRONT_FACE,  {ORANGE, GREEN}}, // LF

    // Down layer
    {{ 0, -1,  1}, _BOTTOM_FACE, _FRONT_FACE,  {YELLOW, GREEN}}, // DF
    {{ 1, -1,  0}, _BOTTOM_FACE, _RIGHT_FACE,  {YELLOW, RED}},   // DR
    {{ 0, -1, -1}, _BOTTOM_FACE, _BACK_FACE,   {YELLOW, BLUE}},  // DB
    {{-1, -1,  0}, _BOTTOM_FACE, _LEFT_FACE,   {YELLOW, ORANGE}} // DL
};

std::vector<Corner> standardCornerPieces = {
    // Up layer
    {
        { 1,  1,  1},
        _TOP_FACE,
        _RIGHT_FACE,
        _FRONT_FACE,
        {WHITE, RED, GREEN}
    }, // URF

    {
        { 1,  1, -1},
        _TOP_FACE,
        _RIGHT_FACE,
        _BACK_FACE,
        {WHITE, RED, BLUE}
    }, // URB

    {
        {-1,  1, -1},
        _TOP_FACE,
        _LEFT_FACE,
        _BACK_FACE,
        {WHITE, ORANGE, BLUE}
    }, // ULB

    {
        {-1,  1,  1},
        _TOP_FACE,
        _LEFT_FACE,
        _FRONT_FACE,
        {WHITE, ORANGE, GREEN}
    }, // ULF

    // Down layer
    {
        { 1, -1,  1},
        _BOTTOM_FACE,
        _RIGHT_FACE,
        _FRONT_FACE,
        {YELLOW, RED, GREEN}
    }, // DRF

    {
        { 1, -1, -1},
        _BOTTOM_FACE,
        _RIGHT_FACE,
        _BACK_FACE,
        {YELLOW, RED, BLUE}
    }, // DRB

    {
        {-1, -1, -1},
        _BOTTOM_FACE,
        _LEFT_FACE,
        _BACK_FACE,
        {YELLOW, ORANGE, BLUE}
    }, // DLB

    {
        {-1, -1,  1},
        _BOTTOM_FACE,
        _LEFT_FACE,
        _FRONT_FACE,
        {YELLOW, ORANGE, GREEN}
    } // DLF
};

class Cube {
private:
	uint64_t lastSecond = 0.0f;
	Vector3 rotations = {0.0f, 0.0f, 0.0f};
	std::vector<Edge> edgePieces = standardEdgePieces;
	std::vector<Corner> cornerPieces = standardCornerPieces;
	std::vector<Center> centerPieces = standardCenterPieces;

	std::unordered_map<Vector3, std::set<Edge*>, Vector3Hash, Vector3Equal> cubeEdgeSides;
	std::unordered_map<Vector3, std::set<Corner*>, Vector3Hash, Vector3Equal> cubeCornerSides;
	std::unordered_map<Vector3, std::set<Center*>, Vector3Hash, Vector3Equal> cubeCenterSides;
public:
	void Render() {
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
	void Update() {

		_update_data();
		_handle_key_input();

		uint64_t currentSecond = (uint64_t)GetTime();
		if(currentSecond > lastSecond){
			lastSecond = currentSecond;
		}
	}
private:
	void _handle_key_input() {
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
	}

	void _update_data(){
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

	void _render_edge(Edge& edge) {
		_draw_sticker(edge.position, edge.normal1, edge.clrs.clr1);
		_draw_sticker(edge.position, edge.normal2, edge.clrs.clr2);
	}
	void _render_corner(Corner& corner) {
		_draw_sticker(corner.position, corner.normal1, corner.clrs.clr1);
		_draw_sticker(corner.position, corner.normal2, corner.clrs.clr2);
		_draw_sticker(corner.position, corner.normal3, corner.clrs.clr3);
	}

	void _render_center(Center& center){
		_draw_sticker(center.position, center.normal, center.clr);
	}

	void _rotate_slice(Vector3 axis, float angle) {
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

	void _rotate_face(Vector3 face, float angle) {
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

	void _rotate_wide(Vector3 face, float angle){
		_rotate_slice(face, angle);
		_rotate_face(face, angle);
	}

	void _rotate_cube(Vector3 direction, float angle) {
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
private:
	void _collect_move_pieces(
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
	void _draw_sticker(Vector3 position, Vector3 normal, Color clr) {
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
};

int main() {
    InitWindow(800, 600, "cuber");
    SetTargetFPS(60);

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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
