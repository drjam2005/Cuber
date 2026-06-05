#include "game.h"
#include <iostream>
#include "raylib.h"

#define MINI_CASE_SENSITIVE
#include "ini.h"

void Game::_push_score(bool isSolved){
	Solve newSolve;
	newSolve.solveNumber = solveLists[currentListIndex].solves.size() + 1;
	newSolve.solveTime = solveTime;
	newSolve.inspectionTime = inspectionTime;
	newSolve.type = COMPLETE;
	newSolve.scramble = this->cube.GetScramble();

	if(!isSolved)
		newSolve.type = DNF;

	solveLists[currentListIndex].solves.push_back(newSolve);
	std::cout << "pushing score!" << std::endl;
	_save_scores();
}

void Game::_handle_scores(){
	SolveList& currentList = solveLists[currentListIndex];
}

void Game::_load_scores(){
	if(!FileExists("scores/config.ini"))
		return;
	mINI::INIFile config("scores/config.ini");
	mINI::INIStructure iniConfig;
	config.read(iniConfig);

	solveLists.resize(
			TextToInteger(iniConfig["CURRENT"]["count"].c_str())
		);
	currentListIndex = TextToInteger(iniConfig["CURRENT"]["index"].c_str());

	for(int i = 1; i <= solveLists.size(); ++i){
		SolveList& solveList = solveLists[i-1];

		mINI::INIFile file("scores/session_" + std::to_string(i) + ".ini");
		mINI::INIStructure ini;
		file.read(ini);

		solveList.sessionName = ini["DATA"]["sessionName"];
		solveList.solves.resize(
			TextToInteger(ini["DATA"]["solveCount"].c_str())
		);

		int j = 1;
		for(auto& solve : solveList.solves){
			solve.solveNumber = TextToInteger(ini[TextFormat("score_%i", j)]["NO"].c_str());
			solve.scramble = ini[TextFormat("score_%i", j)]["SCRAMBLE"];
			solve.type = (SOLVE_TYPE)TextToInteger(ini[TextFormat("score_%i", j)]["TYPE"].c_str());
			solve.inspectionTime = TextToFloat(ini[TextFormat("score_%i", j)]["INSPECTION_TIME"].c_str());
			solve.solveTime = TextToFloat(ini[TextFormat("score_%i", j)]["SOLVE_TIME"].c_str());
			j++;
		}

		std::string sessionName = solveList.sessionName;
	}
}

void Game::_save_scores(){
	if(!DirectoryExists("scores"))
		MakeDirectory("scores");

	mINI::INIFile config("scores/config.ini");
	mINI::INIStructure iniConfig;
	config.read(iniConfig);

	iniConfig["CURRENT"]["count"] = std::to_string(solveLists.size());
	iniConfig["CURRENT"]["index"] = std::to_string(currentListIndex);

	config.write(iniConfig, true);

	int j = 1;
	for(auto& solveList : solveLists){
		if(solveList.solves.empty())
			continue;
		std::string sessionName = solveList.sessionName;

		mINI::INIFile file("scores/session_" + std::to_string(j) + ".ini");
		mINI::INIStructure ini;
		file.read(ini);

		ini["DATA"]["sessionName"] = solveList.sessionName;
		ini["DATA"]["solveCount"] = std::to_string(solveList.solves.size());

		int i = 1;
		for(auto& solve : solveList.solves){
			ini[TextFormat("score_%i", i)]["NO"] = std::to_string(solve.solveNumber);
			ini[TextFormat("score_%i", i)]["SCRAMBLE"] = solve.scramble;
			ini[TextFormat("score_%i", i)]["TYPE"] = std::to_string((int)solve.type);
			ini[TextFormat("score_%i", i)]["INSPECTION_TIME"] = TextFormat("%.2f", solve.inspectionTime);
			ini[TextFormat("score_%i", i)]["SOLVE_TIME"] = TextFormat("%.2f",solve.solveTime);

			i++;
		}

		file.write(ini, true);
		j++;
	}
}
