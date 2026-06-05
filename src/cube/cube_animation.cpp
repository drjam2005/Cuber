#include "cube.h"
#include "raylib.h"
#include <algorithm>

namespace {
template <typename Piece>
bool HasOverlap(const std::set<Piece*>& pieces, const std::set<Piece*>& hiddenPieces) {
	for (Piece* piece : pieces) {
		if (hiddenPieces.find(piece) != hiddenPieces.end())
			return true;
	}

	return false;
}

template <typename Piece>
void CopyPieces(const std::set<Piece*>& pieces, std::vector<Piece>& destination) {
	for (Piece* piece : pieces)
		destination.push_back(*piece);
}
}

void Cube::_start_move_animation(const Move& move, const std::set<Edge*>& edges, const std::set<Corner*>& corners, const std::set<Center*>& centers) {
	moveAnimations.erase(
		std::remove_if(
			moveAnimations.begin(),
			moveAnimations.end(),
			[&](const MoveAnimation& animation) {
				return HasOverlap(edges, animation.hiddenEdges)
					|| HasOverlap(corners, animation.hiddenCorners)
					|| HasOverlap(centers, animation.hiddenCenters);
			}
		),
		moveAnimations.end()
	);

	MoveAnimation animation;
	animation.move = move;
	animation.hiddenEdges = edges;
	animation.hiddenCorners = corners;
	animation.hiddenCenters = centers;

	CopyPieces(edges, animation.edges);
	CopyPieces(corners, animation.corners);
	CopyPieces(centers, animation.centers);

	moveAnimations.push_back(animation);
}

void Cube::_update_animations() {
	const float deltaTime = GetFrameTime();

	for (MoveAnimation& animation : moveAnimations)
		animation.elapsed += deltaTime;

	moveAnimations.erase(
		std::remove_if(
			moveAnimations.begin(),
			moveAnimations.end(),
			[](const MoveAnimation& animation) {
				return animation.elapsed >= animation.duration;
			}
		),
		moveAnimations.end()
	);
}

bool Cube::_is_edge_hidden(Edge& edge) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenEdges.find(&edge) != animation.hiddenEdges.end())
			return true;
	}

	return false;
}

bool Cube::_is_corner_hidden(Corner& corner) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenCorners.find(&corner) != animation.hiddenCorners.end())
			return true;
	}

	return false;
}

bool Cube::_is_center_hidden(Center& center) const {
	for (const MoveAnimation& animation : moveAnimations) {
		if (animation.hiddenCenters.find(&center) != animation.hiddenCenters.end())
			return true;
	}

	return false;
}
