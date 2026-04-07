#include "player.h"
#include <windows.h>


Player::Player(std::string name) : m_name(name) {

}

std::string Player::getName() {
	return m_name;
}

int Player::getScore() {
	return m_score;
}

void Player::setScore(int score) {
	m_score = score;
};

int Player::countTiles() {
	return m_hand.size();
};

Tile Player::takeTile(std::string letter) {
	for (int i = 0; i < m_hand.size(); i++) {
		if (m_hand[i].letter == letter) {
			Tile t = m_hand[i];                  // Kopie
			m_hand.erase(m_hand.begin() + i);    // Original weg
			return t;                            // Kopie zurück
		}
	}
	return Tile{}; //To-Do umbauen auf throw in try-catch-Block!!!
}

void Player::drawTiles(Bag& bag) {
	while (m_hand.size() < MAX_HAND && !bag.isEmpty()) {
		m_hand.push_back(bag.drawTile());
	}
}

void Player::putBackTile(Bag& bag, std::string letter) {
	Tile t = takeTile(letter);   // entfernt aus m_hand
	bag.putBackTile(t);          // legt in Bag
};

void Player::displayHand() const {
	std::cout << "\n";
	std::cout << m_name << ":\n";
	for (const auto& tile : m_hand) {
		std::cout << "------ ";
	}
	std::cout << std::endl;

	//std::cout << "------ ------ ------ ------ ------ ------ ------ ------ " << std::endl;
	//std::cout << "      ";
	for (const auto& tile : m_hand) {
		std::cout << "|" << tile.letter << "   | ";
	}
	std::cout << std::endl;
	//std::cout << "      ";
	for (const auto& tile : m_hand) {
		if (tile.value < 10) {
			std::cout << "|   " << tile.value << "| ";
		}
		else { std::cout << "|  " << tile.value << "| "; }
	}
	std::cout << std::endl;

	for (const auto& tile : m_hand) {
		std::cout << "------ ";
	}
	std::cout << std::endl;
	//std::cout << "      ";
	//std::cout << "------ ------ ------ ------ ------ ------ ------ ------ " << std::endl;
}

bool Player::hasTile(std::string letter) const {
	for (const auto& tile : m_hand) {
		if (tile.letter == letter) {
			return true;
		}
	}
	return false;
}