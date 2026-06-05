#include "game.h"

#include <iostream>
#include "helpers.h"
#include "raylib.h"

void Game::_handle_cube_state(){
	float dt = GetFrameTime();
	switch(cubeState){
		case IN_INSPECTION:
			inspectionTime += dt;
			break;
		case IN_SOLVE:
			solveTime += dt;
			break;
		default:
			break;
	}

	bool startedInspection = false;
	if(IsKeyPressed(KEY_SPACE) && cube.IsSolved() && cubeState == IN_IDLE){
		cubeState = IN_INSPECTION;
		startedInspection = true;
	}
	if(!startedInspection && cube.IsMoving() && cubeState == IN_INSPECTION){
		cubeState = IN_SOLVE;
	}
	if(IsKeyPressed(KEY_ESCAPE) && (cubeState == IN_SOLVE || cubeState == IN_INSPECTION)){
		_push_score(false);
		solveTime = 0.0f;
		inspectionTime = 0.0f;
		cubeState = IN_IDLE;
	}

	if(cube.IsMoving() && cube.IsSolved() && cubeState == IN_SOLVE){
		_push_score(true);
		solveTime = 0.0f;
		inspectionTime = 0.0f;
		cubeState = IN_IDLE;
	}
	this->cube.Update();
}
