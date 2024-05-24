#ifndef USER_BRIDGE
#define USER_BRIDGE

#include "Guest.h"
#include "Receptionist.h"
#include "Manager.h"
#include "Util.h"
#include "DBUtil.h"

class UserBridge {

public:
	static void renderStartMenu();
	static void renderLoginPrompt();
	static void prepareMainMenuAccess(const std::string& username);
};

#endif

