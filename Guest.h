#ifndef GUEST
#define GUEST

#include "Util.h"
#include "DBUtil.h"

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
	static bool isPasswordCorrect(const std::string& username, const std::string& password);
	static bool isUser(const std::string& ICNumber);

	static void renderRegisterPrompt();
	static void renderMainMenu();

private:
	static std::string ICNumber;
	static std::string guestUsername;
	static std::string name;
	static std::string phoneNo;
	static std::string password;
	static std::string userType;

	static void renderAddReservationMenu();
	static void renderCancelReservationMenu();
	static void renderBookingHistory();
	
	static void viewBookingDetailsOf(const std::string& bookingID);
	static void viewCancelConfirmation(const std::vector<std::string>& roomNumbers, const std::string& bookingID);
};

#endif