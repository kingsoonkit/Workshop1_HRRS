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
	std::cout << "|\tLOGGED AS: " << ANSI_COLOR_YELLOW << name << ANSI_COLOR_RESET << "  |  Username: " << ANSI_COLOR_YELLOW << id << " " << " [" << userType << "]\n" << ANSI_COLOR_RESET;
}


void Util::showInvalidAction() {
	std::cout << "|\t" << ANSI_COLOR_RED << "Invalid action" << ANSI_COLOR_RED;
}
///



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

	// Format the IC number
	input = input.substr(0, 6) + "-" + input.substr(6, 2) + "-" + input.substr(8, 4);
	return input;
}


std::string Util::parsePhoneNumberInput() {
	constexpr int minLength = 15; // Assuming minimum length of 10 digits for a valid phone number
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
		else if ((!input.empty() && input.length() < minLength) || input.length() > minLength) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Incorrect phone number format\n" << ANSI_COLOR_RESET;
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
/////



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


std::vector<std::string> Util::splitString(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}


// Table data

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