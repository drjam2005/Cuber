#include "cube.h"
#include "defines.h"
#include "helpers.h"

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
