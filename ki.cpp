#include "ki.h"

std::vector<Move> Ki::calculateBestMove(Board& board, std::set<std::string>& dictionary) {
	m_possibleMoves.clear();
	auto hand = m_hand;

	if (board.isEmpty(7, 7)) {
		std::vector<PlacementKi> placements;
		findWord(7, 7, "", dictionary, hand, placements, board, true);
		findWord(7, 7, "", dictionary, hand, placements, board, false);
		return m_possibleMoves;
	}

	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
			if (isStartPoint(board, r, c)) {

				for (int i = 1; i <= 4; i++) {
					if (c - i >= 0 && !board.isEmpty(r, c - i)) {
						std::vector<PlacementKi> testPlacements;
						findWord(r, c - i, "", dictionary, hand, testPlacements, board, true);
					}
				}
				for (int i = 1; i <= 4; i++) {
					if (r - i >= 0 && !board.isEmpty(r - i, c)) {
						std::vector<PlacementKi> testPlacements;
						findWord(r - i, c, "", dictionary, hand, testPlacements, board, false);
					}
				}
				std::vector<PlacementKi> currentPlacements;
				findWord(r, c, "", dictionary, hand, currentPlacements, board, true);
				findWord(r, c, "", dictionary, hand, currentPlacements, board, false);
			}
		}
	}
	return m_possibleMoves;
}

bool Ki::isStartPoint(const Board& board, int row, int col) {
	if (board.isEmpty(row, col)) {
		if ((row > 0 && !board.isEmpty(row - 1, col)) ||
			(row < 14 && !board.isEmpty(row + 1, col)) ||
			(col > 0 && !board.isEmpty(row, col - 1)) ||
			(col < 14 && !board.isEmpty(row, col + 1))) {
			return true;
		}
	}
	return false;
}

void Ki::findWord(int row, int col, std::string currentWord, std::set<std::string>& dictionary, std::vector<Tile>& hand, std::vector<PlacementKi> currentPlacements, Board& board, bool isHorizontal) {
	if (!board.isEmpty(row, col)) {
		std::string nextWord = currentWord + board.getTile(row, col)->letter;

		if (!wordStillPossible(nextWord, dictionary)) {
			return;
		}
		int nextRow = row + (isHorizontal ? 0 : 1);
		int nextCol = col + (isHorizontal ? 1 : 0);

		findWord(nextRow, nextCol, nextWord, dictionary, hand, currentPlacements, board, isHorizontal);
	return;
	}
	
	if (dictionary.count(currentWord)) {
		Move move;
		move.word = currentWord;
		move.placements = currentPlacements;
		m_possibleMoves.push_back(move);
	}

	if (!wordStillPossible(currentWord, dictionary)) {
		return;
	}

	if (hand.empty()) {
		return;
	}

	for (int i = 0; i < hand.size(); i++) {
		Tile check = hand[i];
		std::string next = currentWord + check.letter;

		if (!wordStillPossible(next, dictionary)) {
			continue;
		}

		if (!checkSecondary(row, col, check.letter, dictionary, board, isHorizontal)) {
			continue;
		}

		PlacementKi placement = { row, col, check };
		currentPlacements.push_back(placement);

		std::vector<Tile> newHand = hand;
		newHand.erase(newHand.begin() + i);

		int nextRow = row + (isHorizontal ? 0 : 1);
		int nextCol = col + (isHorizontal ? 1 : 0);

		findWord(nextRow, nextCol, next, dictionary, newHand, currentPlacements, board, isHorizontal);
		currentPlacements.pop_back();
	}
}

bool Ki::wordStillPossible(std::string& current, std::set<std::string>& dictionary) {
	if (current.empty()) {
		return true;
	}

	auto first = dictionary.lower_bound(current);

	if (first == dictionary.end()) {
		return false;
	}

	return first->rfind(current, 0) == 0;
}

bool Ki::checkSecondary(int row, int col, const std::string& letter, std::set<std::string>& dictionary, Board& board, bool isHorizontal) {
	int secondaryStartRow = row;
	int secondaryStartCol = col;
	bool secondaryDirectionVertical = isHorizontal;

	if (!secondaryDirectionVertical) {
		while (secondaryStartCol > 0 && !board.isEmpty(secondaryStartRow, secondaryStartCol - 1)) {
			secondaryStartCol--;
		}
	}
	else {
		while (secondaryStartRow > 0 && !board.isEmpty(secondaryStartRow - 1, secondaryStartCol)) {
			secondaryStartRow--;
		}
	}

	std::string secondaryWord = "";

	int currentRow = secondaryStartRow;
	int currentCol = secondaryStartCol;

	while (currentRow < 15 && currentCol < 15 && !board.isEmpty(currentCol, currentRow)) {
		if (currentRow == row && currentCol == col) {
			secondaryWord += letter;
		}
		else {
			secondaryWord += letter;
			auto tileptr = board.getTile(currentCol, currentRow);
			if (tileptr != nullptr) {
				secondaryWord += tileptr->letter;
			} else {
				break;
			}
		}
		if (secondaryDirectionVertical) {
			currentRow++;
		}
		else {
			currentCol++;
		}
	}
	if (secondaryWord.length() <= 1) {
		return true;
	}

	return dictionary.count(secondaryWord) > 0;
}

int Ki::simulateScore(const std::vector<PlacementKi>& placements, Board& board, std::string word) {
	return 0;
}