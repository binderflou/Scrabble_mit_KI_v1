#include <stdio.h>
#include "board.h"
#include "player.h"
#include <windows.h>
#include <iostream>

int main() {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	board board;
	player player;
	Bag bag("config_de.txt");

	board.display();
	player.drawTiles(bag);
	player.displayHand();
}