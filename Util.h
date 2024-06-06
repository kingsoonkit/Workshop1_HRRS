#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include "tabulate/table.hpp"

#define ANSI_COLOR_RED   "\033[91m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_GOLD "\033[33m"
#define ANSI_COLOR_YELLOW "\x1b[38;5;191m"
#define ANSI_COLOR_ORANGE "\x1b[38;5;202m"
#define ANSI_COLOR_BLUE  "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_RESET "\033[0m"

#define __EXIT_CODE__ "EXIT_LOOP_CONFIRM_99"



class Util {
public:
	static void showHorizontalLine(const std::string& lines);
	static void showRefreshCountdown();
	static void showLogHeading(const std::string& name, const std::string& userType, const std::string& username);
	static void showLogHeading(const std::string& name, const std::string& userType);
	static void showInvalidAction();
	static void showEscInstruction();
	static void showPositiveMessage(const std::string& message, const bool& addMargin = false);
	static void showPassiveMessage(const std::string& message);
	static void showNegativeMessage(const std::string& message);

	static void showBookingIDDisplay(const std::string& bookingID);
	static void showPressAnyKeyToContinue();

	static std::string writeTodayDate(const bool addNewline, int tabCount);
	static std::string writeRoomList(const bool addNewline, std::vector<std::string> rooms);

	static std::string parseUsernameInput();
	static std::string parseNameInput();
	static std::string parseICNumberInput();
	static std::string parsePhoneNumberInput();
	static std::string parsePasswordInput(const bool& hide, const bool& isReg);
	static std::string parseTextInput(const std::string& fieldName);
	static std::string parseDateInput(std::string text, const bool& showInstruction, std::string startDate = "");
	static std::string parseYearInput();
	static std::string parseMonthInput();

	static std::string getCurrentDate();
	static std::string hashText(const std::string& input);
	static std::string formatCurrencyDecimal(std::string input);
	static std::vector<std::string> split(const std::string& s, char delimiter);

	static bool isStartDateValid(const std::string& date);
	static bool isDateOverLimit(int year, int month, int day);
	static bool firstDateIsEarlier(const std::string& date1, const std::string& date2);


	static int daysBetweenDates(const std::string& date1, const std::string& date2);

	static std::string generateInvoice_AsString(std::vector<std::string>& data, std::vector<std::vector<std::string>>& rooms);
	
	static void generatePDF(const std::string& text, const std::string& filename);
};

