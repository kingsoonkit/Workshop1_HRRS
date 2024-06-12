#include "Manager.h"


void Manager::renderRoomSalesReport() {
	DBConnection db;
	tabulate::Table table;
	std::vector<std::string> data;
	bool isExiting = false;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Room Sales Report\n" << ANSI_COLOR_RESET;
		std::cout << "|\t------------------------\n";
		std::cout << "|\n";
		Util::showEscInstruction();
		std::cout << "|\tView top sales for each room type by year-month\n";

		std::string year = Util::parseYearInput();
		if (year == __EXIT_CODE__) { break; }

		std::string month = Util::parseMonthInput();
		if (month == __EXIT_CODE__) { break; }

		table = DBUtil::getTable_MonthlyRoomSales(year, month);
		DBUtil::applyMonthlyBookingSales_To(data, year, month);

		do {
			if (table.size() > 0) {
				std::cout << "|\n";
				std::cout << "|\n";
				std::cout << "|\t" << ANSI_COLOR_YELLOW << "TOP ROOM SALES of " << ANSI_COLOR_GOLD << year << "-" << month << ":\n" << ANSI_COLOR_RESET;
				std::cout << table << "\n";
				std::cout << "|\t Total Booking(s) : " << ANSI_COLOR_GOLD << data[0] << ANSI_COLOR_RESET << "\n";
				std::cout << "|\t Total Sales      : " << ANSI_COLOR_GOLD << data[1] << ANSI_COLOR_RESET << "\n";
				std::cout << "|\t-----------------------------------\n";
				std::cout << "|\n";
			}
			else {
				std::cout << "|\n";
				std::cout << "|\n";
				Util::showPassiveMessage("There are 0 sales in this period");
				std::cout << "|\t" << ANSI_COLOR_ORANGE << "Period: " << year << "-" << month << ":\n" << ANSI_COLOR_RESET;
				std::cout << "|\n";
				std::cout << "|\n";
				Util::showHorizontalLine("single");
				std::cout << "|\n";
			}
			Util::showPressAnyKeyToContinue();
			break;
		} while (true);
	} while (true);
}



void Manager::renderMainMenu() {
	int action = 0;
	bool isContinue = true;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		Util::showLogHeading(getStaffUsername(), getUserType());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu\n" << ANSI_COLOR_RESET;
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Room Sales Report <\n" << ANSI_COLOR_RESET;
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
				renderRoomSalesReport();
			}
			if (action == 1) {
				isContinue = false; // Go to previous page
				Util::showRefreshCountdown();
				break;
			}
		}
	} while (isContinue);
}