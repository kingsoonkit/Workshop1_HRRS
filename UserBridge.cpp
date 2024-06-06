#include "UserBridge.h"


// General Functions
void UserBridge::prepareMainMenuAccess(const std::string& username) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT GuestUsername AS Username, UserType FROM Guest"
							" WHERE GuestUsername = ?"
							" UNION"
							" SELECT StaffUsername AS Username, UserType FROM Staff "
							" WHERE StaffUsername = ?");
		db.stmt->setString(1, username);
		db.stmt->setString(2, username);
		db.QueryResult();

		if (db.res->rowsCount() == 1) {
			std::string userType;
			while (db.res->next()) {
				userType = db.res->getString("UserType");
			}
			if (userType == "Guest") {
				db.prepareStatement("SELECT * FROM Guest WHERE GuestUsername = ?");
				db.stmt->setString(1, username);
				db.QueryResult();

				if (db.res->next()) {
					Guest::setICNumber(db.res->getString("ICNumber"));
					Guest::setGuestUsername(db.res->getString("GuestUsername"));
					Guest::setName(db.res->getString("Name"));
					Guest::setPhoneNo(db.res->getString("PhoneNo"));
					Guest::setPassword(db.res->getString("Password"));
					Guest::setUserType(db.res->getString("UserType"));

					Guest::renderMainMenu();
				}
			}
			else if (userType == "Receptionist") {
				db.prepareStatement("SELECT * FROM Staff WHERE StaffUsername = ?");
				db.stmt->setString(1, username);
				db.QueryResult();

				if (db.res->next()) {
					Staff::setStaffUsername(db.res->getString("StaffUsername"));
					Staff::setName(db.res->getString("Name"));
					Staff::setPhoneNo(db.res->getString("PhoneNo"));
					Staff::setPassword(db.res->getString("Password"));
					Staff::setUserType(db.res->getString("UserType"));

					Receptionist::renderMainMenu();
				}
			}
			else if (userType == "Manager") {
				db.prepareStatement("SELECT * FROM Staff WHERE StaffUsername = ?");
				db.stmt->setString(1, username);
				db.QueryResult();

				if (db.res->next()) {
					Staff::setStaffUsername(db.res->getString("StaffUsername"));
					Staff::setName(db.res->getString("Name"));
					Staff::setPhoneNo(db.res->getString("PhoneNo"));
					Staff::setUserType(db.res->getString("UserType"));
					Staff::setPassword(db.res->getString("Password"));

					Manager::renderMainMenu();
				}
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "|\tError accessing user: " << e.what() << std::endl;
	}
}


// DRAW, DISPLAY or RENDER Functions
void UserBridge::renderLoginPrompt() {
	bool isGuest = true;
	bool isStaff = true;
	do {
		system("cls");

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t--------------------------------------------------\n";
		std::cout << "|\t\t\t" << ANSI_COLOR_YELLOW << "LOG IN\n" << ANSI_COLOR_RESET;
		std::cout << "|\t--------------------------------------------------\n";
		Util::showEscInstruction();
		if (!isGuest || !isStaff) {
			std::cout << "|\t" << ANSI_COLOR_RED << "{ Username and password does not match }\n" << ANSI_COLOR_RESET;
		}
		std::cout << "|\n";
		
		const std::string username = Util::parseUsernameInput();
		if (username == __EXIT_CODE__) { break; }
		const std::string password = Util::parsePasswordInput(true, false);
		if (password == __EXIT_CODE__) { break; }

		isGuest = Guest::isPasswordCorrect(username, password);
		isStaff = Staff::isPasswordCorrect(username, password);
		if (isGuest || isStaff) {
			Util::showPositiveMessage("Logging in",true);
			Util::showHorizontalLine("double");
			Util::showRefreshCountdown();
			UserBridge::prepareMainMenuAccess(username);
			break;
		}
	} while (true);
}


void UserBridge::renderStartMenu() {
	int action = 0;
	do {
		system("cls");  // Use the UserBridge command to clear the screen

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "HRRS: Hotel Room Reservation System\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "WELCOME!\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> LOG IN <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_GOLD : "") << "> REGISTER AS Guest <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 2 ? ANSI_COLOR_GOLD : "") << "> REGISTER AS Receptionist <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 3 ? ANSI_COLOR_RED : "") << "> EXIT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ LOGIN ]\n" : action == 1 ? "[ REGISTER AS Guest ]\n" : action == 2 ? "[ REGISTER AS Receptionist ]\n" : ANSI_COLOR_RED "[ EXIT ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 4) % 4;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 4;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				int action = 0;
				UserBridge::renderLoginPrompt();
			}
			if (action == 1) {
				int action = 0;
				Guest::renderRegisterPrompt();
			}
			if (action == 2) {
				int action = 0;
				Receptionist::renderRegisterPrompt();
			}
			if (action == 3) {
				std::cout << ANSI_COLOR_RED;
				std::cout << "-\n";
				std::cout << "-\tEXITING PROGRAM...\n";
				std::cout << "-\n";
				Util::showHorizontalLine("double");
				std::cout << ANSI_COLOR_RESET;
				exit(0);
			}
		}
	} while (true);
}
