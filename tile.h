#pragma once
#include <vector>
#include <string>

struct Tile {
	char letter;
	int value;
};

class Bag {
	private:
		std::vector<Tile> tiles;
	public:
		Bag(const std::string& configPath);
		bool isEmpty() const;
		Tile drawTile();
};

