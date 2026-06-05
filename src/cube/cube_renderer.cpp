#include "cube.h"
#include "rlgl.h"
#include "raymath.h"
#include "cube_defines.h"
#include <algorithm>

static Vector3 GetStickerTangent(Vector3 normal) {
	normal = Vector3Normalize(normal);

	if (fabsf(normal.x) > 0.5f)
		return _TOP_FACE;
	if (fabsf(normal.y) > 0.5f)
		return _RIGHT_FACE;
	return _TOP_FACE;
}

void Cube::_render_scene(){
	for(auto& edge : edgePieces){
		if (_is_edge_hidden(edge))
			continue;
		_render_edge(edge);
	}
	for(auto& corner : cornerPieces){
		if (_is_corner_hidden(corner))
			continue;
		_render_corner(corner);
	}
	for(auto& center : centerPieces){
		if (_is_center_hidden(center))
			continue;
		_render_center(center);
	}

	for (const MoveAnimation& animation : moveAnimations) {
		float progress = animation.elapsed / animation.duration;
		progress = std::clamp(progress, 0.0f, 1.0f);
		progress = 1.0f - (1.0f - progress) * (1.0f - progress);
		float angle = animation.move.angle * progress;

		for (const Edge& edge : animation.edges)
			_render_edge_animated(edge, animation.move.axis, angle);

		for (const Corner& corner : animation.corners)
			_render_corner_animated(corner, animation.move.axis, angle);

		for (const Center& center : animation.centers)
			_render_center_animated(center, animation.move.axis, angle);
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

void Cube::_render_edge_animated(const Edge& edge, Vector3 axis, float angle) {
	float angleRadians = angle * DEG2RAD;
	Vector3 rotatedPosition = Vector3RotateByAxisAngle(edge.position, axis, angleRadians);

	_draw_sticker(
		rotatedPosition,
		Vector3RotateByAxisAngle(edge.normal1, axis, angleRadians),
		edge.clrs.clr1,
		Vector3RotateByAxisAngle(GetStickerTangent(edge.normal1), axis, angleRadians)
	);
	_draw_sticker(
		rotatedPosition,
		Vector3RotateByAxisAngle(edge.normal2, axis, angleRadians),
		edge.clrs.clr2,
		Vector3RotateByAxisAngle(GetStickerTangent(edge.normal2), axis, angleRadians)
	);
}

void Cube::_render_corner_animated(const Corner& corner, Vector3 axis, float angle) {
	float angleRadians = angle * DEG2RAD;
	Vector3 rotatedPosition = Vector3RotateByAxisAngle(corner.position, axis, angleRadians);

	_draw_sticker(
		rotatedPosition,
		Vector3RotateByAxisAngle(corner.normal1, axis, angleRadians),
		corner.clrs.clr1,
		Vector3RotateByAxisAngle(GetStickerTangent(corner.normal1), axis, angleRadians)
	);
	_draw_sticker(
		rotatedPosition,
		Vector3RotateByAxisAngle(corner.normal2, axis, angleRadians),
		corner.clrs.clr2,
		Vector3RotateByAxisAngle(GetStickerTangent(corner.normal2), axis, angleRadians)
	);
	_draw_sticker(
		rotatedPosition,
		Vector3RotateByAxisAngle(corner.normal3, axis, angleRadians),
		corner.clrs.clr3,
		Vector3RotateByAxisAngle(GetStickerTangent(corner.normal3), axis, angleRadians)
	);
}

void Cube::_render_center_animated(const Center& center, Vector3 axis, float angle) {
	float angleRadians = angle * DEG2RAD;

	_draw_sticker(
		Vector3RotateByAxisAngle(center.position, axis, angleRadians),
		Vector3RotateByAxisAngle(center.normal, axis, angleRadians),
		center.clr,
		Vector3RotateByAxisAngle(GetStickerTangent(center.normal), axis, angleRadians)
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


void Cube::_draw_sticker(Vector3 position, Vector3 normal, Color clr, Vector3 tangent) {
	normal = Vector3Normalize(normal);
	Vector3 center = Vector3Add(position, Vector3Scale(normal, 0.51f));

	if (Vector3Length(tangent) < 0.0001f)
		tangent = GetStickerTangent(normal);

	tangent = Vector3Normalize(tangent);
	Vector3 bitangent = Vector3Normalize(Vector3CrossProduct(tangent, normal));

	const float halfSize = 0.4f;
	tangent = Vector3Scale(tangent, halfSize);
	bitangent = Vector3Scale(bitangent, halfSize);

	Vector3 v1 = Vector3Subtract(Vector3Subtract(center, tangent), bitangent);
	Vector3 v2 = Vector3Add(Vector3Subtract(center, tangent), bitangent);
	Vector3 v3 = Vector3Add(Vector3Add(center, tangent), bitangent);
	Vector3 v4 = Vector3Subtract(Vector3Add(center, tangent), bitangent);

	rlBegin(RL_QUADS);
		rlColor4ub(clr.r, clr.g, clr.b, clr.a);
		rlNormal3f(normal.x, normal.y, normal.z);
		rlVertex3f(v1.x, v1.y, v1.z);
		rlVertex3f(v2.x, v2.y, v2.z);
		rlVertex3f(v3.x, v3.y, v3.z);
		rlVertex3f(v4.x, v4.y, v4.z);
	rlEnd();
}
