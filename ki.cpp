#include "ki.h"

std::vector<Move> Ki::calculateBestMove(Board& board, std::set<std::string>& dictionary) {
	m_possibleMoves.clear();
	auto hand = m_hand;

	//Sonderfall erstes Wort: Muss auf 7,7 liegen
	if (board.isEmpty(7, 7)) {
		std::vector<PlacementKi> placements;
		//Wörter in beide Richtungen suchen: Row, Collomn, Aktuelles Wort, Wörterbuch, Hand, Aktuelle Platzierungen, Board, Richtung
		findWord(7, 7, "", dictionary, hand, placements, board, true);
		findWord(7, 7, "", dictionary, hand, placements, board, false);
		return m_possibleMoves;
	}

	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
			if (isStartPointHorizontal(board, r, c)) {
				std::vector<PlacementKi> currentPlacements;
				findWord(r, c, "", dictionary, hand, currentPlacements, board, true);
			}
			if (isStartPointVertical(board, r, c)) {
				std::vector<PlacementKi> currentPlacements;
				findWord(r, c, "", dictionary, hand, currentPlacements, board, false);
			}
			if (isStartPoint(board, r, c)) {
				std::vector<PlacementKi> currentPlacements;
				findWord(r, c, "", dictionary, hand, currentPlacements, board, true);
				findWord(r, c, "", dictionary, hand, currentPlacements, board, false);
			}
		}
	}
	std::sort(m_possibleMoves.begin(), m_possibleMoves.end(), [](const Move& a, const Move& b) {
		return a.score > b.score;
		});

	return m_possibleMoves;
}

//Prüft ob aktuelles Feld leer ist und mindestens ein benachbartes Feld belegt ist
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

//Für Beginn weiter links
bool Ki::isStartPointHorizontal(const Board& board, int row, int col) {
	if (!board.isEmpty(row, col)) {
		return false;
	}

	int counter = 0;

	for (int c = col; c < 15; c++) {
		if (!board.isEmpty(row, c)) {
			return true;
		}
		if ((row > 0 && !board.isEmpty(row - 1, c)) ||
			(row < 14 && !board.isEmpty(row + 1, c)) ||
			(c < 14 && !board.isEmpty(row, c + 1))) {
			return true;
		}
		counter++;
		if (counter >= 7) {
			return false;
		}
	}
	return false;
}

//Für Beginn weiter oben
bool Ki::isStartPointVertical(const Board& board, int row, int col) {
	if (!board.isEmpty(row, col)) {
		return false;
	}

	int counter = 0;

	for (int r = row; r < 15; r++) {
		if (!board.isEmpty(r, col)) {
			return true;
		}
		if ((col > 0 && !board.isEmpty(r, col - 1)) ||
			(col < 14 && !board.isEmpty(r, col + 1)) ||
			(r < 14 && !board.isEmpty(r + 1, col))) {
			return true;
		}
		counter++;
		if (counter >= 7) {
			return false;
		}
	}
	return false;
}

//Hilfsfunktion um zu überprüfen, ob die neuen Steine mit bereits liegenden Steinen verbunden sind (außer erstes Wort)
bool Ki::connected(std::vector<PlacementKi>& placements, Board& board) {
	if (board.isEmpty(7, 7)) {
		return true;
	}
	for (const auto& placement : placements) {
		int row = placement.row;
		int col = placement.col;
		if ((row > 0 && !board.isEmpty(row - 1, col)) ||
			(row < 14 && !board.isEmpty(row + 1, col)) ||
			(col > 0 && !board.isEmpty(row, col - 1)) ||
			(col < 14 && !board.isEmpty(row, col + 1))) {
			return true;
		}
	}
	return false;
}

