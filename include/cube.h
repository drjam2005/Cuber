#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <unordered_map>
#include <cmath>
#include <cstdio>
#include <vector>
#include <set>
#include <cstdint>
#include <string>

#include "raygui.h"
#include "rlgl.h"
#include "pieces.h"
#include "standard_pieces.h"

struct MoveAnimation {
	Move move;
	float elapsed = 0.0f;
	float duration = 0.10f;
	std::vector<Edge> edges;
	std::vector<Corner> corners;
	std::vector<Center> centers;
	std::set<Edge*> hiddenEdges;
	std::set<Corner*> hiddenCorners;
	std::set<Center*> hiddenCenters;
};

class Cube {
private:
	uint64_t lastSecond = 0.0f;
	Vector3 rotations = {0.0f, 0.0f, 0.0f};
	Camera3D camera;
	std::vector<Move> moves;
	std::vector<MoveAnimation> moveAnimations;
	std::string scrambleText = "";
	std::vector<Edge> edgePieces = standardEdgePieces;
	std::vector<Corner> cornerPieces = standardCornerPieces;
	std::vector<Center> centerPieces = standardCenterPieces;

	std::unordered_map<Vector3, std::set<Edge*>, Vector3Hash, Vector3Equal> cubeEdgeSides;
	std::unordered_map<Vector3, std::set<Corner*>, Vector3Hash, Vector3Equal> cubeCornerSides;
	std::unordered_map<Vector3, std::set<Center*>, Vector3Hash, Vector3Equal> cubeCenterSides;
	bool closeRequested = false;
	bool isMoving = false;
public:
	Cube();
	void Render();
	void Update();
	bool IsMoving();
	bool IsEnded();
	bool IsSolved();
	std::string GetScramble() const;
private:
	std::string _generate_scramble(int length=25);
	void _render_scene();

	bool _handle_key_input();
	void _update_data();
	void _apply_move(const Move& move);
	void _start_move_animation(const Move& move, const std::set<Edge*>& edges, const std::set<Corner*>& corners, const std::set<Center*>& centers);
	void _update_animations();
	bool _is_edge_hidden(Edge& edge) const;
	bool _is_corner_hidden(Corner& corner) const;
	bool _is_center_hidden(Center& center) const;

	void _render_edge(Edge& edge);
	void _render_corner(Corner& corner);

	void _render_center(Center& center);
	void _draw_rotated_sticker(Vector3 position, Vector3 normal, Color clr, Vector3 axis, float angleRadians);
	void _render_edge_animated(const Edge& edge, Vector3 axis, float angle);
	void _render_corner_animated(const Corner& corner, Vector3 axis, float angle);
	void _render_center_animated(const Center& center, Vector3 axis, float angle);

	void _rotate_slice(Vector3 axis, float angle);
	void _rotate_face(Vector3 face, float angle);

	void _rotate_wide(Vector3 face, float angle);

	void _rotate_cube(Vector3 direction, float angle);

private:
	void _collect_move_pieces(Move move, std::set<Edge*>& outEdges, std::set<Corner*>& outCorners, std::set<Center*>& outCenters);
	void _draw_sticker(Vector3 position, Vector3 normal, Color clr, Vector3 tangent = {0.0f, 0.0f, 0.0f});
};

#endif // CUBE_H
