#pragma once
#ifndef HELPERS_H
#define HELPERS_H

#include "raylib.h"
#include "pieces.h"
#include <cmath>

bool approxEqual(float a, float b, float epsilon=0.1f);
Vector3 Vector3AND(Vector3 vec1, Vector3 vec2);
Vector3 Snap(Vector3 v);

bool SameVec(Vector3 a, Vector3 b);
bool EdgeMatches(const Edge& a, const Edge& b);
bool CornerMatches(const Corner& a, const Corner& b);
bool CenterMatches(const Center& a, const Center& b);

Edge RotatedEdge(Edge e, Vector3 axis, float angle);
Corner RotatedCorner(Corner c, Vector3 axis, float angle);
Center RotatedCenter(Center c, Vector3 axis, float angle);

float getMinutes(float elapsedTime);
float getSeconds(float elapsedTime);

// fetched from raylib examples lol
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

// Draw text using font inside rectangle limits with support for text selection


#endif // HELPERS_H
