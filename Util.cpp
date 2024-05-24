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

void Util::showInputCancelInstruction() {
	std::cout << "|\t(Enter \"" << ANSI_COLOR_ORANGE << "esc" << ANSI_COLOR_RESET << "\" to return to previous page)\n";
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

void Util::showNegativeMessage(std::string message) {
	std::cout << "|\t" << ANSI_COLOR_RED << message << "\n" << ANSI_COLOR_RESET;
}
////



// Input Parsing (includes cin and input validation, hence "parsing" //
std::string Util::parseUsernameInput() {
	std::string input;
	constexpr int maxLength = 30;

	while (true) {
		std::cout << "|\tUsername\t: "; 
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}


		// Validation
		const bool inputFormatIsIncorrect = !std::all_of(input.begin(), input.end(), [](char c) { return isalnum(c) || c == '_' || c == '.'; });
		if (inputFormatIsIncorrect || input.empty() || input.length() > maxLength) {
			showNegativeMessage("Username must be between 1 and 30 characters long and can");
			showNegativeMessage("only contain letters(A - Z, a - z), numbers (0-9), underscores (_), and periods (.)");
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
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			showNegativeMessage("Name must be filled");
		}
		else if (input.length() > maxLength) {
			showNegativeMessage("Name is too long");
		}
		else if (!std::all_of(input.begin(), input.end(), [](char c) { return std::isalpha(c) || std::isspace(c); })) {
			showNegativeMessage("Name must only contain letters (A-Z, a-z)");
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
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			showNegativeMessage("IC Number must be filled");
		}
		else if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			showNegativeMessage("Please enter digits only (no dashes \"-\")");
		}
		else if (input.length() != exactLength) {
			showNegativeMessage("IC number has incorrect format");
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
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			showNegativeMessage("Please enter numbers only (no dashes)");
		}
		else if ((!input.empty() && (input.length() < minLength || input.length() > 20))) {
			showNegativeMessage("Incorrect phone number length");
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
			std::cout << ANSI_COLOR_GOLD;
			std::getline(std::cin, input);
			std::cout << ANSI_COLOR_RESET;
		}
		else {
			char ch;
			int count = 0;
			std::cout << ANSI_COLOR_GOLD;
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
			std::cout << ANSI_COLOR_RESET << std::endl;
		}

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (input.empty()) {
			showNegativeMessage("Password must be filled");
		}
		else if (isReg && !std::regex_match(input, pattern)) {
			showNegativeMessage("Password must be at least 8 characters long and contain at least 1 digit,");
			showNegativeMessage("1 uppercase letter, and 1 lowercase letter");
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
			showNegativeMessage("Must not be empty");
		}
		else if (!std::regex_match(input, pattern)) {
			showNegativeMessage("Can only contain numbers, letters or dashes \"-\"");
		}
		else {
			break;
		}
		std::cout << "|\n";
	} while (true);
	return input;
}


std::string Util::parseDateInput(std::string text, const bool& showInstruction, std::string startDate = "") {
	const std::regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");
	std::string input;

	if (showInstruction) { std::cout << "|\tIn YYYY-MM-DD format (2001-05-03)\n"; }
	do {
		std::cout << "|\t" << text << "\t: ";
		std::cout << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;
		
		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		int day, month, year;
		if (!std::regex_match(input, datePattern)) {
			showNegativeMessage("Date must be in DD/MM/YYYY format");
		}
		else {
			// Extracting parts of the date from the inputted format (DD/MM/YYYY)
			std::smatch match;
			std::regex_search(input, match, datePattern);
			year = std::stoi(match[1].str());
			month = std::stoi(match[2].str());
			day = std::stoi(match[3].str());
		}
		

		// Basic validation of month and day ranges
		bool dateIsInvalid = 
			(month < 1 || month > 12) ||
			(day < 1 || day > 31) ||
			((month == 4 || month == 6 || month == 9 || month == 11) && day > 30);
		if (dateIsInvalid) {
			showNegativeMessage("Invalid date");
		}
		else if (month == 2) {  
			bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
			if (day > 29 || (day == 29 && !isLeapYear)) {
				showNegativeMessage("Invalid date (This year is a leap year)");
			}
		} 
		else if (startDate.empty()) {
			if (Util::firstDateIsEarlier(input, getCurrentDate())) {
				showNegativeMessage("Date must not be earlier than today");
			}
		}
		else if (Util::firstDateIsEarlier(input, startDate)) {
			showNegativeMessage("Date must not be earlier than starting date");
		}
		else {
			break;
		}
		std::cout << "|\n";
	} while (true);
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
	oss << (localTime.tm_year + 1900) << "-"// Year (years since 1900)
		<< std::setw(2) << (localTime.tm_mon + 1) << "-" // Month (zero-based)
		<< std::setw(2) << localTime.tm_mday; // Day
		
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


std::string Util::formatCurrencyDecimal(std::string input) {
	// Find the position of the decimal point
	std::size_t decimalPos = input.find('.');

	// If there's no decimal point or the number already has 2 or fewer decimal places, return the original string
	if (decimalPos == std::string::npos || decimalPos + 3 >= input.size()) {
		return input;
	}

	// Otherwise, truncate the string to two decimal places
	return input.substr(0, decimalPos + 3);
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



bool Util::firstDateIsEarlier(const std::string& date1, const std::string& date2) {
	struct std::tm tm1 = {};
	struct std::tm tm2 = {};

	// Convert date strings to tm structures
	std::istringstream ss1(date1);
	std::istringstream ss2(date2);

	ss1 >> std::get_time(&tm1, "%Y-%m-%d");
	ss2 >> std::get_time(&tm2, "%Y-%m-%d");

	// Convert tm structures to time_t
	std::time_t time1 = std::mktime(&tm1);
	std::time_t time2 = std::mktime(&tm2);

	// Compare time_t values
	return time1 < time2;
}


int Util::daysBetweenDates(const std::string& date1, const std::string& date2) {
	struct std::tm tm1 = {};
	struct std::tm tm2 = {};

	// Convert date strings to tm structures
	std::istringstream ss1(date1);
	std::istringstream ss2(date2);

	ss1 >> std::get_time(&tm1, "%Y-%m-%d");
	ss2 >> std::get_time(&tm2, "%Y-%m-%d");

	// Ensure that the tm structures are correctly initialized
	tm1.tm_hour = 0; tm1.tm_min = 0; tm1.tm_sec = 0;
	tm2.tm_hour = 0; tm2.tm_min = 0; tm2.tm_sec = 0;

	// Convert tm structures to time_t
	std::time_t time1 = std::mktime(&tm1);
	std::time_t time2 = std::mktime(&tm2);

	// Calculate the difference in seconds and convert to days
	double differenceInSeconds = std::difftime(time2, time1);
	int differenceInDays = std::round(differenceInSeconds / (60 * 60 * 24));

	return differenceInDays;
}
//////