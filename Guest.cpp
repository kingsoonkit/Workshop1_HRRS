#include "Guest.h"

std::string Guest::guestUsername;
std::string Guest::name;
std::string Guest::ICNumber;
std::string Guest::phoneNo;
std::string Guest::userType;
std::string Guest::password;



// Setters 
void Guest::setGuestUsername(const std::string& guestUsername) {
	Guest::guestUsername = guestUsername;
}
void Guest::setName(const std::string& name) {
	Guest::name = name;
}
void Guest::setICNumber(const std::string& ICNumber) {
	Guest::ICNumber = ICNumber;
}
void Guest::setPhoneNo(const std::string& phoneNo) {
	Guest::phoneNo = phoneNo;
}
void Guest::setUserType(const std::string& userType) {
	Guest::userType = userType;
}
void Guest::setPassword(const std::string& password) {
	Guest::password = password;
}

// Getters
std::string Guest::getGuestUsername() {
	return guestUsername;
}
std::string Guest::getName() {
	return name;
}
std::string Guest::getICNumber() {
	return ICNumber;
}
std::string Guest::getPhoneNo() {
	return phoneNo;
}
std::string Guest::getUserType() {
	return userType;
}
std::string Guest::getPassword() {
	return password;
}



// Functionalities
bool Guest::isUsernameExist(const std::string& username) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT GuestUsername"
							" FROM Guest"
							" WHERE GuestUsername = ?");
		db.stmt->setString(1, username);
		db.QueryResult();

		if (db.res->rowsCount() == 1) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool Guest::isPasswordCorrect(const std::string& username, const std::string& password) {
	try {
		const std::string hashedText = Util::hashText(password);

		DBConnection db;
		db.prepareStatement("SELECT Password"
							" FROM Guest"
							" WHERE GuestUsername = ? AND Password = ?");
		db.stmt->setString(1, username);
		db.stmt->setString(2, hashedText);
		db.QueryResult();


		if (db.res->rowsCount() <= 0) {
			return false;
		}
		else if (db.res->rowsCount() == 1) {
			while (db.res->next())
				if (hashedText != db.res->getString("Password"))
					return false;
		}

	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}

	return true;
}



// Rendering
void Guest::renderRegisterPrompt() {
	do {
		system("cls");

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Register as Guest\n" << ANSI_COLOR_RESET;
		std::cout << "|\t(Enter \"esc\" to return to previous page)\n";
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";

		std::string tempArr[5];

		tempArr[0] = Util::parseUsernameInput();
		if (tempArr[0] == __EXIT_CODE__) { break; }
		if (Guest::isUsernameExist(tempArr[0])) {
			std::cout << "|\t" << ANSI_COLOR_RED << "This username is already taken. Try another one\n" << ANSI_COLOR_RESET;
		}
		else {
			std::cout << "|\t" << ANSI_COLOR_GREEN << "[OK]\n" << ANSI_COLOR_RESET;
			std::cout << "|\t-----------------------------------\n";
		}

		tempArr[1] = Util::parseNameInput();
		if (tempArr[1] == __EXIT_CODE__) { break; }

		tempArr[2] = Util::parseICNumberInput();
		if (tempArr[2] == __EXIT_CODE__) { break; }

		tempArr[3] = Util::parsePhoneNumberInput();
		if (tempArr[3] == __EXIT_CODE__) { break; }

		setUserType("Guest");

		tempArr[4] = Util::hashText(Util::parsePasswordInput(false, true));
		if (tempArr[4] == __EXIT_CODE__) { break; }

		try {
			DBConnection db;
			db.prepareStatement("INSERT INTO guest (GuestUsername, Name, ICNumber, PhoneNo, UserType, Password) VALUES (?,?,?,?,?,?)");
			db.stmt->setString(1, tempArr[0]);
			db.stmt->setString(2, tempArr[1]);
			db.stmt->setString(3, tempArr[2]);
			db.stmt->setString(4, tempArr[3]);
			db.stmt->setString(5, getUserType());
			db.stmt->setString(6, tempArr[4]);
			db.QueryStatement();

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_GREEN << "[ Registration Complete! ]\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;

	} while (true);
}


void Guest::renderReservationMenu() {
	
}

void Guest::renderBookingHistory(const std::string& GuestUsername) {
	
}


void Guest::renderMainMenu() {
	int action = 0;
	bool isContinue = true;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		Util::showLogHeading(getName(), getGuestUsername(), getUserType());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu\n" << ANSI_COLOR_RESET;
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Make/Cancel Reservation <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_GOLD : "") << "> Booking History <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 2 ? ANSI_COLOR_ORANGE : "") << "> LOG OUT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ Make/Cancel Reservation ]\n" : action == 1 ? "[ Booking History ]\n" :  ANSI_COLOR_ORANGE "[ LOG OUT ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 3) % 3;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 3;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				renderReservationMenu();
			}
			if (action == 1) {
				renderBookingHistory(getGuestUsername());
			}
			if (action == 2) {
				action = 0;
				isContinue = false; // Go to previous page
				break;
			}
		}
	} while (isContinue);
}


