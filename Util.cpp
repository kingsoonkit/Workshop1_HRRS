#include "Util.h"

/////
/// Display functions

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
	std::cout << "|\t" << ANSI_COLOR_ORANGE << "Refreshing in ";
	for (int i = seconds; i > 0; --i) {
		std::cout << i << "... ";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second
	}
	std::cout << ANSI_COLOR_RESET;
}

void Util::showLogHeading(const std::string& name, const std::string& userType, const std::string& username) {
	std::cout << "|\tLOGGED AS: " << ANSI_COLOR_YELLOW << name << " [" << userType << "]" << ANSI_COLOR_RESET << "  |  Username: " << ANSI_COLOR_YELLOW << username << '\n' << ANSI_COLOR_RESET;
}

void Util::showLogHeading(const std::string& userType, const std::string& username) {
	std::cout << "|\tLOGGED AS: " << ANSI_COLOR_YELLOW << username << " [" << userType << "]\n" << ANSI_COLOR_RESET;
}

void Util::showEscInstruction() {
	std::cout << "|\t(Enter \"" << ANSI_COLOR_ORANGE << "ESC" << ANSI_COLOR_RESET << "\" to return to previous page)\n";
}

void Util::showInvalidAction() {
	std::cout << "|\t" << ANSI_COLOR_RED << "{ Invalid action }\n" << ANSI_COLOR_RESET;
}

void Util::showBookingIDDisplay(const std::string& bookingID) {
	std::cout << "|\t" << ANSI_COLOR_YELLOW << "The Booking ID: " << ANSI_COLOR_GOLD << bookingID << "\n" << ANSI_COLOR_RESET;
}

void Util::showPressAnyKeyToContinue() {
	std::cout << "|\t" << ANSI_COLOR_YELLOW << "Enter any key to continue\n" << ANSI_COLOR_RESET;
	std::cout << "|\n";
	Util::showHorizontalLine("double");
	std::cout << "|\t"; _getch();
	std::cout << "\n";
}

void Util::showPositiveMessage(const std::string& message, const bool& addMargin) {
	if (addMargin) { std::cout << "|\n"; }
	std::cout << "|\t" << ANSI_COLOR_GREEN << "[ "<< message <<" ]\n" << ANSI_COLOR_RESET;
	if (addMargin) { std::cout << "|\n"; }
}

void Util::showPassiveMessage(const std::string& message) {
	std::cout << "|\t" << ANSI_COLOR_GOLD << message << "\n" << ANSI_COLOR_RESET;
}

void Util::showNegativeMessage(const std::string& message) {
	std::cout << "|\t" << ANSI_COLOR_RED << message << "\n" << ANSI_COLOR_RESET;
}

std::string Util::writeTodayDate(const bool addNewline, int tabCount = 8) {
	std::string tab_str;
	for (int i = 0; i < tabCount; i++) {
		tab_str += "\t";
	}
	return ANSI_COLOR_RESET + tab_str + "Today: " + ANSI_COLOR_YELLOW + Util::getCurrentDate() + (addNewline ? "\n" : "") + ANSI_COLOR_RESET;
}

std::string Util::writeRoomList(const bool addNewline, std::vector<std::string> rooms) {
	std::string roomList;

	roomList += ANSI_COLOR_GOLD;
	for (int i = 0; i < rooms.size(); i++) {
		roomList += rooms[i];
		if (i != rooms.size() - 1) {
			roomList += ", ";
		}
	}
	roomList += ANSI_COLOR_RESET;
	roomList += (addNewline ? "\n" : "");

	return roomList;

}

/////
/// Input parsing functions

