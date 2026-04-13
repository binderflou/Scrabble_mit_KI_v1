#pragma once
#include <string>
#include "tile.h"
#include "player.h"
#include "board.h"

enum class RunCase {
	GetFirstPlayer,
	Draw,
	CheckDraw,
	ChangeActivePlayer,
};
struct Placement
{
	//std::string letter;
	int row;
	int col;
};

class Game
{
	//Spiel
	bool m_active;
	int m_numberOfPlayers = 1;
	std::string m_language = "config_de.txt";
	Bag m_bag;
	Board m_board;
	

	//Zug-Eigenschaften
	RunCase m_runCase = RunCase::GetFirstPlayer;
	int m_activePlayer = 0;
	int m_drawValue = 0;
	std::vector<Placement> m_drawPlacements; //Zuordnung vom Baustein zum den koordinaten

	bool isFirstTurn = true;

public:
	std::vector<Player> m_players;
	Game(int numberOfPlayers, std::string language);
	void run();			//Programmablauf
	void scoring();		//Auswertung Spiel
	void getFirstPlayer();
	int draw();
	bool checkDraw();
	std::string turnToUpper(std::string letter);
	bool isInLine();
	bool isConnected();
	bool hasAdjacent();
	int DrawScore();
	void changeActivePlayer();
	void returnTilesToPlayer();
};