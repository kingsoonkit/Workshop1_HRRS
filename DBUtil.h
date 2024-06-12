#include "DBConnection.h"
#include "Util.h"

#define __BOOKING_TYPE_WALK_IN__ "Walk-In"
#define __BOOKING_TYPE_RESERVATION__ "Reservation"

#define __BOOKING_STATUS_CHECKEDIN__ "CheckedIn"
#define __BOOKING_STATUS_CHECKEDOUT__ "CheckedOut"
#define __BOOKING_STATUS_RESERVED__ "Reserved"
#define __BOOKING_STATUS_CANCELLED__ "Cancelled"

#pragma once
class DBUtil {
public:
	static int numOfBookingIDs;
	static std::vector<std::string> bookingIDs;

	static bool isUsernameExist(const std::string& username);
	static bool isBookingIDExist(const std::string& bookingID);
	static bool isBookingIDExist(const std::string& bookingID, const std::string& ICNumber);
	static bool isBookingReserved(const std::string& bookingID);
	static bool isBookingCheckedIn(const std::string& bookingID);
	static bool isBookingIDHere(const std::string bookingID, tabulate::Table& table, const int colIndex);
	static bool isRoomNumberExist(const std::vector<std::string>& roomNumbers);
	static bool isRoomAvailable(const std::vector<std::string>& roomNumbers);
	static bool isRoomNumberHere(const std::vector<std::string>& roomNumbers, tabulate::Table& table, const int colIndex);
	static bool isReservationOnToday(const std::string& bookingID);

	static void updateRoomStatuses();
	static void updateBookingStatuses();
	static void updateBookingNetPrice(const std::string bookingID);

	static std::vector<std::string> getRoomNumbersByBookingID(const std::string& bookingID);

	static tabulate::Table getTable_AvailableRoomsFrom(const std::string& startDate, const std::string& endDate);
	static tabulate::Table getTable_BookingHistoryByICNumber(const std::string& ICNumber = "");
	static tabulate::Table getTable_ActiveReservationsByICNumber(const std::string& ICNumber = "");
	static tabulate::Table getTable_MonthlyRoomSales(const std::string& year, const std::string& month);
	
	static void applyMonthlyBookingSales_To(std::vector<std::string>& data, const std::string& year, const std::string& month);
	static void applyFullBookingInfo_To(std::vector<std::string>& data, std::vector<std::vector<std::string>>& rooms, std::string bookingID);
	static void applyGuestBookingInfo_To(std::vector<std::string>& data, const std::string& bookingID);
};

