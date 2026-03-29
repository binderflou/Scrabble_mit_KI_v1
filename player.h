#pragma once
#include <string>
#include "tile.h"
#include <vector>
#include <iostream>

class player {
private:
	std::vector<Tile> hand;
	const size_t MAX_HAND = 8;
public:
	void drawTiles(Bag& bag);
	void displayHand() const;
};

