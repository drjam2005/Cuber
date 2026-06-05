#include "cube.h"
#include "raylib.h"

void Cube::_start_move_animation(const Move& move, const std::set<Edge*>& edges, const std::set<Corner*>& corners, const std::set<Center*>& centers) {
	std::vector<MoveAnimation> activeAnimations;
	activeAnimations.reserve(moveAnimations.size());

	for (const MoveAnimation& animation : moveAnimations) {
		bool overlapsMove = false;

		for (Edge* edge : edges)
			overlapsMove = overlapsMove || animation.hiddenEdges.contains(edge);

		for (Corner* corner : corners)
			overlapsMove = overlapsMove || animation.hiddenCorners.contains(corner);

		for (Center* center : centers)
			overlapsMove = overlapsMove || animation.hiddenCenters.contains(center);

		if (!overlapsMove)
			activeAnimations.push_back(animation);
	}

	moveAnimations = activeAnimations;

	MoveAnimation animation;
	animation.move = move;
	animation.hiddenEdges = edges;
	animation.hiddenCorners = corners;
	animation.hiddenCenters = centers;

	for (Edge* edge : edges)
		animation.edges.push_back(*edge);

	for (Corner* corner : corners)
		animation.corners.push_back(*corner);

	for (Center* center : centers)
		animation.centers.push_back(*center);

	moveAnimations.push_back(animation);
}

void Cube::_update_animations() {
	const float deltaTime = GetFrameTime();

	for (MoveAnimation& animation : moveAnimations)
		animation.elapsed += deltaTime;

	std::vector<MoveAnimation> activeAnimations;
	activeAnimations.reserve(moveAnimations.size());

	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.elapsed < animation.duration)
			activeAnimations.push_back(animation);
	}

	moveAnimations = activeAnimations;
}

bool Cube::_is_edge_hidden(Edge& edge) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenEdges.contains(&edge))
			return true;
	}

	return false;
}

bool Cube::_is_corner_hidden(Corner& corner) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenCorners.contains(&corner))
			return true;
	}

	return false;
}

bool Cube::_is_center_hidden(Center& center) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenCenters.contains(&center))
			return true;
	}

	return false;
}
