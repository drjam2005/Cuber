#include "helpers.h"
#include "raymath.h"

bool approxEqual(float a, float b, float epsilon){
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

bool SameVec(Vector3 a, Vector3 b) {
    return Vector3Equals(Snap(a), Snap(b));
}

bool EdgeMatches(const Edge& a, const Edge& b) {
    return SameVec(a.position, b.position) &&
           SameVec(a.normal1, b.normal1) &&
           SameVec(a.normal2, b.normal2) &&
           ColorIsEqual(a.clrs.clr1, b.clrs.clr1) &&
           ColorIsEqual(a.clrs.clr2, b.clrs.clr2);
}

bool CornerMatches(const Corner& a, const Corner& b) {
    return SameVec(a.position, b.position) &&
           SameVec(a.normal1, b.normal1) &&
           SameVec(a.normal2, b.normal2) &&
           SameVec(a.normal3, b.normal3) &&
           ColorIsEqual(a.clrs.clr1, b.clrs.clr1) &&
           ColorIsEqual(a.clrs.clr2, b.clrs.clr2) &&
           ColorIsEqual(a.clrs.clr3, b.clrs.clr3);
}

bool CenterMatches(const Center& a, const Center& b) {
    return SameVec(a.position, b.position) &&
           SameVec(a.normal, b.normal) &&
           ColorIsEqual(a.clr, b.clr);
}

Edge RotatedEdge(Edge e, Vector3 axis, float angle) {
    e.position = Snap(Vector3RotateByAxisAngle(e.position, axis, angle * DEG2RAD));
    e.normal1  = Snap(Vector3RotateByAxisAngle(e.normal1,  axis, angle * DEG2RAD));
    e.normal2  = Snap(Vector3RotateByAxisAngle(e.normal2,  axis, angle * DEG2RAD));
    return e;
}

Corner RotatedCorner(Corner c, Vector3 axis, float angle) {
    c.position = Snap(Vector3RotateByAxisAngle(c.position, axis, angle * DEG2RAD));
    c.normal1  = Snap(Vector3RotateByAxisAngle(c.normal1,  axis, angle * DEG2RAD));
    c.normal2  = Snap(Vector3RotateByAxisAngle(c.normal2,  axis, angle * DEG2RAD));
    c.normal3  = Snap(Vector3RotateByAxisAngle(c.normal3,  axis, angle * DEG2RAD));
    return c;
}

Center RotatedCenter(Center c, Vector3 axis, float angle) {
    c.position = Snap(Vector3RotateByAxisAngle(c.position, axis, angle * DEG2RAD));
    c.normal   = Snap(Vector3RotateByAxisAngle(c.normal,   axis, angle * DEG2RAD));
    return c;
}
