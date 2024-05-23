#include "Util.h"


// UI Related
void Util::showHorizontalLine(const std::string& lines) {
	if (lines == "single") {
		std::cout << "| ----------------------------------------------------------------------------------------------------------------------\n";
	}
	else if (lines == "double") {
		std::cout << "| ======================================================================================================================\n";
	}
}


void Util::showRefreshCountdown() {
	const int seconds = 3;
	std::cout << "|\tRefreshing in ";
	for (int i = seconds; i > 0; --i) {
		std::cout << i << "... ";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second
	}
}


void Util::showLogHeading(const std::string& name, const std::string& id, const std::string& userType) {
	std::cout << "|\tLOGGED AS: " << ANSI_COLOR_YELLOW << name << " [" << userType << "]" << ANSI_COLOR_RESET << "  |  Username: " << ANSI_COLOR_YELLOW << id << '\n' << ANSI_COLOR_RESET;
}

void Util::showLogHeading(const std::string& name, const std::string& userType) {
	std::cout << "|\tLOGGED AS: " << ANSI_COLOR_YELLOW << name << " [" << userType << "]\n" << ANSI_COLOR_RESET;
}

std::string Util::writeTodayDate(const bool addNewline, int tabCount = 8) {
	std::string tab_str;
	for (int i = 0; i < tabCount; i++) {
		tab_str += "\t";
	}
	return ANSI_COLOR_RESET + tab_str + "Today: " + ANSI_COLOR_YELLOW + Util::getCurrentDate() + (addNewline ? "\n" : "") + ANSI_COLOR_RESET;
}

void Util::showInvalidAction() {
	std::cout << "|\t" << ANSI_COLOR_RED << "{ Invalid action }\n" << ANSI_COLOR_RESET;
}

void Util::showPositiveMessage(std::string message) {
	std::cout << "|\n";
	std::cout << "|\t" << ANSI_COLOR_GREEN << "[ "<< message <<" ]\n" << ANSI_COLOR_RESET;
	std::cout << "|\n";
}
////



// Input Parsing (includes cin and input validation, hence "parsing" //
std::string Util::parseUsernameInput() {
	std::string input;
	constexpr int maxLength = 30;

	while (true) {
		std::cout << "|\tUsername\t: "; std::getline(std::cin, input);

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}


		// Validation
		const bool inputFormatIsIncorrect = !std::all_of(input.begin(), input.end(), [](char c) { return isalnum(c) || c == '_' || c == '.'; });
		if (inputFormatIsIncorrect || input.empty() || input.length() > maxLength) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Username must be between 1 and 30 characters long and can\n" << ANSI_COLOR_RESET;
			std::cout << "|\t" << ANSI_COLOR_RED << "only contain letters(A - Z, a - z), numbers (0-9), underscores (_), and periods (.)\n" << ANSI_COLOR_RESET;
		}
		else {
			break;
		}
		std::cout << "|\n";
	}
	return input;
}


std::string Util::parseNameInput() {
	constexpr int maxLength = 255; // Follows database
	std::string input;

	while (true) {
		std::cout << "|\tName\t\t: ";
		std::getline(std::cin, input);

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Name must be filled\n" << ANSI_COLOR_RESET;
		}
		else if (input.length() > maxLength) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Name is too long\n" << ANSI_COLOR_RESET;
		}
		else if (!std::all_of(input.begin(), input.end(), [](char c) { return std::isalpha(c) || std::isspace(c); })) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Name must only contain letters (A-Z, a-z)\n" << ANSI_COLOR_RESET;
		}
		else {
			break;
		}
		std::cout << "|\n";
	}
	return input;
}


std::string Util::parseICNumberInput() {
	constexpr int exactLength = 12;
	std::string input;

	while (true) {
		std::cout << "|\tIC number\t: ";
		std::getline(std::cin, input);

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			std::cout << "|\t" << ANSI_COLOR_RED << "IC Number must be filled\n" << ANSI_COLOR_RESET;
		}
		else if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Please enter digits only (no dashes \"-\")\n" << ANSI_COLOR_RESET;
		}
		else if (input.length() != exactLength) {
			std::cout << "|\t" << ANSI_COLOR_RED << "IC number has incorrect format\n" << ANSI_COLOR_RESET;
		}
		else {
			break;
		}
		std::cout << "|\n";
	}

	return input;
}


std::string Util::parsePhoneNumberInput() {
	constexpr int minLength = 9; // Assuming minimum length of 10 digits for a valid phone number
	std::string input;

	while (true) {
		std::cout << "|\tPhone number (optional)\t: ";
		std::getline(std::cin, input);

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Please enter numbers only (no dashes)\n" << ANSI_COLOR_RESET;
		}
		else if ((!input.empty() && (input.length() < minLength || input.length() > 20))) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Incorrect phone number length\n" << ANSI_COLOR_RESET;
		}
		else {
			break;
		}
		std::cout << "|\n";
	}
	return input;
}


