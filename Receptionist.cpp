#include "Receptionist.h"
#include "Function.h"
#include <iostream>

void Receptionist::renderMainMenu() {
	std::cout << "|	[ Receptionist Main Menu ]";
}

void Receptionist::renderRegistration() {
	std::string staffID;
	std::string name;
	std::string phoneNo;
	std::string password;


	std::cout << "|\n";
	std::cout << "|	REGISTER As Receptionist\n";
	std::cout << "|	------------------------\n";
	std::cout << "|\n";
	std::cout << "|	Register a new Receptionist Account\n";
	std::cout << "|	\n";
	std::cout << "|	--------------------------------\n";
	std::cout << "|	Account Type\t: Receptionist\n";
	std::cout << "|	---------------------------------\n";
	std::cout << "|	StaffID		 : "; std::cin >> staffID;
	std::cout << "|	Name		 : "; std::cin >> name;
	std::cout << "|	Phone Number : "; std::cin >> phoneNo;
	std::cout << "|	Password	 : "; std::cin >> password;
	std::cout << "|\n";
	std::cout << "| ------------------------------------------------------------------------------------------------------------\n";


	setStaffID(staffID);
	setName(name);
	setPhoneNo(phoneNo);
	setPassword(password);
}


void Receptionist::setStaffID(const std::string& staffID_new) {
	staffID = staffID_new;
}
void Receptionist::setName(const std::string& name_new){
	name = name_new;
}
void Receptionist::setPhoneNo(const std::string& phoneNo_new) {
	phoneNo = phoneNo_new;
}
void Receptionist::setPassword(const std::string& password_new) {
	password = password_new;
}


std::string Receptionist::getStaffID() {
	return staffID;
}
std::string Receptionist::getName() {
	return name;
}
std::string Receptionist::getPhoneNo() {
	return phoneNo;
}
std::string Receptionist::getPassword() {
	return password;
}
