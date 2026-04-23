//Wörter Tausch am Anfang ->erster Zug dann kein erster Zug mehr
//Absturz bei Wortlegen aber kein Wort legen und gleich zurücknehmen drücken ;
//Komplette Hand tauschen
#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <vector>

#include "game.h"

//Konstuktor
Game::Game(int numberOfPlayers, std::string language)
	: m_numberOfPlayers(numberOfPlayers),
	m_bag(language), m_active(true)
{
	std::string input;
	std::cin.ignore(); //vorheriges enter abfangen

	//Name für die Spieler vergeben
	for (int i = 1; i <= m_numberOfPlayers; i++) {

		if (i == 1)
			system("cls");

		//Name für den i. Player eingeben
		std::cout << "Name Spieler " << i << ":";

		std::getline(std::cin, input); //liest komplette Zeile, cin nur bis zum ersten Leerzeichen
		std::cout << "\n";

		m_players.emplace_back(Player(input));

	}

	//jeden Spieler 8 Buchstaben aus dem Beutel ziehen lassen
	for (int i = 0; i < m_numberOfPlayers; i++) {
		m_players[i].drawTiles(m_bag);
	}

}

void Game::run() {

	//To-Do TEST!! Scoring testen --> random Werte den Spielern zuweisen
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 200);
	//
	//for (int i = 0; i < m_players.size(); i++)
	//{
	//	int randomScore = dist(gen);
	//	m_players[i].setScore(randomScore);
	//}

	//Ablauf: Start ermitteln -> Zug -> Check -> Spielerwechsel->
	//                           |                               |
	//                           |_______________________________|

	while (m_active) {
		switch (m_runCase)
		{
		case RunCase::GetFirstPlayer: {
			getFirstPlayer();
			m_runCase = RunCase::Draw;
			break;
		}

		case RunCase::Draw: {
			m_drawValue = draw();
			m_runCase = RunCase::CheckDraw;
			break;
		}
		case RunCase::CheckDraw: {
			if (checkDraw()) {
				DrawScore();
				m_runCase = RunCase::ChangeActivePlayer;
				break;
			}
			m_runCase = RunCase::Draw;
			break;
		}
		case RunCase::ChangeActivePlayer: {
			changeActivePlayer();
			m_runCase = RunCase::Draw;
			break;
		}

		default:
			m_active = false;
			break;
		}
	}
}

