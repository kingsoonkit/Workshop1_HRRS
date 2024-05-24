#include <string>
#include <vector>
#include "DBConnection.h"

#pragma once
class DBUtil {
public:
	static bool isUsernameExist(const std::string& username);
	static bool isBookingIDExist(const std::string& bookingID);

	static bool isRoomNumberExist(const std::vector<std::string>& rooms, DBConnection& db);
	static bool isRoomAvailable(const std::vector<std::string>& rooms, DBConnection& db);

	//static bool isAvailable(const std::vector<std::string>& rooms, DBConnection& db);

	static bool updateRoomStatuses();
	static bool updateBookingNetPrice(std::string bookingID);
};

