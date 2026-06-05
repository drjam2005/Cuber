#include "game.h"
#include "raylib.h"

Game::Game() {
	if(solveLists.empty()){
		solveLists.resize(1);
		solveLists[0].sessionName = "session1";
	}

	_load_scores();

	windowDimensions.x = GetScreenWidth();
	windowDimensions.y = GetScreenHeight();

}

void Game::Update(){
	this->windowDimensions.x = GetScreenWidth();
	this->windowDimensions.y = GetScreenHeight();

	_handle_scores();
	if(!selectedSolve)
		_handle_cube_state();

}

void Game::Render(){
	this->cube.Render();
	_handle_ui();
}

bool Game::IsEnded(){
	return this->cube.IsEnded();
}

