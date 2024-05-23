#include "Manager.h"


void Manager::renderMainMenu() {
	int action = 0;
	bool isContinue = true;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		Util::showLogHeading(getName(), getStaffUsername());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu\n" << ANSI_COLOR_RESET;
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Sales Report <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_ORANGE : "") << "> LOG OUT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ Sales Report ]\n" : ANSI_COLOR_ORANGE "[ LOG OUT ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 2) % 2;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 2;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				//renderSalesReport();
			}
			if (action == 1) {
				action = 0;
				isContinue = false; // Go to previous page
				break;
			}
		}
	} while (isContinue);
}