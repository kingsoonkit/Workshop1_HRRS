#include "DBUtil.h"


bool DBUtil::isUsernameExist(const std::string& username) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT GuestUsername FROM Guest"
							" WHERE GuestUsername = ?"
							" UNION"
							" SELECT StaffUsername FROM Staff"
							" WHERE StaffUsername = ?"
		);
		db.stmt->setString(1, username);
		db.stmt->setString(2, username);
		db.QueryResult();
		if (db.res->rowsCount() == 1) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isBookingIDExist(const std::string& bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT bookingID FROM booking"
							" WHERE bookingID = ?"
		);
		db.stmt->setString(1, bookingID);
		db.QueryResult();
		if (db.res->rowsCount() == 1) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isBookingIDExist(const std::string& bookingID, const std::string& ICNumber) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT bookingID FROM booking"
							" WHERE bookingID = ? AND ICNumber = ?"
		);
		db.stmt->setString(1, bookingID);
		db.stmt->setString(2, ICNumber);
		db.QueryResult();
		if (db.res->rowsCount() == 1) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isBookingReserved(const std::string& bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT bookingID FROM bookingline"
							" WHERE bookingStatus = 'Reserved' AND bookingID = ?"
		);
		db.stmt->setString(1, bookingID);
		db.QueryResult();
		if (db.res->rowsCount() > 0) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isReservationOnToday(const std::string& bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT bookingID FROM bookingline"
							" WHERE StartDate = CURRENT_DATE AND bookingID = ?"
		);
		db.stmt->setString(1, bookingID);
		db.QueryResult();
		if (db.res->rowsCount() > 0) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isBookingCheckedIn(const std::string& bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT bookingID FROM bookingline"
							" WHERE bookingStatus = ? AND bookingID = ?"
		);
		db.stmt->setString(1, __BOOKING_STATUS_CHECKEDIN__);
		db.stmt->setString(2, bookingID);
		db.QueryResult();
		if (db.res->rowsCount() > 0) {
			return true;
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}


bool DBUtil::isRoomNumberExist(const std::vector<std::string>& roomNumbers) {
	DBConnection db;
	try {
		db.prepareStatement("SELECT RoomNumber FROM Room");
		db.QueryResult();

		// Check if each room number exists in the result set
		for (const auto& room : roomNumbers) {
			bool roomExists = false;
			db.res->beforeFirst(); // Reset result set cursor to the beginning
			while (db.res->next()) {
				if (room == db.res->getString("RoomNumber")) {
					roomExists = true;
					break; // Room found, no need to continue the loop
				}
			}
			if (!roomExists) {
				return false; // Room not found in the result set
			}
		}
		// All room numbers exist
		return true;
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	// Error occurred or no room numbers provided
	return false;
}




bool DBUtil::isRoomAvailable(const std::vector<std::string>& roomNumbers) {
	DBConnection db;
	try {
		db.prepareStatement("SELECT RoomNumber, RoomStatus FROM Room");
		db.QueryResult();

		// Iterate through the result set and check each room status
		for (const std::string& room : roomNumbers) {
			bool roomFound = false;

			// Reset the result set cursor to the beginning for each room number check
			db.res->beforeFirst();
			while (db.res->next()) {
				if (room == db.res->getString("RoomNumber")) {
					roomFound = true;
					if ("Available" != db.res->getString("RoomStatus")) {
						return false;
					}
					break; // Room found and it's available, no need to continue the inner loop
				}
			}

			// If the room was not found in the result set, it is considered unavailable
			if (!roomFound) {
				return false;
			}
		}
		return true;
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false; // Return false if there is an exception
}


bool DBUtil::isRoomNumberHere(const std::vector<std::string>& roomNumbers, tabulate::Table& table, const int colIndex) {
	for (std::string roomNumber : roomNumbers) {
		for (int i = 1; i < table.size();i++) {
			if (!table.row(i).cell(colIndex).get_text().empty() && roomNumber == table.row(i).cell(colIndex).get_text()) {
				return true;
			}
		}
		return false;
	}
	return false;
}


bool DBUtil::isBookingIDHere(const std::string bookingID, tabulate::Table& table, const int colIndex) {
	for (int i = 1; i < table.size();i++) {
		if (!table.row(i).cell(colIndex).get_text().empty() && bookingID == table.row(i).cell(colIndex).get_text()) {
			return true;
		}
	}
	return false;
}


void DBUtil::updateRoomStatuses() {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE room r "
							" LEFT JOIN ( "
							"    SELECT "
							"        bl1.RoomNumber, "
							"        bl1.StartDate, "
							"        bl1.EndDate, "
							"        bl1.BookingStatus "
							"    FROM bookingline bl1 "
							"    JOIN ( "
							"        SELECT RoomNumber, MAX(StartDate) AS LatestStartDate, MAX(BookingID) AS LatestBookingID "
							"        FROM bookingline "
							"        WHERE CURRENT_DATE BETWEEN StartDate AND EndDate "
							"        GROUP BY RoomNumber "
							"    ) bl2 "
							"    ON bl1.RoomNumber = bl2.RoomNumber AND bl1.StartDate = bl2.LatestStartDate AND bl1.BookingID = bl2.LatestBookingID "
							" ) bl "
							" ON r.RoomNumber = bl.RoomNumber "
							" SET RoomStatus = ( "
							"    CASE "
							"        WHEN bl.BookingStatus = 'Reserved' "
							"			THEN 'Reserved' "
							"        WHEN bl.BookingStatus = 'CheckedIn' "
							"			THEN 'Occupied' "
							"        WHEN bl.BookingStatus = 'CheckedOut' "
							"            OR bl.BookingStatus = 'Cancelled' "
							"            OR bl.BookingStatus IS NULL "
							"			THEN 'Available' "
							"        ELSE r.RoomStatus "
							"    END "
							" );");
		db.QueryStatement();
		db.~DBConnection();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}


void DBUtil::updateBookingStatuses() {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE bookingline bl"
							" SET bl.BookingStatus = 'Cancelled', bl.CancelledDate = bl.StartDate"
							" WHERE bl.StartDate < CURRENT_DATE AND bl.BookingStatus = 'Reserved'");
		db.QueryStatement();
		db.~DBConnection();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}