std::string Util::parseUsernameInput() {
	std::string input;
	constexpr int maxLength = 30;

	while (true) {
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Username" << ": " << ANSI_COLOR_GOLD;
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
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Name" << ": " << ANSI_COLOR_GOLD;
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
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "IC number" << ": " << ANSI_COLOR_GOLD;
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
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Phone number (optional)" << ": " << ANSI_COLOR_GOLD;
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
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Password" << ": ";

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


std::string Util::parseTextInput(const std::string& fieldName) {
	std::regex pattern("^[a-zA-Z0-9]+(?:-[a-zA-Z0-9]+)*$");
	std::string input;

	do {
		std::cout << "|\t" << ANSI_COLOR_YELLOW << fieldName << ": " << ANSI_COLOR_GOLD;
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


std::string Util::parseDateInput(std::string fieldName, const bool& showInstruction, std::string startDate) {
	const std::regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");
	std::string input;

	if (showInstruction) { std::cout << "|\tIn YYYY-MM-DD format (2001-05-03)\n"; }
	do {
		std::cout << "|\t" << ANSI_COLOR_YELLOW << fieldName << ": " << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;
		
		// Exit
		if (input == "esc" || input == "ESC") { return __EXIT_CODE__; }

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
			else if (Util::isDateOverLimit(year, month, day)) {
				showNegativeMessage("You can only book under 18 months in advance (1.5 years)");
			}
			else if (startDate.empty()) { // When start date is known (Walk-in takes current date)
				if (Util::isFirstDateEarlier(input, getCurrentDate())) {
					showNegativeMessage("Date must not be earlier than today");
				}
				else { break; }
			}
			else if (Util::isFirstDateEarlier(input, startDate)) {
				showNegativeMessage("Date must not be earlier than starting date");
			}
			else if (!isStartDateValid(input)) {
				showNegativeMessage("Stay duration must be at least 2 days 1 night");
			}
			else if (!isStayDurationUnderLimit(input, startDate)) {
				showNegativeMessage("Stay duration maximum is 14 nights per stay");
			}
			else {
				break;
			}
		}		
		std::cout << "|\n";
	} while (true);
	return input;
}


std::string Util::parseYearInput() {
	std::string input;
	while (true) {
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Year: " << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			showNegativeMessage("Only numbers are allowed");
		}
		else if (std::stoi(input) < 1990) {  // If it's outside range of 1-12 (inclusive)
			showNegativeMessage("Year is too long ago");
		}
		else {
			break;
		}
		std::cout << "|\n";
	}
	return input;
}


std::string Util::parseMonthInput() {
	std::string input;
	while (true) {
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Month: " << ANSI_COLOR_GOLD;
		std::getline(std::cin, input);
		std::cout << ANSI_COLOR_RESET;

		// Exit
		if (input == "esc" || input == "ESC") {
			return __EXIT_CODE__;
		}

		// Validation
		if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
			showNegativeMessage("Only numbers are allowed");
		}
		else if (std::stoi(input) < 1 || std::stoi(input) > 12) {  // If it's outside range of 1-12 (inclusive)
			showNegativeMessage("Invalid month"); 
		}
		else {
			break;
		}
		std::cout << "|\n";
	}
	return input;
}

/////
/// Miscellaneous functions

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


std::string Util::formatCurrencyDecimal(std::string input) {
	// Find the position of the decimal point
	std::size_t decimalPos = input.find('.');

	// If there's no decimal point or the number already has 2 or fewer decimal places, return the original string
	if (decimalPos == std::string::npos || decimalPos + 3 >= input.size()) {
		return input;
	}

	// Otherwise, round up the string to two decimal places
	double roundedValue = std::stod(input);
	double roundedCeil = std::ceil(roundedValue * 100) / 100; // Round up to two decimal places

	// Convert the rounded value back to a string
	std::string roundedString = std::to_string(roundedCeil);

	// Truncate if necessary to ensure exactly two decimal places
	size_t truncatePos = roundedString.find('.');
	if (truncatePos != std::string::npos && truncatePos + 3 < roundedString.size()) {
		roundedString.erase(truncatePos + 3);
	}

	return roundedString;
}


std::string Util::generateInvoice_AsString(std::vector<std::string>& data, std::vector<std::vector<std::string>>& rooms) {
	std::ostringstream oss;
	const int idxRoomNumber = 0;
	const int idxRoomType = 1;
	const int idxPricePerNight = 2;

	const int idxBookingID = 0;
	const int idxCurrentDate = 1;
	const int idxName = 2;
	const int idxPhoneNo = 3;
	const int idxBookingType = 4;
	const int idxStartDate = 5;
	const int idxEndDate = 6;
	const int idxTotalDays = 7;
	const int idxTotalNights = 8;
	const int idxNetPrice = 9;

	oss << ANSI_COLOR_GOLD;
	oss << "=========================================================================\n";
	oss << "                                   INVOICE\n";
	oss << "=========================================================================\n";
	oss << "\n";
	oss << "   Booking ID   : " << data[idxBookingID] << "                  Date: " << data[idxCurrentDate] << "\n";
	oss << "\n";
	oss << "   Name         : " << data[idxName] << "\n";
	oss << "   Phone Number : " << data[idxPhoneNo] << "\n";
	oss << "\n";
	oss << "=========================================================================\n";
	oss << "\n";
	oss << "   Booking Information:\n";
	oss << "   --------------------\n";
	oss << "\n";
	for (const auto& roomData : rooms) {
		oss << "      Room Number     : " << roomData[idxRoomNumber] << "\n";
		oss << "      Room Name       : " << roomData[idxRoomType] << "\n";
		oss << "      Price per Night : " << formatCurrencyDecimal(roomData[idxPricePerNight]) << "\n";
		oss << "\n";
	}
	oss << "---------------------------------------------------------------------------\n";
	oss << "\n";
	oss << "   [ " << data[idxBookingType] << " ]\n";
	oss << "   Start Date   : " << data[idxStartDate] << "\n";
	oss << "   End Date     : " << data[idxEndDate] << "\n";
	oss << "   Duration     : " << data[idxTotalDays] << " days, " << data[idxTotalNights] << " night(s)" << "\n";
	oss << "\n";
	oss << "   Net Price    : RM" << formatCurrencyDecimal(data[idxNetPrice]) << "\n";
	oss << "\n";
	oss << "===========================================================================\n";
	oss << "                            THANK YOU FOR CHOOSING\n";
	oss << "                                  OUR HOTEL!\n";
	oss << "===========================================================================\n";
	oss << ANSI_COLOR_RESET;

	
	return oss.str();
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

/////
/// Boolean functions

bool Util::isFirstDateEarlier(const std::string& date1, const std::string& date2) {
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


bool Util::isDateOverLimit(int year, int month, int day) {
	// Get the current date
	std::time_t now = std::time(0);
	std::tm currentDate;
	localtime_s(&currentDate, &now);

	// Set the maximum booking window (18 months in advance)
	currentDate.tm_mon += 18;
	std::mktime(&currentDate); // Normalize the date in case of overflow

	// Construct the booking date
	std::tm bookingDate = {};
	bookingDate.tm_year = year - 1900;
	bookingDate.tm_mon = month - 1;
	bookingDate.tm_mday = day;

	// Convert to time_t for comparison
	std::time_t bookingTime = std::mktime(&bookingDate);

	// Check if booking date is within the allowed window
	return bookingTime > std::mktime(&currentDate);
}


bool Util::isStartDateValid(const std::string& date) {
	struct std::tm tm1 = {};
	struct std::tm tm2 = {};

	// Convert date strings to tm structures
	std::istringstream ss1(date);
	std::istringstream ss2(getCurrentDate());

	ss1 >> std::get_time(&tm1, "%Y-%m-%d");
	ss2 >> std::get_time(&tm2, "%Y-%m-%d");

	// Convert tm structures to time_t
	std::time_t time1 = std::mktime(&tm1);
	std::time_t time2 = std::mktime(&tm2);

	double secondsDifference = std::difftime(time1, time2);
	double dayDifference = std::abs(secondsDifference) / 86400; // 86400 seconds in a day

	return dayDifference >= 1.0;
}


bool Util::isStayDurationUnderLimit(const std::string& date1, const std::string& date2) {
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

	double secondsDifference = std::difftime(time1, time2);
	double dayDifference = std::abs(secondsDifference) / 86400; // 86400 seconds in a day

	return dayDifference <= 14.0;
}