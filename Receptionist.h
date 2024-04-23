#pragma once
#include <string>

class Receptionist {
protected:
	std::string staffID, name, phoneNo, password;

public:
	void setStaffID(const std::string& staffID);
	void setName(const std::string& name);
	void setPhoneNo(const std::string& phoneNo);
	void setPassword(const std::string& password);

	std::string getStaffID();
	std::string getName();
	std::string getPhoneNo();
	std::string getPassword();


	void renderMainMenu();
	void renderRegistration();
};
