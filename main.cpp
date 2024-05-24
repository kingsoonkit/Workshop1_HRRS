#include "UserBridge.h"

int main() {
	DBUtil::updateRoomStatuses();

	//UserBridge::renderStartMenu();
	

	Receptionist::renderMainMenu();   // 1
	//Receptionist::renderMenuOfCICO();  // 2

	//Receptionist::renderBookingHistory();  //3

}