void DBUtil::updateBookingNetPrice(std::string bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE booking b"
							" SET NetPrice = ("
							" SELECT SUM(r.PricePerNight * DATEDIFF(bl.EndDate, bl.StartDate))"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" WHERE bl.BookingStatus != 'Cancelled' AND bl.BookingID = b.BookingID"
							" )"
							" WHERE b.BookingID = ?;");
		db.stmt->setString(1, bookingID);
		db.QueryStatement();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}


std::vector<std::string> DBUtil::getRoomNumbersByBookingID(const std::string& bookingID) {
	DBConnection db;
	std::vector<std::string> rooms;

	db.prepareStatement("SELECT bl.RoomNumber"
						" FROM bookingline bl"
						" WHERE bl.BookingID = ?");
	db.stmt->setString(1, bookingID);
	db.QueryResult();
	while (db.res->next()) {
		rooms.push_back(db.res->getString("RoomNumber"));
	}
	return rooms;
}


tabulate::Table DBUtil::getTable_AvailableRoomsFrom(const std::string& startDate, const std::string& endDate) {
	DBConnection db;
	tabulate::Table table;
	try {
		db.prepareStatement("SELECT "
							"    r.RoomNumber, "
							"    r.RoomType, "
							"    r.Pax, "
							"    r.PricePerNight "
							" FROM room r "
							" LEFT JOIN ( "
							"    SELECT "
							"        bl1.BookingID, "
							"        bl1.RoomNumber, "
							"        bl1.StartDate, "
							"        bl1.EndDate, "
							"        bl1.BookingStatus "
							"    FROM bookingline bl1 "
							"    JOIN ( "
							"        SELECT RoomNumber, "
							"            MAX(StartDate) AS LatestStartDate, "
							"            MAX(BookingID) AS LatestBookingID "
							"        FROM bookingline "
							"        GROUP BY RoomNumber "
							"    ) bl2 "
							"    ON bl1.RoomNumber = bl2.RoomNumber "
							"    AND bl1.StartDate = bl2.LatestStartDate "
							"    AND bl1.BookingID = bl2.LatestBookingID "
							" ) bl "
							" ON r.RoomNumber = bl.RoomNumber "
							" WHERE bl.RoomNumber IS NULL "
							"    OR bl.BookingStatus = 'Cancelled' "
							"    OR bl.BookingStatus = 'CheckedOut' "
							"    OR (bl.BookingStatus = 'Reserved' OR bl.BookingStatus = 'CheckedIn'"
							"        AND ((? < bl.StartDate AND ? < bl.StartDate) "
							"            OR (? > bl.EndDate AND ? > bl.EndDate)));");
		db.stmt->setString(1, startDate);
		db.stmt->setString(2, endDate);
		db.stmt->setString(3, startDate);
		db.stmt->setString(4, endDate);
		db.QueryResult();

		if (db.res->rowsCount() > 0) {
			std::string RoomNumber, RoomType, Pax, PricePerNight;

			// Add column headers
			table.add_row({ "RoomNumber", "RoomType", "Pax", "PricePerNight" });
			while (db.res->next()) {
				RoomNumber = db.res->getString("RoomNumber");
				RoomType = db.res->getString("RoomType");
				Pax = db.res->getString("Pax");
				PricePerNight = Util::formatCurrencyDecimal(db.res->getString("PricePerNight"));

				table.add_row({ RoomNumber, RoomType, Pax, PricePerNight });
			}
			for (int i = 0; i <= db.res->rowsCount(); i++) {
				table[i][0].format()
					.border_left("|\t|")
					.corner("|\t+");
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return table;
}


tabulate::Table DBUtil::getTable_BookingHistoryByICNumber(const std::string& ICNumber) {
	DBConnection db;
	tabulate::Table table;
	std::string BookingID, Name, BookingType, Active;
	try {
		if (ICNumber.empty()) {
			db.prepareStatement("SELECT "
								" b.BookingID, "
								" g.ICNumber, "
								" g.Name, "
								" b.BookingType, "
								" CASE WHEN (CURRENT_DATE BETWEEN bl.StartDate AND bl.EndDate AND bl.BookingStatus = 'CheckedIn') "
								"    OR (SELECT COUNT(*)"
								"		FROM bookingline bl_sub"
								"		WHERE bl_sub.BookingID = b.BookingID"
								"		AND bl_sub.BookingStatus = 'Reserved'"
								"	 ) > 0 "
								"    THEN 'Yes' ELSE 'No' "
								"    END AS Active "
								" FROM booking b "
								" LEFT JOIN guest g ON b.ICNumber = g.ICNumber "
								" LEFT JOIN bookingline bl ON b.BookingID = bl.BookingID "
								" GROUP BY b.BookingID");
			db.QueryResult();
			if (db.res->rowsCount() > 0) {
				// Add column headers
				table.add_row({ "BookingID", "ICNumber", "Name", "BookingType", "Active" });
				std::string ic;
				while (db.res->next()) {
					BookingID = db.res->getString("BookingID");
					ic = db.res->getString("ICNumber");
					Name = db.res->getString("Name");
					BookingType = db.res->getString("BookingType");
					Active = db.res->getString("Active");
					
					table.add_row({ BookingID, ic, Name, BookingType, Active });
				}
				for (int i = 0; i <= db.res->rowsCount(); i++) {
					table[i][0].format()
						.border_left("|\t|")
						.corner("|\t+");
				}
			}
		}
		else {
			db.prepareStatement("SELECT "
								" b.BookingID, "
								" b.BookingType, "
								" CASE WHEN (CURRENT_DATE BETWEEN bl.StartDate AND bl.EndDate AND bl.BookingStatus = 'CheckedIn') "
								"    OR (SELECT COUNT(*)"
								"		FROM bookingline bl_sub"
								"		WHERE bl_sub.BookingID = b.BookingID"
								"		AND bl_sub.BookingStatus = 'Reserved'"
								"	 ) > 0 "
								"    THEN 'Yes' ELSE 'No' "
								"    END AS Active "
								" FROM booking b "
								" LEFT JOIN guest g ON b.ICNumber = g.ICNumber "
								" LEFT JOIN bookingline bl ON b.BookingID = bl.BookingID "
								" WHERE b.ICNumber = ? "
								" GROUP BY b.BookingID");
			db.stmt->setString(1, ICNumber);
			db.QueryResult();
			if (db.res->rowsCount() > 0) {
				// Add column headers
				table.add_row({ "BookingID", "BookingType", "Active"});
				while (db.res->next()) {
					BookingID = db.res->getString("BookingID");
					BookingType = db.res->getString("BookingType");
					Active = db.res->getString("Active");

					table.add_row({ BookingID, BookingType, Active });
				}
				for (int i = 0; i <= db.res->rowsCount(); i++) {
					table[i][0].format()
						.border_left("|\t|")
						.corner("|\t+");
				}
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return table;
}

int DBUtil::numOfBookingIDs = 0;
std::vector<std::string> DBUtil::bookingIDs;
tabulate::Table DBUtil::getTable_ActiveReservationsByICNumber(const std::string& ICNumber) {
	DBConnection db;
	tabulate::Table table;
	std::string BookingID, NetPrice, StartDate, EndDate, RoomNumber, RoomType, Pax, PricePerNight, BookingStatus;
	try {
		numOfBookingIDs = 0;
		if (ICNumber.empty()) {
			// TODO
		}
		else {
			db.prepareStatement("SELECT b.BookingID, bl.StartDate, bl.EndDate, b.NetPrice, r.RoomNumber, r.RoomType, r.Pax, r.PricePerNight"
								" FROM booking b "
								" LEFT JOIN bookingline bl ON b.BookingID = bl.BookingID "
								" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber "
								" WHERE b.StaffUsername IS NULL "
								" AND bl.StartDate >= CURRENT_DATE "
								" AND bl.BookingStatus = 'Reserved' "
								" AND b.ICNumber = ?"); 
			db.stmt->setString(1, ICNumber);
			db.QueryResult();

			if (db.res->rowsCount() > 0) {
				// Add column headers
				table.add_row({ "Booking ID", "StartDate", "EndDate", "Net Price", "Room Number", "Room Name", "Pax", "PricePerNight" });

				std::string currentID;
				numOfBookingIDs = 0;
				bookingIDs.clear();
				while (db.res->next()) {
					numOfBookingIDs++;
					BookingID = db.res->getString("BookingID");
					StartDate = db.res->getString("StartDate");
					EndDate = db.res->getString("EndDate");
					NetPrice = "RM" + Util::formatCurrencyDecimal(db.res->getString("NetPrice"));
						
					if (currentID == BookingID) {
						BookingID.clear();
						StartDate.clear();
						EndDate.clear();
						NetPrice.clear();
						numOfBookingIDs--;
					}
					else { 
						currentID = BookingID;
						bookingIDs.push_back(BookingID);
					}
					
					RoomNumber = db.res->getString("RoomNumber");
					RoomType = db.res->getString("RoomType");
					Pax = db.res->getString("Pax");
					PricePerNight = "RM" + Util::formatCurrencyDecimal(db.res->getString("PricePerNight"));

					table.add_row({ BookingID, StartDate, EndDate, NetPrice, RoomNumber, RoomType, Pax, PricePerNight });
				}
				for (int i = 0; i <= db.res->rowsCount(); i++) {
					table[i][0].format()
						.border_left("|\t|")
						.corner("|\t+");
				}
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return table;
}


tabulate::Table DBUtil::getTable_MonthlyRoomSales(const std::string& year, const std::string& month) {
	DBConnection db;
	tabulate::Table table;
	try {
		db.prepareStatement("SELECT "
							" r.RoomType,"
							" SUM(r.PricePerNight * DATEDIFF(bl.EndDate, bl.StartDate)) AS Sales,"
							" COUNT(*) AS BookingCount"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" WHERE bl.BookingStatus != 'Cancelled'"
							" AND YEAR(bl.StartDate) = ?"
							" AND MONTH(bl.StartDate) = ?"
							" GROUP BY r.RoomType"
							" ORDER BY Sales DESC;");
		db.stmt->setString(1, year);
		db.stmt->setString(2, month);
		db.QueryResult();

		int rankNum = 0;
		std::string RoomType, Sales, BookingCount;
		if (db.res->rowsCount() > 0) {
			// Add column headers
			table.add_row({ "Ranking No.", "Room Type", "Sales", "Number of Bookings" });
			while (db.res->next()) {
				rankNum++;
				RoomType = db.res->getString("RoomType");
				Sales = "RM" + Util::formatCurrencyDecimal(db.res->getString("Sales"));
				BookingCount = db.res->getString("BookingCount");

				table.add_row({ std::to_string(rankNum), RoomType, Sales , BookingCount});
			}
			for (int i = 0; i <= db.res->rowsCount(); i++) {
				table[i][0].format()
					.border_left("|\t|")
					.corner("|\t+");
					
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return table;
}


void DBUtil::applyMonthlyBookingSales_To(std::vector<std::string>& data, const std::string& year, const std::string& month){
	DBConnection db;
	try {
		db.prepareStatement("SELECT"
							" COUNT(*) AS TotalBookings,"
							" SUM(r.PricePerNight * DATEDIFF(bl.EndDate, bl.StartDate)) AS TotalSales"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" WHERE bl.BookingStatus != 'Cancelled'"
							" AND YEAR(bl.StartDate) = ?"
							" AND MONTH(bl.StartDate) = ?;");
		db.stmt->setString(1, year);
		db.stmt->setString(2, month);
		db.QueryResult();

		data.clear();
		if (db.res->rowsCount() == 1 && db.res->next()) {
			data.push_back(db.res->getString("TotalBookings"));
			data.push_back("RM" + Util::formatCurrencyDecimal(db.res->getString("TotalSales")));
		}
		else {
			std::cout << "Something is wrong\n";
		}
	} 
	catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}


void DBUtil::applyFullBookingInfo_To(std::vector<std::string>& data, std::vector<std::vector<std::string>>& rooms, std::string bookingID) {
	DBConnection db;
	try {
		db.prepareStatement("SELECT "
							" bl.BookingID, "
							" CURRENT_DATE as CurrentDate, "
							" g.Name, "
							" g.PhoneNo, "
							" b.BookingType, "
							" bl.StartDate, "
							" bl.EndDate, "
							" DATEDIFF(bl.EndDate, bl.StartDate)+1 AS TotalDays, "
							" DATEDIFF(bl.EndDate, bl.StartDate) AS TotalNights, "
							" b.NetPrice, "
							" r.RoomNumber, "
							" r.RoomType, "
							" r.PricePerNight "
							" FROM bookingline bl "
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber "
							" LEFT JOIN booking b ON bl.BookingID = b.BookingID "
							" LEFT JOIN guest g ON b.ICNumber = g.ICNumber "
							" WHERE bl.BookingID = ?");
		db.stmt->setString(1, bookingID);
		db.QueryResult();

		if (db.res->rowsCount() > 0) {
			if (db.res->next()) { // uses if, because it only needs 1 row
				data.push_back(db.res->getString("BookingID"));
				data.push_back(db.res->getString("CurrentDate"));
				data.push_back(db.res->getString("Name"));
				data.push_back(db.res->getString("PhoneNo"));
				data.push_back(db.res->getString("BookingType"));
				data.push_back(db.res->getString("StartDate"));
				data.push_back(db.res->getString("EndDate"));
				data.push_back(db.res->getString("TotalDays"));
				data.push_back(db.res->getString("TotalNights"));
				data.push_back(db.res->getString("NetPrice"));

				db.res->beforeFirst();
				int i = 0;
				while (db.res->next() && i < db.res->rowsCount()) { // Fetch data for all rooms
					// Ensure the row exists or create it
					if (i >= rooms.size()) {
						rooms.resize(i + 1); // Increase the size of the outer vector if needed
					}
					rooms[i].push_back(db.res->getString("RoomNumber"));
					rooms[i].push_back(db.res->getString("RoomType"));
					rooms[i].push_back(db.res->getString("PricePerNight"));
					++i;
				}
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}


void DBUtil::applyGuestBookingInfo_To(std::vector<std::string>& data, const std::string& bookingID) {
	DBConnection db;
	try {
		db.prepareStatement("SELECT "
							" bl.BookingID, "
							" g.ICNumber, "
							" g.Name, "
							" g.PhoneNo, "
							" bl.StartDate, "
							" bl.EndDate, "
							" DATEDIFF(bl.EndDate, bl.StartDate)+1 AS TotalDays, "
							" DATEDIFF(bl.EndDate, bl.StartDate) AS TotalNights "
							" FROM bookingline bl "
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber "
							" LEFT JOIN booking b ON bl.BookingID = b.BookingID "
							" LEFT JOIN guest g ON b.ICNumber = g.ICNumber "
							" WHERE bl.BookingID = ?;");
		db.stmt->setString(1, bookingID);
		db.QueryResult();

		std::cout << db.res->rowsCount() << std::endl;

		if (db.res->rowsCount() > 0 && db.res->next()) {
			data.push_back(db.res->getString("BookingID"));
			data.push_back(db.res->getString("ICNumber"));
			data.push_back(db.res->getString("Name"));
			data.push_back(db.res->getString("PhoneNo"));
			data.push_back(db.res->getString("StartDate"));
			data.push_back(db.res->getString("EndDate"));
			data.push_back(db.res->getString("TotalDays"));
			data.push_back(db.res->getString("TotalNights"));
		}
		else {
			std::cout << "Something is wrong\n";
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
}
