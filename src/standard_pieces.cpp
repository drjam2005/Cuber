#include "standard_pieces.h"

#include "cube_defines.h"

std::vector<Center> standardCenterPieces = {
    { { 0,  1,  0}, _TOP_FACE,    WHITE  }, // U
    { { 0, -1,  0}, _BOTTOM_FACE, YELLOW }, // D

    { {-1,  0,  0}, _LEFT_FACE,   ORANGE }, // L
    { { 1,  0,  0}, _RIGHT_FACE,  RED    }, // R

    { { 0,  0,  1}, _FRONT_FACE,  GREEN  }, // F
    { { 0,  0, -1}, _BACK_FACE,   BLUE   }  // B
};

std::vector<Edge> standardEdgePieces = {
    // Up layer
    {{ 0,  1,  1}, _TOP_FACE,    _FRONT_FACE,  {WHITE, GREEN}},  // UF
    {{ 1,  1,  0}, _TOP_FACE,    _RIGHT_FACE,  {WHITE, RED}},    // UR
    {{ 0,  1, -1}, _TOP_FACE,    _BACK_FACE,   {WHITE, BLUE}},   // UB
    {{-1,  1,  0}, _TOP_FACE,    _LEFT_FACE,   {WHITE, ORANGE}}, // UL

    // Middle layer
    {{ 1,  0,  1}, _RIGHT_FACE,  _FRONT_FACE,  {RED, GREEN}},    // RF
    {{ 1,  0, -1}, _RIGHT_FACE,  _BACK_FACE,   {RED, BLUE}},     // RB
    {{-1,  0, -1}, _LEFT_FACE,   _BACK_FACE,   {ORANGE, BLUE}},  // LB
    {{-1,  0,  1}, _LEFT_FACE,   _FRONT_FACE,  {ORANGE, GREEN}}, // LF

    // Down layer
    {{ 0, -1,  1}, _BOTTOM_FACE, _FRONT_FACE,  {YELLOW, GREEN}}, // DF
    {{ 1, -1,  0}, _BOTTOM_FACE, _RIGHT_FACE,  {YELLOW, RED}},   // DR
    {{ 0, -1, -1}, _BOTTOM_FACE, _BACK_FACE,   {YELLOW, BLUE}},  // DB
    {{-1, -1,  0}, _BOTTOM_FACE, _LEFT_FACE,   {YELLOW, ORANGE}} // DL
};

std::vector<Corner> standardCornerPieces = {
    // Up layer
    {
        { 1,  1,  1},
        _TOP_FACE,
        _RIGHT_FACE,
        _FRONT_FACE,
        {WHITE, RED, GREEN}
    }, // URF

    {
        { 1,  1, -1},
        _TOP_FACE,
        _RIGHT_FACE,
        _BACK_FACE,
        {WHITE, RED, BLUE}
    }, // URB

    {
        {-1,  1, -1},
        _TOP_FACE,
        _LEFT_FACE,
        _BACK_FACE,
        {WHITE, ORANGE, BLUE}
    }, // ULB

    {
        {-1,  1,  1},
        _TOP_FACE,
        _LEFT_FACE,
        _FRONT_FACE,
        {WHITE, ORANGE, GREEN}
    }, // ULF

    // Down layer
    {
        { 1, -1,  1},
        _BOTTOM_FACE,
        _RIGHT_FACE,
        _FRONT_FACE,
        {YELLOW, RED, GREEN}
    }, // DRF

    {
        { 1, -1, -1},
        _BOTTOM_FACE,
        _RIGHT_FACE,
        _BACK_FACE,
        {YELLOW, RED, BLUE}
    }, // DRB

    {
        {-1, -1, -1},
        _BOTTOM_FACE,
        _LEFT_FACE,
        _BACK_FACE,
        {YELLOW, ORANGE, BLUE}
    }, // DLB

    {
        {-1, -1,  1},
        _BOTTOM_FACE,
        _LEFT_FACE,
        _FRONT_FACE,
        {YELLOW, ORANGE, GREEN}
    } // DLF
};
