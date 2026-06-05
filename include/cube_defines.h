#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#define _SPLIT_V3(v) v.x, v.y, v.z

#define _TOP_FACE ((Vector3){0.0f, 1.0f, 0.0f})
#define _BOTTOM_FACE ((Vector3){0.0f, -1.0f, 0.0f})
#define _LEFT_FACE ((Vector3){-1.0f, 0.0f, 0.0f})
#define _RIGHT_FACE ((Vector3){1.0f, 0.0f, 0.0f})
#define _FRONT_FACE ((Vector3){0.0f, 0.0f, 1.0f})
#define _BACK_FACE ((Vector3){0.0f, 0.0f, -1.0f})

#define _M_SLICE _LEFT_FACE
#define _E_SLICE _BOTTOM_FACE
#define _S_FACE _FRONT_FACE

#define _C_WISE (-90.0f)
#define _CC_WISE (90.0f)

#endif // DEFINES_H
