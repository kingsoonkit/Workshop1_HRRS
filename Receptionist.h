#ifndef RECEPTIONIST
#define RECEPTIONIST

#include "Staff.h"

#define RECEPTIONIST_USERTYPE "Receptionist"

class Receptionist : Staff {
public:
	static void renderMainMenu();
	static void renderRegisterPrompt();
	static void renderMenuOfCICO();

	static void displayRoomDashboard(DBConnection& db);

private:
	static void renderCI_Reservation(const std::vector<std::string>& rooms, DBConnection dbObj);
	static void renderCI_WalkIn(const std::vector<std::string>& rooms, DBConnection dbObj);
	static void renderCO(const std::vector<std::string>& rooms, DBConnection dbObj);
};

#endif

