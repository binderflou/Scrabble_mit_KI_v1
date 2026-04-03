#pragma once
#include <vector>
#include <string>

struct Tile {
	std::string letter;
	int value;
};

class Bag {
	private:
		std::vector<Tile> tiles;
	public:
		Bag(const std::string& configPath);
		bool isEmpty() const;
		void shuffle();
		Tile drawTile();
		void putBackTile(const Tile& tile);
};

