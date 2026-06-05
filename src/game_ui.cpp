#include "game.h"
#include "helpers.h"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static float GetUiScale(Vector2 windowDimensions){
	float scaleX = windowDimensions.x / 768.0f;
	float scaleY = windowDimensions.y / 432.0f;
	float scale = scaleX < scaleY ? scaleX : scaleY;

	if(scale < 0.65f)
		return 0.65f;
	if(scale > 2.0f)
		return 2.0f;
	return scale;
}

static float ScaleUi(Vector2 windowDimensions, float value){
	return value * GetUiScale(windowDimensions);
}

static int ScaleFont(Vector2 windowDimensions, float value){
	return (int)(ScaleUi(windowDimensions, value) + 0.5f);
}


void Game::_handle_ui(){
	GuiSetStyle(DEFAULT, TEXT_SIZE, ScaleFont(windowDimensions, 20.0f));

	if(selectedSolve != nullptr){
		_handle_selected_score();
		return;
	}
	_render_scramble();
	_render_session();
}

void Game::_render_scramble(){
	const int textSize = ScaleFont(windowDimensions, 20.0f);
	const float textSpacing = ScaleUi(windowDimensions, 2.0f);
	const float headerHeight = (windowDimensions.y/10.0f)/2.0f + ScaleUi(windowDimensions, 35.0f);

	// scramble
	DrawRectangle(0, 0, windowDimensions.x, headerHeight, DARKGRAY);
	if(cubeState != IN_IDLE){
		Vector2 textDims = MeasureTextEx(GetFontDefault(), TextFormat("%s", this->cube.GetScramble().c_str()), textSize, textSpacing);
		DrawText(TextFormat("%s", this->cube.GetScramble().c_str()), 
				(windowDimensions.x - textDims.x)/2,
				(windowDimensions.y/10.0f)/2.0f,
			textSize, LIGHTGRAY);
	}
}

void Game::_render_session(){
	SolveList& currentList = solveLists[currentListIndex];
	static float solveListScroll = 0.0f;
	static size_t lastListIndex = currentListIndex;

	if(lastListIndex != currentListIndex){
		solveListScroll = 0.0f;
		lastListIndex = currentListIndex;
	}

	// render the uhhh list
	const int textSize = ScaleFont(windowDimensions, 20.0f);
	const float textSpacing = ScaleUi(windowDimensions, 2.0f);
	const float padding = ScaleUi(windowDimensions, 10.0f);
	const float x = padding;
	const float y = windowDimensions.y / 10.0f + ScaleUi(windowDimensions, 50.0f);
	const float rowHeight = ScaleUi(windowDimensions, 32.0f);
	const float buttonWidth = ScaleUi(windowDimensions, 52.0f);
	const float buttonHeight = ScaleUi(windowDimensions, 22.0f);
	const float listWidth = ScaleUi(windowDimensions, 170.0f);
	const Rectangle solveListBounds = {
		x,
		y+padding,
		listWidth,
		windowDimensions.y - (y+padding) - (padding * 2.0f)
	};
	const float solveListContentHeight = currentList.solves.size() * rowHeight;
	const float maxSolveListScroll = solveListContentHeight > solveListBounds.height
		? solveListContentHeight - solveListBounds.height
		: 0.0f;

	if(CheckCollisionPointRec(GetMousePosition(), solveListBounds)){
		solveListScroll -= GetMouseWheelMove() * rowHeight * 3.0f;
	}
	if(solveListScroll < 0.0f)
		solveListScroll = 0.0f;
	if(solveListScroll > maxSolveListScroll)
		solveListScroll = maxSolveListScroll;

	// background
	DrawText(TextFormat("Session: %s", currentList.sessionName.c_str()), x-(padding * 0.5f), y-ScaleUi(windowDimensions, 30.0f), textSize, BLACK);
	if(GuiButton({x+(padding * 0.5f), y-padding, buttonWidth, buttonHeight}, "<")){
		if(currentListIndex > 0){
			currentListIndex--;
			solveListScroll = 0.0f;
			return;
		}
	}
	if(GuiButton({x+(padding * 0.5f)+buttonWidth+ScaleUi(windowDimensions, 5.0f), y-padding, buttonWidth, buttonHeight}, ">")){
		if(currentListIndex + 1< solveLists.size()){
			currentListIndex++;
			solveListScroll = 0.0f;
			return;
		}
		if(!solveLists[currentListIndex].solves.empty()){
			solveLists.push_back( SolveList{ });
			currentListIndex = solveLists.size() - 1;
			solveLists[currentListIndex].sessionName = TextFormat("session%i", currentListIndex+1);
			solveListScroll = 0.0f;
			return;
		}
	}
	DrawRectangleRec(solveListBounds, (Color){150, 150, 150, 200});
	BeginScissorMode(
			solveListBounds.x,
			solveListBounds.y,
			solveListBounds.width,
			solveListBounds.height
			);
	for(size_t i = currentList.solves.size(); i-- > 0;){
		Solve& solve = currentList.solves[i];
		const float rowY = solveListBounds.y + ((currentList.solves.size() - 1 - i) * rowHeight) - solveListScroll;
		if(rowY + rowHeight < solveListBounds.y || rowY > solveListBounds.y + solveListBounds.height)
			continue;

		if(solve.type == DNF){
			DrawText( TextFormat("DNF"), x+(padding * 0.5f), rowY+(padding * 0.5f), textSize, MAROON);
		} else{
			DrawText( TextFormat("%02.0f:%05.2f\n", getMinutes(solve.solveTime),getSeconds(solve.solveTime)) , x+(padding * 0.5f), rowY+(padding * 0.5f), textSize, DARKGREEN);
		}

		if(GuiButton({x+listWidth-buttonWidth-padding, rowY+(padding * 0.5f), buttonWidth, buttonHeight}, "View"))
			selectedSolve = &solve;
	}
	EndScissorMode();

	if(maxSolveListScroll > 0.0f){
		const float scrollBarHeight = (solveListBounds.height / solveListContentHeight) * solveListBounds.height;
		const float scrollBarY = solveListBounds.y + (solveListScroll / maxSolveListScroll) * (solveListBounds.height - scrollBarHeight);
		DrawRectangle(solveListBounds.x + solveListBounds.width - ScaleUi(windowDimensions, 4.0f), scrollBarY, ScaleUi(windowDimensions, 4.0f), scrollBarHeight, DARKGRAY);
	}


	switch(cubeState){
		case IN_INSPECTION:
			{
				Vector2 textDims = MeasureTextEx(GetFontDefault(),
												TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(inspectionTime),getSeconds(inspectionTime)),
												textSize,
												textSpacing
												);
				DrawText(TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(inspectionTime),getSeconds(inspectionTime)),
					this->windowDimensions.x - textDims.x - padding,
					this->windowDimensions.y/2.0f - textDims.y - padding,
					textSize,
					BLACK
				);
				break;
			}
		case IN_SOLVE:
			{
				Vector2 textDims = MeasureTextEx(GetFontDefault(),
												TextFormat("%02.0f:%05.2f\n", getMinutes(solveTime),getSeconds(solveTime)),
												textSize,
												textSpacing
												);
				DrawText(TextFormat("%02.0f:%05.2f\n", getMinutes(solveTime),getSeconds(solveTime)),
					this->windowDimensions.x - textDims.x - padding,
					this->windowDimensions.y/2.0f - textDims.y - padding,
					textSize,
					BLACK
				);
				break;
			}
			break;
		default:
			break;

	}
}

