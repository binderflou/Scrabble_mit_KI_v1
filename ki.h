#pragma once
#include "player.h"
#include "board.h"
#include <algorithm>
#include <set>

struct Move {
	std::vector<PlacementKi> placements;
	std::string word;
	int score;
	bool isHorizontal;
};

class Ki : public Player {
private:
	std::vector<Move> m_possibleMoves;
public:
	Ki(std::string name) : Player(name) {}
	std::vector<Move> calculateBestMove(Board& board, std::set<std::string>& dictionary);
	bool isStartPoint(const Board& board, int row, int col);
	void findWord(int row, int col, std::string current, std::set<std::string>& dictionary, std::vector<Tile> hand, std::vector<PlacementKi> currentPlacements, Board& board, bool isHorizontal);
	bool wordStillPossible(std::string& current, std::set<std::string>& dictionary);
	bool checkSecondary(int row, int col, const std::string& letter, std::set<std::string>& dictionary, Board& board, bool isHorizontal);
	int simulateScore(const std::vector<PlacementKi>& placements, Board& board, std::string word);
	bool checkSimulatedMove(const std::vector<PlacementKi>& placements, Board& board, int row, int col);
	int getSimulatedTileScore(const std::vector<PlacementKi>& placements, int row, int col);
};