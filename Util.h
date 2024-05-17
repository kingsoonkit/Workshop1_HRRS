#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <regex>
#include <vector>
#include "tabulate/table.hpp"
#include "DBConnection.h"


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
	static void showLogHeading(const std::string& name, const std::string& id, const std::string& userType);
	static void showInvalidAction();

	static std::string parseUsernameInput();
	static std::string parseNameInput();
	static std::string parseICNumberInput();
	static std::string parsePhoneNumberInput();
	static std::string parsePasswordInput(const bool& hide, const bool& isReg);

	static std::string getCurrentDate();
	static std::string hashText(const std::string& input);
	static std::string truncateDecimal(std::string input);
	static std::vector<std::string> splitString(const std::string& s, char delimiter);

	static bool updateRoomStatuses();
};

