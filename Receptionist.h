#ifndef RECEPTIONIST
#define RECEPTIONIST

#include "Staff.h"


class Receptionist : Staff {
public:
	static void renderMainMenu();
	static void renderRegisterPrompt();
	static void renderCICOMenu();

private:
	static void renderCI_Reservation(const std::string& roomNumber, DBConnection dbObj);
	static void renderCI_WalkIn(const std::string& roomNumber, DBConnection dbObj);
	static void renderCO(const std::string& roomNumber, DBConnection dbObj);
};

#endif

