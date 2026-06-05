#include "game.h"
#include "helpers.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void Game::_handle_ui(){
	if(selectedSolve != nullptr){
		_handle_selected_score();
		return;
	}
	_render_scramble();
	_render_session();
}

void Game::_render_scramble(){
	// scramble
	DrawRectangle(0, 0, windowDimensions.x, (windowDimensions.y/10.0f)/2.0f+35, DARKGRAY);
	if(cubeState != IN_IDLE){
		Vector2 textDims = MeasureTextEx(GetFontDefault(), TextFormat("%s", this->cube.GetScramble().c_str()), 20, (int)20/10);
		DrawText(TextFormat("%s", this->cube.GetScramble().c_str()), 
				(windowDimensions.x - textDims.x)/2,
				(windowDimensions.y/10.0f)/2.0f,
			20, LIGHTGRAY);
	}
}

void Game::_render_session(){
	SolveList& currentList = solveLists[currentListIndex];

	// render the uhhh list
	float x = 10.0f;
	float y = windowDimensions.y / 10.0f + 50.0f;
	// background
	DrawText(TextFormat("Session: %s", currentList.sessionName.c_str()), x-5, y-30, 20, BLACK);
	if(GuiButton({x+5, y-10, 30, 17}, "<")){
		if(currentListIndex > 0)
			currentListIndex--;
	}
	if(GuiButton({x+40, y-10, 30, 17}, ">")){
		if(currentListIndex + 1< solveLists.size()){
			currentListIndex++;
			return;
		}
		if(!solveLists[currentListIndex].solves.empty()){
			solveLists.push_back(
					SolveList{
						TextFormat("session%d", currentListIndex+1)
					}
				);
			currentListIndex = solveLists.size() - 1;
		}
	}
	DrawRectangle(
			x, y+10,
			130,
			currentList.solves.size() * 25,
			(Color){150, 150, 150, 200}
		);
	for(auto& solve : currentList.solves){
		if(solve.type == DNF){
			DrawText( TextFormat("DNF"), x+5, y+15, 20, MAROON);
		} else{
			DrawText( TextFormat("%02.0f:%05.2f\n", getMinutes(solve.solveTime),getSeconds(solve.solveTime)) , x+5, y+15, 20, DARKGREEN);
		}

		if(GuiButton({x+95, y+15, 30, 17}, "View"))
			selectedSolve = &solve;

		y += 25;
	}

}

void Game::_handle_selected_score(){
	Rectangle uiBounds = {
		windowDimensions.x/4,
		windowDimensions.y/4,
		windowDimensions.x/2,
		windowDimensions.y/2
	};

	Rectangle scrambleBounds = {
		uiBounds.x + uiBounds.width*(1.0f/16),
		uiBounds.y + uiBounds.height*(1.0f/4),
		uiBounds.width*(3.0f/4),
		uiBounds.y*(3.0f/4)
	};

	DrawRectangleRec(uiBounds, GRAY);
	DrawRectangleRec(
			{
				scrambleBounds.x ,
				scrambleBounds.y ,
				scrambleBounds.width + 10,
				scrambleBounds.height + 10
			}
			, LIGHTGRAY);

	DrawTextBoxed(GetFontDefault(), selectedSolve->scramble.c_str(), 
			{
				scrambleBounds.x + 10,
				scrambleBounds.y + 10,
				scrambleBounds.width,
				scrambleBounds.height
			}
			, 20, 2, true, BLACK);

	DrawText(TextFormat("Session: %s", solveLists[currentListIndex].sessionName.c_str()), 
				uiBounds.x + 10,
				uiBounds.y + 10,
				20,
				BLACK
			);
	DrawText(TextFormat("Solve No: %i", selectedSolve->solveNumber),
				uiBounds.x + 10,
				uiBounds.y + 35,
				20,
				BLACK
			);

	if(selectedSolve->type == DNF){
		DrawText("DNF",
				uiBounds.x + 10,
				scrambleBounds.y + scrambleBounds.height + 25,
				20,
				RED
			);
		DrawText(TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(selectedSolve->inspectionTime),getSeconds(selectedSolve->inspectionTime)),
					uiBounds.x + 10,
					scrambleBounds.y + scrambleBounds.height + 50,
					20,
					MAROON
				);

		DrawText(TextFormat("Solve Time: %02.0f:%05.2f\n", getMinutes(selectedSolve->solveTime),getSeconds(selectedSolve->solveTime)),
					uiBounds.x + 10,
					scrambleBounds.y + scrambleBounds.height + 75,
					20,
					MAROON
				);
	}else{
		DrawText(TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(selectedSolve->inspectionTime),getSeconds(selectedSolve->inspectionTime)),
					uiBounds.x + 10,
					scrambleBounds.y + scrambleBounds.height + 50,
					20,
					BLACK
				);

		DrawText(TextFormat("Solve Time: %02.0f:%05.2f\n", getMinutes(selectedSolve->solveTime),getSeconds(selectedSolve->solveTime)),
					uiBounds.x + 10,
					scrambleBounds.y + scrambleBounds.height + 75,
					20,
					BLACK
				);
	}

	if(GuiButton({
				uiBounds.x + uiBounds.width - 80 - 100,
				uiBounds.y + uiBounds.height - 40,
				80,
				30
				}, "back"))
		selectedSolve = nullptr;
	if(GuiButton({
				uiBounds.x + uiBounds.width - 80 - 10,
				uiBounds.y + uiBounds.height - 40,
				80,
				30
				}, "DELETE")){
		solveLists[currentListIndex].solves.erase(
				solveLists[currentListIndex].solves.begin() + 
				(selectedSolve - &solveLists[currentListIndex].solves[0])
				);
		selectedSolve = nullptr;

		_save_scores();
	}
}