void Game::_handle_selected_score(){
	const int textSize = ScaleFont(windowDimensions, 20.0f);
	const float textSpacing = ScaleUi(windowDimensions, 2.0f);
	const float padding = ScaleUi(windowDimensions, 10.0f);
	const float lineHeight = ScaleUi(windowDimensions, 25.0f);
	const float buttonWidth = ScaleUi(windowDimensions, 90.0f);
	const float buttonHeight = ScaleUi(windowDimensions, 30.0f);
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
				scrambleBounds.x + padding,
				scrambleBounds.y + padding,
				scrambleBounds.width,
				scrambleBounds.height
			}
			, textSize, textSpacing, true, BLACK);

	DrawText(TextFormat("Session: %s", solveLists[currentListIndex].sessionName.c_str()), 
				uiBounds.x + padding,
				uiBounds.y + padding,
				textSize,
				BLACK
			);
	DrawText(TextFormat("Solve No: %i", selectedSolve->solveNumber),
				uiBounds.x + padding,
				uiBounds.y + padding + lineHeight,
				textSize,
				BLACK
			);

	if(selectedSolve->type == DNF){
		DrawText("DNF",
				uiBounds.x + padding,
				scrambleBounds.y + scrambleBounds.height + lineHeight,
				textSize,
				RED
			);
		DrawText(TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(selectedSolve->inspectionTime),getSeconds(selectedSolve->inspectionTime)),
					uiBounds.x + padding,
					scrambleBounds.y + scrambleBounds.height + (lineHeight * 2.0f),
					textSize,
					MAROON
				);

		DrawText(TextFormat("Solve Time: %02.0f:%05.2f\n", getMinutes(selectedSolve->solveTime),getSeconds(selectedSolve->solveTime)),
					uiBounds.x + padding,
					scrambleBounds.y + scrambleBounds.height + (lineHeight * 3.0f),
					textSize,
					MAROON
				);
	}else{
		DrawText(TextFormat("Inspection: %02.0f:%05.2f\n", getMinutes(selectedSolve->inspectionTime),getSeconds(selectedSolve->inspectionTime)),
					uiBounds.x + padding,
					scrambleBounds.y + scrambleBounds.height + (lineHeight * 2.0f),
					textSize,
					BLACK
				);

		DrawText(TextFormat("Solve Time: %02.0f:%05.2f\n", getMinutes(selectedSolve->solveTime),getSeconds(selectedSolve->solveTime)),
					uiBounds.x + padding,
					scrambleBounds.y + scrambleBounds.height + (lineHeight * 3.0f),
					textSize,
					BLACK
				);
	}

	if(GuiButton({
				uiBounds.x + uiBounds.width - buttonWidth - ScaleUi(windowDimensions, 110.0f),
				uiBounds.y + uiBounds.height - buttonHeight - padding,
				buttonWidth,
				buttonHeight
				}, "back"))
		selectedSolve = nullptr;
	if(GuiButton({
				uiBounds.x + uiBounds.width - buttonWidth - padding,
				uiBounds.y + uiBounds.height - buttonHeight - padding,
				buttonWidth,
				buttonHeight
				}, "DELETE")){
		solveLists[currentListIndex].solves.erase(
				solveLists[currentListIndex].solves.begin() + 
				(selectedSolve - &solveLists[currentListIndex].solves[0])
				);
		selectedSolve = nullptr;

		_save_scores();
	}
}
