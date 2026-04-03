#include "tile.h"
#include <fstream>
#include <algorithm>
#include <random>

Bag::Bag(const std::string& configPath) {
	std::ifstream file(configPath);
	if (!file.is_open()) {
		throw std::runtime_error("Datei nicht gefunden: " + configPath);
		return;
	}

	std::string letter;
	int value, count;

	while (file >> letter >> value >> count) {
		for (int i = 0; i < count; ++i) {
			tiles.push_back({ letter, value });
		}
	}

	file.close();

	shuffle();
}

bool Bag::isEmpty() const {
	return tiles.empty();
}

void Bag::shuffle() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(tiles.begin(), tiles.end(), g);
}

Tile Bag::drawTile() {
	if (isEmpty()) {
		throw std::runtime_error("Der Beutel ist leer!");
	}
	Tile tile = tiles.back();
	tiles.pop_back();
	return tile;
}
void Bag::putBackTile(const Tile& tile) {
	tiles.push_back(tile);
}