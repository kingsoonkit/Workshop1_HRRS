#ifndef GUEST
#define GUEST

#include "Util.h"
#include "DBConnection.h"

class Guest {
public:
	static void setGuestUsername(const  std::string& guestUsername);
	static void setName(const  std::string& name);
	static void setICNumber(const  std::string& ICNumber);
	static void setPhoneNo(const  std::string& phoneNo);
	static void setUserType(const  std::string& userType);
	static void setPassword(const  std::string& password);

	static std::string getGuestUsername();
	static std::string getName();
	static std::string getICNumber();
	static std::string getPhoneNo();
	static std::string getUserType();
	static std::string getPassword();

	static bool isUsernameExist(const std::string& username);
	static bool isPasswordCorrect(const std::string& username, const std::string& password);

	static void renderRegisterPrompt();
	static void renderReservationMenu();
	static void renderBookingHistory(const std::string& GuestUsername);
	static void renderMainMenu();

private:
	static std::string guestUsername;
	static std::string name;
	static std::string ICNumber;
	static std::string phoneNo;
	static std::string userType;
	static std::string password;
};

#endif