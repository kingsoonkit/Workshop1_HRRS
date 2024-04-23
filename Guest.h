#pragma once
#include <string>

class Guest {
private:
	std::string guestID;
	std::string name;
	std::string ICNumber;
	std::string phoneNo;

public:
	void renderMainMenu();
	void renderRegistration();

	void setGuestID(const std::string& staffID);
	void setName(const std::string& name);
	void setPhoneNo(const std::string& phoneNo);
	void setPassword(const std::string& password);

	std::string setGuestID();
	std::string getName();
	std::string getPhoneNo();
	std::string getPassword();
};

