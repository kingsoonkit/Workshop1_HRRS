#ifndef RECEPTIONIST
#define RECEPTIONIST

#include "Staff.h"

#define RECEPTIONIST_USERTYPE "Receptionist"

class Receptionist : Staff {
public:
	static void renderMainMenu();
	static void renderRegisterPrompt();
	static void renderDailyFunctionsMenu();

	static void viewRoomDashboard(DBConnection& db);

//private:
	static void renderCI_Reservation(std::string& bookingID);
	static void renderCI_WalkIn(const std::vector<std::string>& rooms);
	static void renderCO(const std::string bookingID);

	static void renderBookingHistory();
	static void viewBookingDetailsOf(const std::string& bookingID);

	static void renderInvoicePrompt(const std::string& bookingID);
};

#endif