std::string Util::parsePasswordInput(const bool& hide, const bool& isReg) {
	constexpr char BACKSPACE = 8;
	constexpr char ENTER = 13;
	const std::regex pattern("(?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,}");
	std::string input;

	while (true) {
		std::cout << "|\tPassword\t: ";

		// Input style (hide or unhide)
		if (!hide) {
			std::getline(std::cin, input);
		}
		else {
			char ch;
			int count = 0;
			while ((ch = _getch()) != ENTER) {
				if (ch == BACKSPACE) {
					if (!input.empty() && count > 0) {
						count--;
						input.pop_back();
						std::cout << "\b \b"; // Move cursor back, erase character, move cursor back again
					}
				}
				else {
					count++;
					input.push_back(ch);
					std::cout << '*'; // Print asterisk instead of the typed character
				}
			}
			std::cout << std::endl;
		}

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Password must be filled\n" << ANSI_COLOR_RESET;
		}
		else if (isReg && !std::regex_match(input, pattern)) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Password must be at least 8 characters long and contain at least 1 digit,\n" << ANSI_COLOR_RESET;
			std::cout << "|\t" << ANSI_COLOR_RED << "1 uppercase letter, and 1 lowercase letter\n" << ANSI_COLOR_RESET;
		}
		else {
			break; // Exit loop if all conditions are met
		}
		std::cout << "|\n";
	}

	return input;
}


std::string Util::parseTextInput() {
	std::regex pattern("^[a-zA-Z0-9]+(?:-[a-zA-Z0-9]+)*$");
	std::string input;

	do {
		std::cout << "|\tAction-#: ";
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		if (input.empty()) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Must not be empty\n" << ANSI_COLOR_RESET;
		}
		else if (!std::regex_match(input, pattern)) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Can only contain numbers, letters or dashes \"-\"\n" << ANSI_COLOR_RESET;
		}
		else {
			break; // Exit loop if all conditions are met
		}
		std::cout << "|\n";
	} while (true);
	return input;
}


std::string Util::parseDateInput() {
	std::string input;

	do {
		std::cout << "|\tIC number\t: ";
		std::getline(std::cin, input);

		
	} while (true);
}
/////


////


// Miscellaneous functions
std::string Util::getCurrentDate() {
	// Get current time
	std::time_t currentTime;
	std::time(&currentTime); // Get current time in seconds since epoch

	// Convert to local time
	std::tm localTime;
	localtime_s(&localTime, &currentTime); // Convert to local time

	// Format the date as dd/mm/yyyy
	std::ostringstream oss;
	oss << std::setfill('0');
	oss << std::setw(2) << localTime.tm_mday << "/" // Day
		<< std::setw(2) << (localTime.tm_mon + 1) << "/" // Month (zero-based)
		<< (localTime.tm_year + 1900); // Year (years since 1900)

	return oss.str();
}


std::string Util::hashText(const std::string& input) {
	std::string ciphertext;
	for (int i = 0; i < input.length(); i++) {
		int asciiDecimal = input[i] + ((i ^ 2 + 1) * input.length());
		while (asciiDecimal < 33) {
			asciiDecimal = asciiDecimal + asciiDecimal + 1;
		}
		while (asciiDecimal > 125) {
			asciiDecimal = (asciiDecimal % 125) + 32;
		}
		ciphertext += static_cast<char>(asciiDecimal);
	}
	return ciphertext;
}


std::string Util::truncateDecimal(std::string input) {
	// Find the position of the decimal point
	size_t decimalPos = input.find('.');

	// If there's no decimal point or it's already at the end, return the input as is
	if (decimalPos == std::string::npos || decimalPos == input.size() - 1) {
		return input;
	}

	// Truncate after two digits following the decimal point
	size_t truncatePos = decimalPos + 3; // Two digits after the decimal point
	if (truncatePos < input.size()) {
		input.erase(truncatePos);
	}
	return input;
}


std::vector<std::string> Util::split(const std::string& str, char delimiter) {
	std::vector<std::string> result;
	std::string item;
	std::stringstream ss(str);
	while (std::getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}


// Table data
bool Util::isRoomNumberExist(const std::vector<std::string>& rooms, DBConnection& db){
	for (int i = 0; i < rooms.size(); ++i) {
		int count = 0;
		bool roomExists = false;
		db.res->beforeFirst();
		while (db.res->next()) {
			count++;
			std::cout << count << db.res->getString("RoomNumber") << rooms[i] << "\n";
			if (rooms[i] == db.res->getString("RoomNumber")) {
				roomExists = true;
				break;
			}
		}
		if (!roomExists) {
			return false; // If any room doesn't exist, return false
		}
	}
	return true;
}



bool Util::updateRoomStatuses() {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE room r"
							" LEFT JOIN booking b ON r.RoomNumber = b.RoomNumber AND b.StartDate = CURRENT_DATE"
							" SET r.RoomStatus ="
							" CASE"
							" WHEN b.BookingStatus = 'Checked-in' && (CURRENT_DATE BETWEEN b.StartDate AND b.EndDate) THEN 'Occupied'"
							" WHEN b.BookingStatus = 'Reserved' THEN 'Reserved'"
							" WHEN b.BookingStatus = 'Checked-out' OR b.BookingStatus = 'Cancelled' OR b.BookingStatus IS NULL THEN 'Available'"
							" END;");
		db.QueryStatement();
		db.~DBConnection();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}
//////