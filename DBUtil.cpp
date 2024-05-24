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


bool DBUtil::isRoomNumberExist(const std::vector<std::string>& rooms, DBConnection& db) {
	try {
		for (int i = 0; i < rooms.size(); ++i) {
			int count = 0;
			bool roomExists = false;
			db.res->beforeFirst();
			while (db.res->next()) {
				/*count++;
				std::cout << count << db.res->getString("RoomNumber") << rooms[i] << "\n";
				*/
				if (rooms[i] == db.res->getString("RoomNumber")) {
					roomExists = true;
					break;
				}
			}
			if (!roomExists) {
				return false;
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return true;
}


bool DBUtil::isRoomAvailable(const std::vector<std::string>& rooms, DBConnection& db) {
	try {
		db.res->beforeFirst();
		for (int i = 0; i < rooms.size(); i++) {
			std::cout << "|\t" << rooms[i] << "\n";
			while (db.res->next()) {
				if (rooms[i] == db.res->getString("RoomNumber")) {
					if ("Available" != db.res->getString("RoomStatus")) {
						return false;
					}
				}
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return true;
}


bool DBUtil::updateRoomStatuses() {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE room r"
							" LEFT JOIN bookingline b ON r.RoomNumber = b.RoomNumber AND b.StartDate = CURRENT_DATE"
							" SET r.RoomStatus ="
							" CASE"
							" WHEN b.BookingStatus = 'Checked-in' && (CURRENT_DATE BETWEEN b.StartDate AND b.EndDate) THEN 'Occupied'"
							" WHEN b.BookingStatus = 'Reserved' THEN 'Reserved'"
							" WHEN b.BookingStatus = 'Checked-out' OR b.BookingStatus = 'Cancelled' OR b.BookingStatus IS NULL THEN 'Available'"
							" END;");
		db.QueryStatement();
		db.~DBConnection();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return true;
}


bool DBUtil::updateBookingNetPrice(std::string bookingID) {
	try {
		DBConnection db;
		db.prepareStatement("UPDATE booking b SET NetPrice = ("
							" SELECT SUM(r.PricePerNight * (DATEDIFF(bl.EndDate, bl.StartDate) - 1))"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" WHERE bl.BookingID = b.BookingID"
							" GROUP BY bl.BookingID)"
							" WHERE b.BookingID = ?");
		db.stmt->setString(1, bookingID);
		db.QueryStatement();
		db.~DBConnection();
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return true;
}