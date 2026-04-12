#pragma once
#include <string>
#include "tile.h"
#include <vector>
#include <iostream>

class Player {
private:
	int m_score;
	std::string m_name;
	std::vector<Tile> m_hand;
	const size_t MAX_HAND = 8;

public:
	Player(std::string name);
	std::string getName();
	int getScore();
	void setScore(int score);
	int countTiles();
	Tile takeTile(std::string letter);
	void drawTiles(Bag& bag);
	void putBackTile(Bag& bag, std::string letter);
	void displayHand() const;
	bool hasTile(std::string letter) const;
	void giveTile(Tile* t);
};

