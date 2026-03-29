#include <stdio.h>
#include "board.h"
#include "player.h"
#include <windows.h>

int main() {
	SetConsoleOutputCP(CP_UTF8);
	board board;
	player player;
	Bag bag("config_de.txt");

	board.display();
	player.drawTiles(bag);
	player.displayHand();
}