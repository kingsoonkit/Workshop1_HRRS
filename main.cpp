#include "UserBridge.h"

int main() {
	std::cout << ANSI_COLOR_RESET;
	DBUtil::updateBookingStatuses();
	DBUtil::updateRoomStatuses();

	UserBridge::renderStartMenu();
}
