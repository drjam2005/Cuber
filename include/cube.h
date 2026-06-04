#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <unordered_map>
#include <cmath>
#include <cstdio>
#include <vector>
#include <set>
#include <cstdint>

#include "raygui.h"
#include "rlgl.h"
#include "pieces.h"
#include "standard_pieces.h"

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
	void Render();
	void Update();
	bool IsSolved();
private:
	bool _handle_key_input();
	void _update_data();

	void _render_edge(Edge& edge);
	void _render_corner(Corner& corner);

	void _render_center(Center& center);

	void _rotate_slice(Vector3 axis, float angle);
	void _rotate_face(Vector3 face, float angle);

	void _rotate_wide(Vector3 face, float angle);

	void _rotate_cube(Vector3 direction, float angle);

private:
	void _collect_move_pieces(Move move, std::set<Edge*>& outEdges, std::set<Corner*>& outCorners, std::set<Center*>& outCenters);
	void _draw_sticker(Vector3 position, Vector3 normal, Color clr);
};

#endif // CUBE_H