void Game::scoring() {

	system("cls");

	m_board.display();

	//Auswertung möglich
	if (m_players.empty())
	{
		std::cout << "Keine Spieler vorhanden\n";
		return;
	}

	//Ausgabe aller Werte
	for (int i = 0; i < m_players.size(); i++) {
		std::cout << "Punkte " << m_players[i].getName() << ": " << m_players[i].getScore() << "\n";
	}

	//Gewinner ermitteln
	std::string winner = m_players[0].getName();
	int max = m_players[0].getScore();
	for (int i = 1; i < m_players.size(); i++) {
		if (max < m_players[i].getScore())
		{
			max = m_players[i].getScore();
			winner = m_players[i].getName();
		}

	}
	std::cout << "Der Gewinner lautet: " << winner << "\n";
}
void Game::getFirstPlayer() {

	bool _valid = false;

	//Jeder Spieler zieht einen Baustein
	std::vector <Tile> _tickets;

	while (!_valid) {

		//Tickets initialisieren
		_tickets.clear();

		//Jeder Spieler zieht einen Stein
		std::cout << "Wer beginnt? Der Startspieler wird ausgelost…\n";
		for (int i = 0; i < m_numberOfPlayers; i++) {
			std::cout << m_players[i].getName() << " zieht einen Stein aus dem Beutel…		";
			_tickets.push_back(m_bag.drawTile());
			std::this_thread::sleep_for(std::chrono::seconds(3));
			std::cout << _tickets[i].letter << " " << _tickets[i].value << "\n";
		}

		//Auswertung gezogene Spielsteine auf höchsten Buchstabenwert
		int _min = _tickets[0].letter[0];
		m_activePlayer = 0;
		_valid = true;

		for (int i = 1; i < m_numberOfPlayers; i++) {
			if (_min == _tickets[i].letter[0])
			{
				_valid = false; //Wert nicht eindeutig
				continue;
			}
			else if (_tickets[i].letter[0] < _min) {
					_min = _tickets[i].letter[0];
					m_activePlayer = i;
					_valid = true; //Wert eindeutig
				}
		}

		//War der _max nicht eindeutig --> Wiederholen
		if (!_valid) {
			std::cout << "\nErgebnis nicht eindeutig, neuer Zug!\n";
			std::this_thread::sleep_for(std::chrono::seconds(3));
			system("cls");
		}

		//Gezogene Steine zurücklegen
		for (int i = 0; i < m_numberOfPlayers; i++) {
			m_bag.putBackTile(_tickets[i]);
		}

		//Sack mischen 
		m_bag.shuffle();
	}

	//Ausgabe des aktiven Spieler
	std::cout << "\n" << m_players[m_activePlayer].getName() << " startet mit dem ersten Zug. \n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

int Game::draw() {

	int _input = 0;

	//To-Do Logik für Zug schreiben
	//int als return für die unterschiedlichen Möglichkeiten für einen Zug
	// 0 = aussetzen
	// 1 = Spielsteine tauschen
	// 2 = regulärer Zug
	//-1 = Spiel beenden

	//Anzeige UI
	system("cls");
	m_board.display();
	m_players[m_activePlayer].displayHand();

	std::cout << "____Aktion wählen____\n";
	std::cout << "0 = aussetzen\n";
	std::cout << "1 = Spielsteine tauschen\n";
	std::cout << "2 = Wort legen\n";
	std::cout << "3 = Spiel beenden\n";

	std::cin >> _input;

	if (std::cin.fail() || _input < 0 || _input > 3) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Ungültige Eingabe, bitte in 3 Sekunden erneut versuchen.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return draw();
	}
	
	//Spielablauf
	//0 = aussetzen
	if (_input == 0) {
		return 0;
	} 
	
	//1 = Spielsteine tauschen
	if (_input == 1) {
			std::string _inputString;
			std::cout << "____Welche Steine sollen getauscht werden:____\n";
			std::cout << "0 für alle Steine tauschen\n";
			std::cout << "Eingabe getrennt durch ','!\n";
			std::cout << "mit 'Enter' abschließen\n";

			//Einlesen, Leerzeichen entfernen, Aufteilen in Vektor 
			std::getline(std::cin >> std::ws, _inputString);
			_inputString.erase(std::remove(_inputString.begin(), _inputString.end(), ' '), _inputString.end());
			std::stringstream ss(_inputString);
			std::string part;
			std::vector<std::string> partvector;

			while (std::getline(ss, part, ',')) {
				if (!part.empty()) {
					partvector.push_back(part);
				}
			}
			
			for(int i = 0; i < partvector.size(); i++) {
				std::string letterUpper = turnToUpper(partvector[i]);
				if (!m_players[m_activePlayer].hasTile(letterUpper)) {
					std::cout << "Du hast den Buchstaben " << partvector[i] << " nicht!\n";
				}
				else {
					m_players[m_activePlayer].putBackTile(m_bag, letterUpper);
				}
			}
			
			//Bausteine aus der Hand entfernen und in den Betuel zurücklegen
			//for (int i = 0; i < _inputString.size(); i++) {
			//	std::string letter(1, _inputString[i]);
			//	m_players[m_activePlayer].putBackTile(m_bag, letter);
			//}
			
			//Beutel mischen
			m_bag.shuffle();

			//neue Bausteine ziehen
			m_players[m_activePlayer].drawTiles(m_bag);
			std::cout << "Folgende neue Hand: \n";
			m_players[m_activePlayer].displayHand();
			std::this_thread::sleep_for(std::chrono::seconds(3));
		
			return 0;
	}

	//2 = Zug konfigurieren
	if (_input == 2) {
		bool done = false;
		Player* player = &m_players[m_activePlayer];

		while (!done) {
			system("cls");
			m_board.display();
			m_players[m_activePlayer].displayHand();
			std::string _inputString;
			std::cout << "Zug konfigurieren im Format: Buchstabe,Spalte,Zeile\n";
			std::cout << "0 um zu Bestätigen; 1 um den letzten Stein zurückzunehmen\n";
			std::getline(std::cin >> std::ws, _inputString);
			_inputString.erase(std::remove(_inputString.begin(), _inputString.end(), ' '), _inputString.end());
			std::stringstream ss(_inputString);
			std::string part;
			std::vector<std::string> partvector;

			//Zug beenden
			if(_inputString == "0") {
				if (m_drawPlacements.empty()) {
					std::cout << "Du hast keinen Baustein gelegt!\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				else {
					done = true;
					continue;
				}
				
			}
			//Zurücknehmen des letzten gelegten Bausteins
			if (_inputString == "1") {
				if(m_drawPlacements.empty()) {
					std::cout << "Keine Bausteine zum Zurücknehmen!\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				int row = m_drawPlacements.back().row;
				int col = m_drawPlacements.back().col;
				Tile* tile = m_board.getTile(row, col);
				m_board.clearTile(row, col);
				m_players[m_activePlayer].giveTile(tile);
				m_drawPlacements.pop_back();
				continue;
			}

			while (std::getline(ss, part, ',')) {
				partvector.push_back(part);
			}
			if (partvector.size() != 3) {
				std::cout << "Ungültige Eingabe, bitte erneut versuchen.\n";
				std::this_thread::sleep_for(std::chrono::seconds(3));
				continue;
			}
			else {
				std::string letter = partvector[0];
				std::string colStr = partvector[1];
				std::string rowStr = partvector[2];

				int row = -1;
				int col = -1;
				char colChar;

				//Umrechnen für weitere Logik
				try {
					row = std::stoi(partvector[2]) - 1;

					if (std::isdigit(colStr[0])) {
						throw std::invalid_argument("Spalte muss ein Buchstabe sein");
					}
					colChar = std::toupper(colStr[0]);
					col = colChar - 'A';
				}
				catch (...){
					std::cout << "Ungültige Eingabe, bitte Format beachten: Buchstabe, Buchstabe, Zahl.\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}

				std::string letterUpper = turnToUpper(letter);

				std::string validLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ_";
				bool isLetterValid = letter.length() > 0 && validLetters.find(letterUpper) != std::string::npos;

				if (!isLetterValid) {
					std::cout << "Buchstabe ungültig.\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				else if (row < 0 || row > 14) {
					std::cout << "Reihe ungültig.\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				else if (col < 0 || col > 14) {
					std::cout << "Spalte ungültig.\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				else if (!m_players[m_activePlayer].hasTile(letterUpper)) {
					std::cout << "Du hast diesen Buchstaben nicht!\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				}
				else if (!m_board.isEmpty(row, col) ) {
					std::cout << "Dieses Feld ist bereits belegt!\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
				} else {
					if(letterUpper == "_") {
						std::string inputLetter;
						std::cout << "Welchen Buchstaben soll der Joker annehmen? (A-Z, Ä, Ö, Ü)\n";
						std::cin >> inputLetter;
						letterUpper = turnToUpper(inputLetter);
						Tile* tilePtr = new Tile(player->takeTile("_"));
						tilePtr->letter = letterUpper;
						m_board.placeTile(tilePtr, row, col);
						std::cout << "Baustein: " << letter << " Spalte: " << colChar << " Zeile: " << row + 1 << "\n";
						m_drawPlacements.push_back({ row, col });
					}
					else {
						Tile* tilePtr = new Tile(player->takeTile(letterUpper));
						m_board.placeTile(tilePtr, row, col);
						std::cout << "Baustein: " << letter << " Spalte: " << colChar << " Zeile: " << row + 1 << "\n";
						m_drawPlacements.push_back({ row, col });
						std::cout << "Nächster Zug...\n";
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
				}
			}
		}
		return 0;
	}
	
	//3 = Spiel beenden
	if (_input == 3) {
		m_active = false;
	}
}

std::string Game::turnToUpper(std::string letter) {
	if (letter.length() == 0) {
		return "";
	}
	std::string letterUpper;
	if (letter == "_") {
		letterUpper = "_";
	}
	else if (letter == "Ä" || letter == "ä") {
		letterUpper = "Ä";
	}
	else if (letter == "Ö" || letter == "ö") {
		letterUpper = "Ö";
	}
	else if (letter == "Ü" || letter == "ü") {
		letterUpper = "Ü";
	}
	else {
		letterUpper = std::toupper(letter[0]);
	}
	return letterUpper;
}

bool Game::checkDraw() {
	if (m_drawPlacements.empty()) {
		return true;
	}
	
	if (isFirstTurn) {
		bool hasCenter = false;
		for (size_t i = 0; i < m_drawPlacements.size(); i++) {
			if (m_drawPlacements[i].row == 7 && m_drawPlacements[i].col == 7) {
				hasCenter = true;
				break;
			}
		}
		if (m_drawPlacements.size() != 0 && !hasCenter) {
			returnTilesToPlayer();
			m_drawPlacements.clear();
			std::cout << "Der erste Zug muss den Mittelpunkt des Spielfelds beinhalten!\n";
			std::this_thread::sleep_for(std::chrono::seconds(3));
			return false;
		}
	}

	bool total = isInLine() && isConnected() && hasAdjacent();
	
	if (!total) {
		returnTilesToPlayer();
		m_drawPlacements.clear();
		return false;
	}
	return true;

	//Wortkombninationen, die sich zusätzlich noch ergeben  


	//Gelegtes Wort bekannt/neu? -> Funktion: Wort in Datenbank aufnehmen
	
}

bool Game::isInLine() {
	bool horizontal = true;
	bool vertical = true;

	if (m_drawPlacements.size() <= 1) {
		return true;
	}
	
	for(size_t i = 1; i < m_drawPlacements.size(); i++) {
		if (m_drawPlacements[i].row != m_drawPlacements[0].row) {
			horizontal = false;
		}
		if (m_drawPlacements[i].col != m_drawPlacements[0].col) {
			vertical = false;
		}
	}
	if(!(horizontal || vertical)) {
		std::cout << "Die Bausteine müssen in einer Linie liegen!\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	return horizontal || vertical;
}

bool Game::isConnected() {
	int minRow = -1;
	int maxRow = -1;
	int minCol = -1;
	int maxCol = -1;
	bool isConnected = true;

	for (size_t i = 0; i < m_drawPlacements.size(); i++) {
		if (minRow == -1 || m_drawPlacements[i].row < minRow) {
			minRow = m_drawPlacements[i].row;
		}
		if (maxRow == -1 || m_drawPlacements[i].row > maxRow) {
			maxRow = m_drawPlacements[i].row;
		}
		if (minCol == -1 || m_drawPlacements[i].col < minCol) {
			minCol = m_drawPlacements[i].col;
		}
		if (maxCol == -1 || m_drawPlacements[i].col > maxCol) {
			maxCol = m_drawPlacements[i].col;
		}
	}

	if (minCol == maxCol) {
		for (int i = minRow; i <= maxRow; i++) {
			if (m_board.isEmpty(i, minCol)) {
				isConnected = false;
				std::cout << "Lücke bei: " << static_cast<char>('A' + minCol) << "," << i + 1 << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}
	}
	else {
		for (int i = minCol; i <= maxCol; i++) {
			if (m_board.isEmpty(minRow, i)) {
				isConnected = false;
				std::cout << "Lücke bei: " << static_cast<char>('A' + i) << "," << minRow + 1 << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}
	}
	return isConnected;
}

bool Game::hasAdjacent() {
	bool hasAdjacent = false;

	if (m_drawPlacements.empty() || isFirstTurn == true) {
		return hasAdjacent = true;
	}

	for (size_t i = 0; i < m_drawPlacements.size(); i++) {
		int row = m_drawPlacements[i].row;
		int col = m_drawPlacements[i].col;

		if (row != 0 && !m_board.isEmpty(row - 1, col) && !std::any_of(m_drawPlacements.begin(), m_drawPlacements.end(), [row, col](const Placement& p) { return p.row == row - 1 && p.col == col; })) {
			return hasAdjacent = true;
		} else if (row != 14 && !m_board.isEmpty(row + 1, col) && !std::any_of(m_drawPlacements.begin(), m_drawPlacements.end(), [row, col](const Placement& p) { return p.row == row + 1 && p.col == col; })) {
			return hasAdjacent = true;
		} else if (col != 0 && !m_board.isEmpty(row, col - 1) && !std::any_of(m_drawPlacements.begin(), m_drawPlacements.end(), [row, col](const Placement& p) { return p.row == row && p.col == col - 1; })) {
			return hasAdjacent = true;
		} else if (col != 14 && !m_board.isEmpty(row, col + 1) && !std::any_of(m_drawPlacements.begin(), m_drawPlacements.end(), [row, col](const Placement& p) { return p.row == row && p.col == col + 1; })) {
			return hasAdjacent = true;
		}
	}
	isFirstTurn = false;
	std::cout << "Die Bausteine müssen an bereits gelegene Bausteine angrenzen!\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return hasAdjacent = false;
}

int Game::DrawScore() {
	int oldScore = m_players[m_activePlayer].getScore();
	int drawScore = 0;
	int wordMultiplier = 1;
	bool isHorizontal = true;

	if (m_drawPlacements.empty()) {
		return m_players[m_activePlayer].getScore();
	}

	if (m_drawPlacements.size() > 1) {
		isHorizontal = m_drawPlacements[0].row == m_drawPlacements[1].row;
	}

	int startRow = m_drawPlacements[0].row;
	int startCol = m_drawPlacements[0].col;

	if (isHorizontal) {
		while (startCol > 0 && !m_board.isEmpty(startRow, startCol - 1)) {
			startCol--;
		}
	} else {
		while (startRow > 0 && !m_board.isEmpty(startRow - 1, startCol)) {
			startRow--;
		}
	}

	int accRow = startRow;
	int accCol = startCol;

	if(checkDraw()) {
		while (accRow < 15 && accCol < 15 && !m_board.isEmpty(accRow, accCol)) {
			int letterValue = m_board.getTileValue(accRow, accCol);
			for(const auto& placement : m_drawPlacements) {
				if (placement.row == accRow && placement.col == accCol) {
					Bonus bonusType = m_board.getTileMultiplier(placement.row, placement.col);
					switch (bonusType) {
						case Bonus::DL: letterValue *= 2; break;
						case Bonus::TL: letterValue *= 3; break;
						case Bonus::DW: wordMultiplier *= 2; break;
						case Bonus::TW: wordMultiplier *= 3; break;
						default: break;
					}
				}
			}
			drawScore += letterValue;

			if (isHorizontal) accCol++; else accRow++;
		}
	}
	int totalScore = drawScore * wordMultiplier;

	if (m_drawPlacements.size() == 8) {
		totalScore += 50;
	}

	m_players[m_activePlayer].setScore(oldScore + totalScore);
	std::cout << "Punkte für diesen Zug: " << totalScore - m_players[m_activePlayer].getScore() << "\n";
	m_drawPlacements.clear();
	return m_players[m_activePlayer].getScore();
}

void Game::returnTilesToPlayer() {
	for (size_t i = 0; i < m_drawPlacements.size(); i++) {
		Tile* tile = m_board.getTile(m_drawPlacements[i].row, m_drawPlacements[i].col);
		m_board.clearTile(m_drawPlacements[i].row, m_drawPlacements[i].col);
		m_players[m_activePlayer].giveTile(tile);
		m_board.clearTile(m_drawPlacements[i].row, m_drawPlacements[i].col);
	}
	m_drawPlacements.clear();
}

void Game::changeActivePlayer() {
	//isFirstTurn = false;
	m_players[m_activePlayer].drawTiles(m_bag);
	m_activePlayer += 1;
	if (m_activePlayer >= m_numberOfPlayers) {
		m_activePlayer = 0;
	}
	system("cls");
	std::cout << "____SPIELERWECHSEL____\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
