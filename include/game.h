#pragma once
#include <sys/types.h>
#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "cube.h"

enum SOLVE_TYPE {
	COMPLETE = 0,
	DNF
};

enum CUBE_STATE {
	IN_SOLVE,
	IN_INSPECTION,
	IN_IDLE
};

struct Solve {
	std::string scramble = "";
	int solveNumber = 0;
	SOLVE_TYPE type;

	float inspectionTime = 0.0f;
	float solveTime = 0.0f;
};

struct SolveList {
	std::string sessionName = "";
	std::vector<Solve> solves;

	float get_ao5();
	float get_ao12();
};

class Game {
private:
	Cube cube;
	CUBE_STATE cubeState = IN_IDLE;
	std::vector<SolveList> solveLists;
	size_t currentListIndex = 0;

	float solveTime = 0.0f;
	float inspectionTime = 0.0f;

	Vector2 windowDimensions;


public:
	Game();
	void Update();
	void Render();

	bool IsEnded();
private:
	void _handle_cube_state();
	void _handle_ui();
	void _handle_scores();
private:
	void _render_session();
	void _render_scramble();
	void _handle_selected_score();
private:
	void _push_score(bool isSolved=true);
	void _save_scores();
	void _load_scores();

	Solve* selectedSolve = nullptr;
};

#endif
