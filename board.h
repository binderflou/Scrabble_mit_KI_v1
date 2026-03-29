#pragma once
#include "tile.h"

enum class Bonus {NONE, DL, DW, TL, TW};

struct Square {
	Tile* tile = nullptr;
	Bonus bonus = Bonus::NONE;
};

class board {
	private:
		Square area[15][15];
	public:
		board();
		void display() const;
		void bonus_up(Bonus bonus) const;
		void bonus_down(Bonus bonus) const;
		bool placeTile(Tile* tile, int x, int y);
};

