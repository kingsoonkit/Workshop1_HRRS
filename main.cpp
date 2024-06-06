#include "UserBridge.h"

int main() {
	std::cout << ANSI_COLOR_RESET;
	DBUtil::updateBookingStatuses();
	DBUtil::updateRoomStatuses();

	UserBridge::renderStartMenu();
	//Guest::renderMainMenu();
	//Receptionist::renderMainMenu();   // 1
	//Manager::renderMainMenu();  
	 
	
	//Receptionist::renderDailyFunctionsMenu();  // 2
	//Receptionist::renderBookingHistory();  //3

	//Receptionist::renderInvoicePrompt("7");
}
