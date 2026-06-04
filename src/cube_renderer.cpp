#include "cube.h"
#include "rlgl.h"
#include "raymath.h"
#include "defines.h"

void Cube::_render_scene(){
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
