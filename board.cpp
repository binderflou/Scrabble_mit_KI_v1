#include "board.h"
#include <iostream>
#define NORMAL "\033[0m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define LIGHTBLUE "\033[94m"
#define LIGHTRED "\033[91m"

board::board() {
	area[0][0].bonus = Bonus::TW;
	area[0][3].bonus = Bonus::DL;
	area[0][7].bonus = Bonus::TW;
	area[0][11].bonus = Bonus::DL;
	area[0][14].bonus = Bonus::TW;
	area[1][1].bonus = Bonus::DW;
	area[1][5].bonus = Bonus::TL;
	area[1][9].bonus = Bonus::TL;
	area[1][13].bonus = Bonus::DW;
	area[2][2].bonus = Bonus::DW;
	area[2][6].bonus = Bonus::DL;
	area[2][8].bonus = Bonus::DL;
	area[2][12].bonus = Bonus::DW;
	area[3][0].bonus = Bonus::DL;
	area[3][3].bonus = Bonus::DW;
	area[3][7].bonus = Bonus::DL;
	area[3][11].bonus = Bonus::DW;
	area[3][14].bonus = Bonus::DL;
	area[4][4].bonus = Bonus::DW;
	area[4][10].bonus = Bonus::DW;
	area[5][1].bonus = Bonus::TL;
	area[5][5].bonus = Bonus::TL;
	area[5][9].bonus = Bonus::TL;
	area[5][13].bonus = Bonus::TL;
	area[6][2].bonus = Bonus::DL;
	area[6][6].bonus = Bonus::DL;
	area[6][8].bonus = Bonus::DL;
	area[6][12].bonus = Bonus::DL;
	area[7][0].bonus = Bonus::TW;
	area[7][3].bonus = Bonus::DL;
	area[7][7].bonus = Bonus::DW;
	area[7][11].bonus = Bonus::DL;
	area[7][14].bonus = Bonus::TW;
	area[8][2].bonus = Bonus::DL;
	area[8][6].bonus = Bonus::DL;
	area[8][8].bonus = Bonus::DL;
	area[8][12].bonus = Bonus::DL;
	area[9][1].bonus = Bonus::TL;
	area[9][5].bonus = Bonus::TL;
	area[9][9].bonus = Bonus::TL;
	area[9][13].bonus = Bonus::TL;
	area[10][4].bonus = Bonus::DW;
	area[10][10].bonus = Bonus::DW;
	area[11][0].bonus = Bonus::DL;
	area[11][3].bonus = Bonus::DW;
	area[11][7].bonus = Bonus::DL;
	area[11][11].bonus = Bonus::DW;
	area[11][14].bonus = Bonus::DL;
	area[12][2].bonus = Bonus::DW;
	area[12][6].bonus = Bonus::DL;
	area[12][8].bonus = Bonus::DL;
	area[12][12].bonus = Bonus::DW;
	area[13][1].bonus = Bonus::DW;
	area[13][5].bonus = Bonus::TL;
	area[13][9].bonus = Bonus::TL;
	area[13][13].bonus = Bonus::DW;
	area[14][0].bonus = Bonus::TW;
	area[14][3].bonus = Bonus::DL;
	area[14][7].bonus = Bonus::TW;
	area[14][11].bonus = Bonus::DL;
	area[14][14].bonus = Bonus::TW;
}

void board::display() const {
	std::cout << "------------------------------------------------------------------------------------------\n";
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (area[i][j].tile) {
				std::cout << "|" << area[i][j].tile->letter << "   |";
			}
			else {
				bonus_up(area[i][j].bonus);
			}
		}
		std::cout << "\n";

		for(int j = 0; j < 15; j++) {
			if (area[i][j].tile) {
				std::cout << "|   " << area[i][j].tile->value << "|";
			}
			else {
				bonus_down(area[i][j].bonus);
			}
		}
		std::cout << "\n";
		std::cout << "------------------------------------------------------------------------------------------\n";
	}
}

void board::bonus_up(Bonus bonus) const{
	switch (bonus) {
	case Bonus::DL: std::cout << "|" << LIGHTBLUE << "D   " << NORMAL << "|"; break;
	case Bonus::DW: std::cout << "|" << LIGHTRED << "D   " << NORMAL << "|"; break;
	case Bonus::TL: std::cout << "|" << BLUE << "T   " << NORMAL << "|"; break;
	case Bonus::TW: std::cout << "|" << RED << "T   " << NORMAL << "|"; break;
	default: std::cout << "|    |"; break;
	}
}

void board::bonus_down(Bonus bonus) const{
	switch (bonus) {
	case Bonus::DL: std::cout << "|" << LIGHTBLUE << "   L" << NORMAL << "|"; break;
	case Bonus::DW: std::cout << "|" << LIGHTRED << "   W" << NORMAL << "|"; break;
	case Bonus::TL: std::cout << "|" << BLUE << "   L" << NORMAL << "|"; break;
	case Bonus::TW: std::cout << "|" << RED << "   W" << NORMAL << "|"; break;
	default: std::cout << "|    |"; break;
	}
}
