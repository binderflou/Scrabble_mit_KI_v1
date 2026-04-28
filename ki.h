#pragma once
#include "player.h"
#include "game.h"

struct Move {
	std::vector<Placement> placements;
	std::string word;
	int score;
	bool isHorizontal;
};

class Ki : public Player {
public:
	Ki(std::string name) : Player(name) {}
	std::vector<Move> calculateBestMove(Board& board, Bag& bag);
	int simulateScore(const std::vector<Placement>& placements, Board& board, std::string word);
};