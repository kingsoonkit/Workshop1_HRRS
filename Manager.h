#ifndef MANAGER
#define MANAGER

#include "Staff.h"

#define MANAGER_USERTYPE "Manager"

class Manager : Staff {
public:
	static void renderMainMenu();
	static void renderRoomSalesReport();

};

#endif