#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "game.h"
#include "board.h"
#include "player.h"


enum class MainCase
{
	Welcome,
	MainMenu,
	Settings,
	Playing,
	Exit
};

int main() {

	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	/* Stand 1: Lukas
	Board board;
	Player player;
	Bag bag("config_de.txt");

	board.display();
	player.drawTiles(bag);
	player.displayHand();*/

	bool programActive = true;
	bool gameActive = false;
	MainCase activeCase = MainCase::Welcome;
	int input;
	int numberOfPlayers = 1; //standardm‰ﬂig nur eine Person gegen KI

	while (programActive) {

		switch (activeCase) {

		case MainCase::Welcome:
			system("cls");
			std::cout << "Willkommen zu Scrabble mit KI\n";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			activeCase = MainCase::MainMenu;
			break;

		case MainCase::MainMenu:
			system("cls");
			std::cout << "____Hauptmen¸____\n";
			std::cout << "Aktuelle Spielerzahl: " << numberOfPlayers << "\n";
			std::cout << "W‰hlen Sie eine Aktion:\n";
			std::cout << "1: Einstellungen\n";
			std::cout << "2: Spiel starten\n";
			std::cout << "3: Beenden\n";

			std::cin >> input;

			//Eingabe abfangen
			if (input == 1)
			{
				activeCase = MainCase::Settings;
			}
			else if (input == 2)
			{
				activeCase = MainCase::Playing;
			}
			else if (input == 3)
			{
				activeCase = MainCase::Exit;
			}
			break;

		case MainCase::Settings:
			system("cls");
			std::cout << "____Einstellungen____\n";
			std::cout << "Spieleranzahl: ";

			std::cin >> input;

			//Eingabe abfangen
			if ((input > 0) && (input <= 2))
			{
				numberOfPlayers = input;
				activeCase = MainCase::MainMenu;
			}
			else {
				std::cout << "Ung¸ltige Eingabe\n";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
			break;

		case MainCase::Playing:
		{
			Game game(numberOfPlayers, "config_de.txt");

			//Spielablauf
			game.run();

			//Anzeige Ergebnis
			game.scoring();

			std::cout << "\n\nZur¸ck zum Homemen¸ mit '1'\n";
			std::cin >> input;

			if (input == 1) {
				activeCase = MainCase::MainMenu;
			}
			break;
		}

		case MainCase::Exit:
			system("cls");
			std::cout << "Mˆchten Sie das Spiel wirklich beenden?\n";
			std::cout << "1: beenden\n";
			std::cout << "2: abbrechen\n";

			std::cin >> input;

			if (input == 1) {
				return 0;
			}
			else if (input == 2) {
				activeCase = MainCase::MainMenu;
			}
			break;

		default:
			std::cout << "Programmierfehler\n"; //kann nach testen wieder entfernt werden
			break;

		}

	}

};