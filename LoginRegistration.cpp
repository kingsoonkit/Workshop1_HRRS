#include "LoginRegistration.h"
#include <iostream>
#include "Manager.h"
#include "Receptionist.h"
#include "Guest.h"

using namespace std;



void Login::renderInputPrompt() {
	string staffID; 
	string password;
	string userType = "";

	cout << "|\n";
	cout << "|\tLOGIN\n";
	cout << "|	-----\n";
	cout << "|	Staff ID: "; cin >> staffID;
	cout << "|	Password: "; cin >> password;
	cout << "|\n";
	cout << "| -------------------------------------------------------------------------------------------------------------\n";

	if (userType == "Manager") {
		Manager manager;
		manager.renderMainMenu();
	}
	else if (userType == "Receptionist") {
		Receptionist receptionist;
		receptionist.renderMainMenu();
	}
	else if (userType == "Guest") {
		Guest guest;
		guest.renderMainMenu();
	}
	else {
		cout << "UNEXPECTED ERROR";
	}

}



void Registration::renderInputPrompt(const std::string& userType) {

	if(userType == "As Receptionist") {
		Receptionist receptionist;
		receptionist.renderRegistration();
	}
	else if(userType == "As Guest") {
		Guest guest;
		Guest::renderRegistration();
	}

}