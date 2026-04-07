#pragma once
#include "tile.h"

enum class Bonus {NONE, DL, DW, TL, TW};

struct Square {
	Tile* tile = nullptr;
	Bonus bonus = Bonus::NONE;
};

class Board {
	private:
		Square area[15][15];
	public:
		Board();
		void display() const;
		void bonus_up(Bonus bonus) const;
		void bonus_down(Bonus bonus) const;
		bool placeTile(Tile* tile, int x, int y);
		bool isEmpty(int x, int y) const;
};

