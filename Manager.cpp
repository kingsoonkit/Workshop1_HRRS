#include "Manager.h"
#include "Function.h"
#include <iostream>


std::string staffID = "[STAFF ID]";
std::string userType = "manager";

void Manager::renderMainMenu() {

	Function::renderHeading(staffID, userType);
	std::cout << "|	[ Receptionist Main Menu ]";
}