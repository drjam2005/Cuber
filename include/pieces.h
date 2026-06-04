#pragma once
#ifndef PIECES_H
#define PIECES_H

#include "raylib.h"

#include <cstddef>
#include <functional>

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
#endif // PIECES_H
