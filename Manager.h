#ifndef MANAGER
#define MANAGER

#include "Staff.h"

#define MANAGER_USERTYPE "Manager"

class Manager : Staff {
public:
	static void renderMainMenu();

private:
	static void renderRoomSalesReport();
};

#endif