void Ki::findWord(int row, int col, std::string currentWord, std::set<std::string>& dictionary, std::vector<Tile> hand, std::vector<PlacementKi> currentPlacements, Board& board, bool isHorizontal) {
	//Abbruch Spielfeldrand
	if (row >= 15 || col >= 15) {
		if (dictionary.count(currentWord) && !currentPlacements.empty() && connected(currentPlacements, board)) {
			Move move;
			move.word = currentWord;
			move.placements = currentPlacements;
			move.score = simulateScore(currentPlacements, board, currentWord);
			m_possibleMoves.push_back(move);
		}
		return;
	}

	//Einlesen bereits liegender Steine
	if (currentWord.empty()) {

		if (isHorizontal) {
			while (col > 0 && !board.isEmpty(row, col - 1)) {
				col--;
			}
		}
		else {
			while (row > 0 && !board.isEmpty(row - 1, col)) {
				row--;
			}
		}
		std::string alreadyOnBoard = "";
		while (row < 15 && col < 15 && !board.isEmpty(row, col)) {
			alreadyOnBoard += board.getTile(row, col)->letter;
			if (isHorizontal) {
				col++;
			}
			else {
				row++;
			}
		}
		currentWord = alreadyOnBoard + currentWord;
	}

	if (!board.isEmpty(row, col)) {
		std::string nextWord = currentWord + board.getTile(row, col)->letter;
		//Prüfen ob aktuell betrachtetes Teilstück in Datenbank existiert
		if (!wordStillPossible(nextWord, dictionary)) {
			return;
		}
		int nextRow = row + (isHorizontal ? 0 : 1);
		int nextCol = col + (isHorizontal ? 1 : 0);

		findWord(nextRow, nextCol, nextWord, dictionary, hand, currentPlacements, board, isHorizontal);
		return;
	}
	
	//Wort in Vektor für mögliche Züge legen
	if (dictionary.count(currentWord) && !currentPlacements.empty() && connected(currentPlacements, board)) {
		Move move;
		move.word = currentWord;
		move.placements = currentPlacements;
		move.score = simulateScore(currentPlacements, board, currentWord);
		m_possibleMoves.push_back(move);
	}

	if (!wordStillPossible(currentWord, dictionary)) {
		return;
	}

	if (hand.empty()) {
		return;
	}

	//Bilden von Wörter
	for (int i = 0; i < hand.size(); i++) {
		Tile check = hand[i];

		//Joker berücksichtigen
		if (check.letter == "_") {
			Tile jokerTile;
			jokerTile.value = 0;
			const std::vector <std::string> alphabet = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Ä", "Ö", "Ü" };
			for (const std::string& dist_letter : alphabet) {
				jokerTile.letter = dist_letter;
				std::string next = currentWord + dist_letter;

				if (!wordStillPossible(next, dictionary)) {
					continue;
				}
				if (!checkSecondary(row, col, dist_letter, dictionary, board, isHorizontal)) {
					continue;
				}

				PlacementKi placement = { row, col, jokerTile };
				currentPlacements.push_back(placement);

				std::vector<Tile> newHand = hand;
				newHand.erase(newHand.begin() + i);

				int nextRow = row + (isHorizontal ? 0 : 1);
				int nextCol = col + (isHorizontal ? 1 : 0);

				findWord(nextRow, nextCol, next, dictionary, newHand, currentPlacements, board, isHorizontal);
				currentPlacements.pop_back();
			}
			continue;
		}
		//Standardfall
		else {
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
}

//Prüft Anfang von Wort, ob in Datenbank vorhanden
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

	if (secondaryDirectionVertical) {
		while (currentRow < 15) {
			if (currentRow == row) {
				secondaryWord += letter;
			}
			else if (!board.isEmpty(currentRow, currentCol)) {
				secondaryWord += board.getTile(currentRow, currentCol)->letter;
			}
			else {
				break;
			}
			currentRow++;
		}
	} else {
		while (currentCol < 15) {
			if (currentCol == col) {
				secondaryWord += letter;
			}
			else if (!board.isEmpty(currentRow, currentCol)) {
				secondaryWord += board.getTile(currentRow, currentCol)->letter;
			}
			else {
				break;
			}
			currentCol++;
		}
	}
	if (secondaryWord.length() <= 1) {
		return true;
	}

	return dictionary.count(secondaryWord) > 0;
}

//Simuliert möglichen Punkte eines Zuges
int Ki::simulateScore(const std::vector<PlacementKi>& placements, Board& board, std::string word) {
	int drawScore = 0;
	int wordMultiplier = 1;
	int totalSecondaryScore = 0;
	int totalScore = 0;

	if (placements.empty()) {
		return 0;
	}
	bool isHorizontal = true;

	//Wenn nur ein Stein gelegt wurde, Richtung anhand benachbarter Steine ermitteln
	if (placements.size() == 1) {
		int row = placements[0].row;
		int col = placements[0].col;
		if ((row > 0 && !board.isEmpty(row - 1, col)) || (row < 14 && !board.isEmpty(row + 1, col))) {
			isHorizontal = false;
		}
		else if ((col > 0 && !board.isEmpty(row, col - 1)) || (col < 14 && !board.isEmpty(row, col + 1))) {
			isHorizontal = true;
		}
	}

	//Wenn mehrere Steine gelegt wurden, Richtung anhand der gesetzten Steine ermitteln
	if (placements.size() > 1) {
		isHorizontal = placements[0].row == placements[1].row;
	}

	int startRow = placements[0].row;
	int startCol = placements[0].col;

	//In Richtung des Hauptworts zum Anfang laufen
	if (isHorizontal) {
		while (startCol > 0 && !board.isEmpty(startRow, startCol - 1)) {
			startCol--;
		}
	}
	else {
		while (startRow > 0 && !board.isEmpty(startRow - 1, startCol)) {
			startRow--;
		}
	}

	int accRow = startRow;
	int accCol = startCol;

	//Hauptwort durchlaufen
	while (accRow < 15 && accCol < 15 && (!board.isEmpty(accRow, accCol) || checkSimulatedMove(placements, board, accRow, accCol))) {
		int letterValue = 0;
		if (!board.isEmpty(accRow, accCol)) {
			letterValue = board.getTileValue(accRow, accCol);
		}
		else {
			letterValue = getSimulatedTileScore(placements, accRow, accCol);
		}

		for (const auto& placement : placements) {
			if (placement.row == accRow && placement.col == accCol) {
				Bonus bonusType = board.getTileMultiplier(placement.row, placement.col);
				switch (bonusType) {
				case Bonus::DL: letterValue *= 2; break;
				case Bonus::TL: letterValue *= 3; break;
				case Bonus::DW: wordMultiplier *= 2; break;
				case Bonus::TW: wordMultiplier *= 3; break;
				default: break;
				}
				break;
			}
		}
		drawScore += letterValue;

		if (isHorizontal) accCol++; else accRow++;
	}

	for (const auto& placement : placements) {
		//Sekundäre Richtung ermitteln

		int secondaryStartRow = placement.row;
		int secondaryStartCol = placement.col;
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

		int secondaryAccRow = secondaryStartRow;
		int secondaryAccCol = secondaryStartCol;

		int secondaryScore = 0;
		int secondaryMultiplier = 1;

		//Sekundäre Richtung (vertikal) durchlaufen
		if (secondaryDirectionVertical) {
			while (secondaryAccRow < 15 && secondaryAccCol < 15 && secondaryAccRow >= 0 && secondaryAccCol >= 0 && (!board.isEmpty(secondaryAccRow, secondaryAccCol) || checkSimulatedMove(placements, board, secondaryAccRow, secondaryAccCol))) {

				//while (secondaryAccRow < 15 && secondaryAccCol < 15 && secondaryAccRow >= 0 && secondaryAccCol >= 0 && (!board.isEmpty(secondaryAccRow + 1, secondaryAccCol) || !board.isEmpty(secondaryAccRow - 1, secondaryAccCol)
				//|| checkSimulatedMove(placements, board, secondaryAccRow + 1, secondaryAccCol) || checkSimulatedMove(placements, board, secondaryAccRow - 1, secondaryAccCol))) {

				int letterValue = 0;
				if (!board.isEmpty(secondaryAccRow, secondaryAccCol)) {
					letterValue = board.getTileValue(secondaryAccRow, secondaryAccCol);
				}
				else {
					letterValue = getSimulatedTileScore(placements, secondaryAccRow, secondaryAccCol);
				}

				for (const auto& placement : placements) {
					if (placement.row == secondaryAccRow && placement.col == secondaryAccCol) {
						Bonus bonusType = board.getTileMultiplier(placement.row, placement.col);
						switch (bonusType) {
						case Bonus::DL: letterValue *= 2; break;
						case Bonus::TL: letterValue *= 3; break;
						case Bonus::DW: secondaryMultiplier *= 2; break;
						case Bonus::TW: secondaryMultiplier *= 3; break;
						default: break;
						}
						break;
					}
				}
				secondaryScore += letterValue;

				secondaryAccRow++;
			}
		}
		//Sekundäre Richtung (horizontal) durchlaufen
		else {
			while (secondaryAccRow < 15 && secondaryAccCol < 15 && secondaryAccRow >= 0 && secondaryAccCol >= 0 && (!board.isEmpty(secondaryAccRow, secondaryAccCol) || checkSimulatedMove(placements, board, secondaryAccRow, secondaryAccCol))) {

				//while (secondaryAccRow < 15 && secondaryAccCol < 15 && secondaryAccRow >= 0 && secondaryAccCol >= 0 && (!board.isEmpty(secondaryAccRow, secondaryAccCol + 1) || !board.isEmpty(secondaryAccRow, secondaryAccCol - 1)
				//|| checkSimulatedMove(placements, board, secondaryAccRow, secondaryAccCol + 1) || checkSimulatedMove(placements, board, secondaryAccRow, secondaryAccCol - 1))) {
				
				int letterValue = 0;

				if (!board.isEmpty(secondaryAccRow, secondaryAccCol)) {
					letterValue = board.getTileValue(secondaryAccRow, secondaryAccCol);
				}
				else {
					letterValue = getSimulatedTileScore(placements, secondaryAccRow, secondaryAccCol);
				}

				for (const auto& placement : placements) {
					if (placement.row == secondaryAccRow && placement.col == secondaryAccCol) {
						Bonus bonusType = board.getTileMultiplier(placement.row, placement.col);
						switch (bonusType) {
						case Bonus::DL: letterValue *= 2; break;
						case Bonus::TL: letterValue *= 3; break;
						case Bonus::DW: secondaryMultiplier *= 2; break;
						case Bonus::TW: secondaryMultiplier *= 3; break;
						default: break;
						}
						break;
					}
				}
				secondaryScore += letterValue;

				secondaryAccCol++;
			}
			totalSecondaryScore += secondaryScore * secondaryMultiplier;
		}

		totalScore = drawScore * wordMultiplier + totalSecondaryScore;

		//Bingo Bonus
		if (placements.size() == 8) {
			totalScore += 50;
		}
	}
	return totalScore;
}

bool Ki::checkSimulatedMove(const std::vector<PlacementKi>& placements, Board& board, int row, int col) {
	return std::any_of(placements.begin(), placements.end(), [row, col](const PlacementKi& placement) {
		return placement.row == row && placement.col == col;
		});
	return 0;
}

int Ki::getSimulatedTileScore(const std::vector<PlacementKi>& placements, int row, int col) {
	for (const auto& placement : placements) {
		if (placement.row == row && placement.col == col) {
			return placement.tile.value;
		}
	}
	return 0;
}