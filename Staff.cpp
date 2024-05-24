#include "Staff.h"

std::string Staff::staffUsername;
std::string Staff::name;
std::string Staff::phoneNo;
std::string Staff::userType;
std::string Staff::password;


void Staff::setStaffUsername(const std::string& staffUsername) {
	Staff::staffUsername = staffUsername;
}
void Staff::setName(const std::string& name) {
	Staff::name = name;
}
void Staff::setPhoneNo(const std::string& phoneNo) {
	Staff::phoneNo = phoneNo;
}
void Staff::setUserType(const std::string& userType) {
	Staff::userType = userType;
}
void Staff::setPassword(const std::string& password) {
	Staff::password = password;
}


std::string Staff::getStaffUsername() {
	return staffUsername;
}
std::string Staff::getName() {
	return name;
}
std::string Staff::getPhoneNo() {
	return phoneNo;
}
std::string Staff::getUserType() {
	return userType;
}
std::string Staff::getPassword() {
	return password;
}


bool Staff::isPasswordCorrect(const std::string& username, const std::string& password) {
	try {
		const std::string hashedInput = Util::hashText(password);

		DBConnection db;
		db.prepareStatement("SELECT Password"
							" FROM Staff"
							" WHERE StaffUsername = ? AND Password = ?");
		db.stmt->setString(1, username);
		db.stmt->setString(2, hashedInput);
		db.QueryResult();

		while (db.res->next())
			if (hashedInput == db.res->getString("Password"))
				return true;

	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}

	return false;
}