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
	for (int i = 0; i < m_players.size(); i++)
	{
		int randomScore = dist(gen);
		m_players[i].setScore(randomScore);
	}

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
			std::cout << m_players[i].getName() << " zieht einen Stein aus dem Beutel..";
			_tickets.push_back(m_bag.drawTile());
			std::this_thread::sleep_for(std::chrono::seconds(5));
			std::cout << _tickets[i].letter << " " << _tickets[i].value << "\n";
		}

		//Auswertung gezogene Spielsteine auf höchsten Buchstabenwert
		int _max = _tickets[0].value;
		m_activePlayer = 0;
		_valid = true;

		for (int i = 1; i < m_numberOfPlayers; i++) {
			if (_max == _tickets[i].value)
			{
				_valid = false; //Wert nicht eindeutig
				continue;
			}
			else {
				if (_max < _tickets[i].value) {
					_max = _tickets[i].value;
					m_activePlayer = i;
					_valid = true; //Wert eindeutig
				}
			}

		}

		//War der _max nicht eindeutig --> Wiederholen
		if (!_valid) {
			std::cout << "\nErgebnis nicht eindeutig, neuer Zug!\n";
			std::this_thread::sleep_for(std::chrono::seconds(5));
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
	std::this_thread::sleep_for(std::chrono::seconds(5));
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
	
	//Spielablauf
	//0 = aussetzen
	if (_input == 0) {
		return 0;
	} 
	
	//1 = Spielsteine tauschen
	if (_input == 1) {
			std::string _inputString;
			std::cout << "____Welche Steine sollen getauscht werden:____\n";
			std::cout << "Eingabe ohne Leerzeichen!\n";
			std::cout << "mit 'Enter' abschließen\n";
			std::cin >> _inputString;

			//Bausteine aus der Hand entfernen und in den Betuel zurücklegen
			for (int i = 0; i < _inputString.size(); i++) {
				std::string letter(1, _inputString[i]);
				m_players[m_activePlayer].putBackTile(m_bag, letter);
			}
			
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
			std::cout << "0 um zu Beenden\n";
			std::getline(std::cin >> std::ws, _inputString);
			_inputString.erase(std::remove(_inputString.begin(), _inputString.end(), ' '), _inputString.end());
			std::stringstream ss(_inputString);
			std::string part;
			std::vector<std::string> partvector;

			if(_inputString == "0") {
				done = true;
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
					int col = colChar - 'A';
				}
				catch (...){
					std::cout << "Ungültige Eingabe, bitte Format beachten: Buchstabe, Buchstabe, Zahl.\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
					continue;
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
				}
				else {
					Tile* tilePtr = new Tile(player->takeTile(letterUpper));
					m_board.placeTile(tilePtr, row, col);
					std::cout << "Baustein: " << letter << " Spalte: " << colChar << " Zeile: " << row + 1 << "\n";
					std::cout << "Nächster Zug...\n";
					std::this_thread::sleep_for(std::chrono::seconds(3));
				}
			}
		}
		m_players[m_activePlayer].drawTiles(m_bag);
		return 0;
	}
	
	//3 = Spiel beenden
	if (_input == 3) {
		m_active = false;
	}
	
}
void checkTile(std::string letter, int col, int row, char colChar) {

	std::string validLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ_";
	bool isLetterValid = letter.length() > 0 && validLetters.find(letterUpper) != std::string::npos;

	if (!isLetterValid) {
		std::cout << "Buchstabe ungültig.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
	else if (row < 0 || row > 14) {
		std::cout << "Reihe ungültig.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
	else if (col < 0 || col > 14) {
		std::cout << "Spalte ungültig.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
	else if (!m_players[m_activePlayer].hasTile(letterUpper)) {
		std::cout << "Du hast diesen Buchstaben nicht!\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
	else if (!m_board.isEmpty(row, col)) {
		std::cout << "Dieses Feld ist bereits belegt!\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
	else {
		Tile* tilePtr = new Tile(&m_players[m_activePlayer]->takeTile(letterUpper));
		m_board.placeTile(tilePtr, row, col);
		std::cout << "Baustein: " << letter << " Spalte: " << colChar << " Zeile: " << row + 1 << "\n";
		std::cout << "Nächster Zug...\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
bool Game::checkDraw() {
	//To-Do Logik für Zugauswertung + Punkteberechnung
	//int als return für die anzahl an erzielten Punken
	//zug gültig = true; (Züge mit 0 und 1 sind immer gültig)
	//zug ungültig = false;

	//Steine richtig gelegt -> eine Linie horizontal oder vertikal + vollständigkeit (keine Lücken)

	//Wortkombninationen, die sich zusätzlich noch ergeben  


	//Gelegtes Wort bekannt/neu? -> Funktion: Wort in Datenbank aufnehmen

	//Auswerten
	if ((m_drawValue == 0) || (m_drawValue == 1)) {
		return true;
	}
	return false;
};
void Game::changeActivePlayer() {
	m_activePlayer += 1;
	if (m_activePlayer >= m_numberOfPlayers) {
		m_activePlayer = 0;
	}
	system("cls");
	std::cout << "____SPIELERWECHSEL____\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));

}
