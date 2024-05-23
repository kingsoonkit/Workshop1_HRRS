#ifndef GUEST
#define GUEST

#include "Util.h"
#include "DBConnection.h"

#define GUEST_USERTYPE "Guest"

class Guest {
public:
	static void setICNumber(const  std::string& ICNumber);
	static void setGuestUsername(const  std::string& guestUsername);
	static void setName(const  std::string& name);
	static void setPhoneNo(const  std::string& phoneNo);
	static void setPassword(const  std::string& password);
	static void setUserType(const  std::string& userType);

	static std::string getICNumber();
	static std::string getGuestUsername();
	static std::string getName();
	static std::string getPhoneNo();
	static std::string getPassword();
	static std::string getUserType();

	static bool isICNumberExist(const std::string& ICNumber);
	static bool isUsernameExist(const std::string& username);
	static bool isPasswordCorrect(const std::string& username, const std::string& password);

	static void renderRegisterPrompt();
	static void renderReservationMenu();
	static void renderBookingHistory(const std::string& GuestUsername);
	static void renderMainMenu();

private:
	static std::string ICNumber;
	static std::string guestUsername;
	static std::string name;
	static std::string phoneNo;
	static std::string password;
	static std::string userType;
};

#endif