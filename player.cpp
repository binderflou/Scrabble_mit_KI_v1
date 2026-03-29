#include "player.h"
#include <windows.h>

void player::drawTiles(Bag& bag) {
	while (hand.size() < MAX_HAND && !bag.isEmpty()) {
		hand.push_back(bag.drawTile());
	}
}

void player::displayHand() const {
	std::cout << "\n";
	std::cout << "Hand: ";
	std::cout << "------ ------ ------ ------ ------ ------ ------ ------ " << std::endl;
	std::cout << "      ";
	for (const auto& tile : hand) {
		std::cout << "|" << tile.letter << "   | ";
	}
	std::cout << std::endl;
	std::cout << "      ";
	for (const auto& tile : hand) {
		if(tile.value <10) {
			std::cout << "|   " << tile.value << "| ";
		}
		else { std::cout << "|  " << tile.value << "| "; }
	}
	std::cout << std::endl;

	std::cout << "      ";
	std::cout << "------ ------ ------ ------ ------ ------ ------ ------ " << std::endl;
}
