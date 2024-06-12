#ifndef STAFF
#define STAFF

#include "Util.h"
#include "DBUtil.h"

class Staff {
public:
	static void setStaffUsername(const std::string& staffUsername);
	static void setName(const std::string& name);
	static void setPhoneNo(const std::string& phoneNo);
	static void setUserType(const std::string& userType);
	static void setPassword(const std::string& password);

	static std::string getStaffUsername();
	static std::string getName();
	static std::string getPhoneNo();
	static std::string getUserType();
	static std::string getPassword();

	static bool isPasswordCorrect(const std::string& username, const std::string& password);

protected:
	static std::string staffUsername;
	static std::string name;
	static std::string phoneNo;
	static std::string userType;
	static std::string password;

};

#endif



