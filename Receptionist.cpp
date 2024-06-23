#include "Receptionist.h"
#include "Guest.h"


void Receptionist::renderRegisterPrompt() {
	do {
		system("cls");

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Register as Receptionist\n" << ANSI_COLOR_RESET;
		Util::showEscInstruction();
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";

		std::string tempArr[4];

		bool isExiting = false;
		std::cout << "|\tUsername will be used for login.\n";
		do {
			tempArr[0] = Util::parseUsernameInput();
			if (tempArr[0] == __EXIT_CODE__) { isExiting = true; break; }
			if (DBUtil::isUsernameExist(tempArr[0])) {
				std::cout << "|\t" << ANSI_COLOR_RED << "This username already exists. Try another one\n" << ANSI_COLOR_RESET;
				std::cout << "|\n";
			}
			else {
				std::cout << "|\t" << ANSI_COLOR_GREEN << "[OK]\n" << ANSI_COLOR_RESET;
				std::cout << "|\t-----------------------------------\n";
				break;
			}
		} while (true);
		if (isExiting) {
			break; // Break the outer loop if exit code was detected
		}

		tempArr[1] = Util::parseNameInput();
		if (tempArr[1] == __EXIT_CODE__) { break; }

		tempArr[2] = Util::parsePhoneNumberInput();
		if (tempArr[2] == __EXIT_CODE__) { break; }

		tempArr[3] = Util::parsePasswordInput(false, true);
		if (tempArr[3] == __EXIT_CODE__) { break; }


		try {
			DBConnection db;
			db.prepareStatement("INSERT INTO Staff (StaffUsername, Name, PhoneNo, Password, UserType) VALUES (?,?,?,?,?)");
			db.stmt->setString(1, tempArr[0]);
			db.stmt->setString(2, tempArr[1]);
			db.stmt->setString(3, tempArr[2]);
			db.stmt->setString(4, Util::hashText(tempArr[3]));
			db.stmt->setString(5, RECEPTIONIST_USERTYPE);
			db.QueryStatement();

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		Util::showPositiveMessage("Registration Complete!", true);
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;
	} while (true);
}


////////////////////


void Receptionist::viewRoomDashboard() {
	DBConnection db;
	tabulate::Table table;

	try {
		db.prepareStatement("SELECT "
							"    RoomStatus, "
							"    r.RoomNumber AS RoomNumber, "
							"    RoomType, "
							"    Pax, "
							"    PricePerNight, "
							"    CASE "
							"        WHEN r.RoomStatus = 'Available' THEN NULL "
							"        ELSE bl.BookingID "
							"    END AS BookingID, "
							"    CASE "
							"        WHEN r.RoomStatus = 'Available' THEN NULL "
							"        ELSE bl.StartDate "
							"    END AS StartDate, "
							"    CASE "
							"        WHEN r.RoomStatus = 'Available' THEN NULL "
							"        ELSE bl.EndDate "
							"    END AS EndDate "
							" FROM room r "
							" LEFT JOIN (SELECT "
							"				bl1.BookingID,"
							"               bl1.RoomNumber, "
							"               bl1.StartDate, "
							"               bl1.EndDate, "
							"               bl1.BookingStatus "
							"           FROM bookingline bl1 "
							"           JOIN ( "
							"               SELECT RoomNumber, MAX(StartDate) AS LatestStartDate, MAX(BookingID) AS LatestBookingID"
							"               FROM bookingline "
							"               WHERE CURRENT_DATE BETWEEN StartDate AND EndDate "
							"               GROUP BY RoomNumber "
							"           ) bl2 ON bl1.RoomNumber = bl2.RoomNumber AND bl1.StartDate = bl2.LatestStartDate AND bl1.BookingID = bl2.LatestBookingID"
							"          ) bl ON r.RoomNumber = bl.RoomNumber "
							" AND CURRENT_DATE BETWEEN bl.StartDate AND bl.EndDate "
							" GROUP BY RoomNumber "
							" ORDER BY RoomNumber;");
		db.QueryResult();

		if (db.res->rowsCount() > 0) {
			std::string RoomStatus, RoomNumber, RoomType, Pax, PricePerNight, BookingID, StartDate, EndDate;

			// Add column headers
			table.add_row({ "RoomStatus", "RoomNumber", "RoomType", "Pax", "PricePerNight", "ID", "StartDate", "EndDate" });
			while (db.res->next()) {
				RoomStatus = db.res->getString("RoomStatus");
				RoomNumber = db.res->getString("RoomNumber");
				RoomType = db.res->getString("RoomType");
				Pax = db.res->getString("Pax");
				PricePerNight = "RM" + Util::formatCurrencyDecimal(db.res->getString("PricePerNight"));
				BookingID = db.res->getString("BookingID");
				StartDate = db.res->getString("StartDate");
				EndDate = db.res->getString("EndDate");

				table.add_row({ RoomStatus, RoomNumber, RoomType, Pax, PricePerNight, BookingID, StartDate, EndDate });
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

	// OUTPUT Table
	std::cout << "|\n";
	std::cout << "|\t" << ANSI_COLOR_YELLOW << "TODAY'S ROOM DASHBOARD" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 9);
	std::cout << "|\t----------------------\n";
	std::cout << "|\n";
	std::cout << table << '\n';
	std::cout << "|\n";
}


void Receptionist::renderCI_WalkIn(const std::vector<std::string>& rooms) {
	std::string bookingID;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Daily Functions -> Check-in (Walk-In)" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 6);
		std::cout << "|\t-------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\tChecking-in on: " << Util::writeRoomList(true, rooms);
		Util::showEscInstruction();
		std::cout << "|\n";
		std::cout << "|\tGuest Information\n";
		std::cout << "|\t-------------------\n";

		std::string tempArr[5];
		tempArr[0] = Util::parseICNumberInput();
		if (tempArr[0] == __EXIT_CODE__) { break; }

		if (Guest::isICNumberExist(tempArr[0])) {
			std::cout << "|\t" << ANSI_COLOR_GREEN << "[ A Returning Guest ]\n" << ANSI_COLOR_RESET;
		}
		else {
			std::cout << "|\t" << ANSI_COLOR_GREEN << "[ A New Guest ]\n" << ANSI_COLOR_RESET;
			std::cout << "|\n";
			tempArr[1] = Util::parseNameInput();
			if (tempArr[1] == __EXIT_CODE__) { break; }

			tempArr[2] = Util::parsePhoneNumberInput();
			if (tempArr[2] == __EXIT_CODE__) { break; }
		}
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tStay Duration\n";
		std::cout << "|\t-------------\n";
		std::cout << "|\tStarting today: " << ANSI_COLOR_GOLD << "" << Util::getCurrentDate() << "\n" << ANSI_COLOR_RESET;

		tempArr[3] = Util::getCurrentDate();

		tempArr[4] = Util::parseDateInput("Until", false, tempArr[3]);
		if (tempArr[4] == __EXIT_CODE__) { break; }

		try {
			DBConnection db;

			// 0. If Guest IC Number DOES NOT EXIST, insert it.
			if (!Guest::isICNumberExist(tempArr[0])) {
				db.prepareStatement("INSERT INTO Guest (ICNumber, Name, PhoneNo) VALUES (?,?,?);");
				db.stmt->setString(1, tempArr[0]);
				db.stmt->setString(2, tempArr[1]);
				db.stmt->setString(3, tempArr[2]);
				db.QueryStatement();
			}

			// 1. Add booking details
			db.prepareStatement("INSERT INTO Booking (ICNumber, StaffUsername, BookingType) VALUES (?,?,?)");
			db.stmt->setString(1, tempArr[0]);
			db.stmt->setString(2, getStaffUsername()); 
			db.stmt->setString(3, __BOOKING_TYPE_WALK_IN__);
			db.QueryStatement();

			// 2.1 Get the new ID from just now (based on ICNumber)
			db.prepareStatement("SELECT bookingID FROM Booking WHERE ICNumber = ?");
			db.stmt->setString(1, tempArr[0]);
			db.QueryResult();
			while (db.res->next()) {
				bookingID = db.res->getString("bookingID");
			}

			// 2.2 Now insert the bookingline, must have bookingID and at least ONE RoomNumber
			for (std::string room : rooms) {
				db.prepareStatement("INSERT INTO bookingline (RoomNumber, bookingID, StartDate, EndDate, BookingStatus)"
									" VALUES (?,?,?,?,?);");
				db.stmt->setString(1, room);
				db.stmt->setString(2, bookingID);
				db.stmt->setString(3, tempArr[3]);
				db.stmt->setString(4, tempArr[4]);
				db.stmt->setString(5, __BOOKING_STATUS_CHECKEDIN__);
				db.QueryStatement();
			}
			DBUtil::updateRoomStatuses();
			DBUtil::updateBookingNetPrice(bookingID);

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}
		std::cout << "|\n";
		Util::showPositiveMessage("Check-in Confirmed", true);
		Util::showBookingIDDisplay(bookingID);
		Util::showPressAnyKeyToContinue();
		Util::showRefreshCountdown();
		break;
	} while (true);
}


void Receptionist::renderCI_Reservation(std::string& bookingID) {
	DBConnection db;
	int action = 0;
	bool isContinue = true;
	std::vector<std::string> data;
	std::vector<std::string> rooms = DBUtil::getRoomNumbersByBookingID(bookingID);
	DBUtil::applyGuestBookingInfo_To(data, bookingID);

	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Daily Functions > Check-in (Reservation)" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 5);
		std::cout << "|\t----------------------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking ID: " << ANSI_COLOR_GOLD << data[0] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\t------------\n";
		std::cout << "|\tIC Number    : " << ANSI_COLOR_GOLD << data[1] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tName         : " << ANSI_COLOR_GOLD << data[2] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tPhone Number : " << ANSI_COLOR_GOLD << data[3] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tStart Date   : " << ANSI_COLOR_GOLD << data[4] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tEnd Date     : " << ANSI_COLOR_GOLD << data[5] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tDuration     : " << ANSI_COLOR_GOLD << data[6] << " days, " << data[7] << "nights\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Checking in to: " << ANSI_COLOR_GOLD << Util::writeRoomList(true, rooms) << ANSI_COLOR_RESET;;
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tConfirm check-in for this reservation?\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> NO <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_ORANGE : "") << "> YES <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ NO ]\n" : ANSI_COLOR_ORANGE "[ YES ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)
		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 2) % 2;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 2;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				isContinue = false;
			}
			else if (action == 1) {
				try {
					DBConnection db;
					db.prepareStatement("UPDATE bookingline"
										" SET BookingStatus = 'CheckedIn'"
										" WHERE BookingStatus = 'Reserved' AND StartDate = CURRENT_DATE AND BookingID = ?");
					db.stmt->setString(1, bookingID);
					db.QueryStatement();
				} catch (sql::SQLException& e) {
					std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
				}
				DBUtil::updateBookingStatuses();
				DBUtil::updateRoomStatuses();
				Util::showPositiveMessage("Check-in Confirm", true);
				Util::showHorizontalLine("double");
				Util::showRefreshCountdown();
				isContinue = false;
			}
			break;
		}
	} while (isContinue);
}


void Receptionist::renderCO(const std::string bookingID) {
	DBConnection db;
	int action = 0;
	bool isContinue = true;
	std::vector<std::string> data;
	std::vector<std::string> rooms = DBUtil::getRoomNumbersByBookingID(bookingID);
	DBUtil::applyGuestBookingInfo_To(data, bookingID);

	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Daily Functions > Check-out" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 7);
		std::cout << "|\t---------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking ID : " << ANSI_COLOR_GOLD << data[0] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\t---------------\n";
		std::cout << "|\tIC Number    : " << ANSI_COLOR_GOLD << data[1] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tName         : " << ANSI_COLOR_GOLD << data[2] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tPhone Number : " << ANSI_COLOR_GOLD << data[3] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tStart Date   : " << ANSI_COLOR_GOLD << data[4] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tEnd Date     : " << ANSI_COLOR_GOLD << data[5] << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tDuration     : " << ANSI_COLOR_GOLD << data[6] << " days, " << data[7] << " night(s)\n" << ANSI_COLOR_RESET;
		std::cout << "|\t--------------------------------------------\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Checking out of: " << ANSI_COLOR_GOLD << Util::writeRoomList(true, rooms);
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_ORANGE << "Confirm check-out?\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> NO <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_ORANGE : "") << "> YES <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ NO ]\n" : ANSI_COLOR_ORANGE "[ YES ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 2) % 2;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 2;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				isContinue = false;
			}
			else if (action == 1) {
				try {
					DBConnection db;
					db.prepareStatement("UPDATE bookingline"
										" SET BookingStatus = 'CheckedOut'"
										" WHERE BookingStatus = 'CheckedIn' AND BookingID = ?");
					db.stmt->setString(1, bookingID);
					db.QueryStatement();
				} 
				catch (sql::SQLException& e) {
					std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
				}
				DBUtil::updateBookingStatuses();
				DBUtil::updateRoomStatuses();
				Util::showPositiveMessage("Check-out Successful", true);
				// INVOICE
				renderInvoicePrompt(bookingID);
				// INVOICE
				Util::showHorizontalLine("single");
				std::cout << "|\n";
				Util::showPressAnyKeyToContinue();
				Util::showRefreshCountdown();
				isContinue = false;
			}
			break;
		}
	} while (isContinue);
}


void Receptionist::renderInvoicePrompt(const std::string& bookingID) {
	DBConnection db;
	int action = 0;

	std::vector<std::string> data;
	std::vector < std::vector<std::string> > rooms;

	DBUtil::applyFullBookingInfo_To(data, rooms, bookingID);
	std::string invoicePreview_String = Util::generateInvoice_AsString(data, rooms);

	Util::showHorizontalLine("single");
	std::cout << "|\n";
	std::cout << "|\t" << ANSI_COLOR_YELLOW << "Invoice Preview :\n" << ANSI_COLOR_RESET;
	std::cout << "|\t----------------\n";
	std::cout << "|\n";
	std::cout << "|\t";
	for (char c : invoicePreview_String) {
		std::cout << ANSI_COLOR_GOLD << c;
		if (c == '\n') {
			std::cout << ANSI_COLOR_RESET << "|\t";
		}
	}
	std::cout << "|\n";
	std::cout << "|\n";




	/// DRAFTED CODE FOR TEXT PRINTING OUTPUT FOR INVOICE
	/*std::cout << "|\tPrint out invoice?\n";
	std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> NO <\n" << ANSI_COLOR_RESET;
	std::cout << "|\t" << (action == 1 ? ANSI_COLOR_ORANGE : "") << "> YES <\n" << ANSI_COLOR_RESET;
	std::cout << "|\n";
	std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
	std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ NO ]\n" : ANSI_COLOR_ORANGE "[ YES ]\n") << ANSI_COLOR_RESET;
	std::cout << "|\n";
	Util::showHorizontalLine("double");

	int key = _getch(); // To collect single key input (Windows specific)
	switch (key) {
	case 72: // Up arrow key code = 72
		action = (action - 1 + 2) % 2;
		break;
	case 80: // Down arrow key code = 80
		action = (action + 1) % 2;
		break;
	case 13:  // Enter key code = 13
		if (action == 0) {
			break;
		}
		if (action == 1) {
			//Util::generatePDF(invoicePreview_String, "HRRS INVOICE BOOKING ID " + bookingID);
			break;
		}
	}
	*/
}


void Receptionist::renderDailyFunctionsMenu() {
	bool isExiting = false;
	std::string action;

	do {
		system("cls");
		Util::showHorizontalLine("double");
		viewRoomDashboard();
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Daily Functions" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 8);
		std::cout << "|\t---------------------------\n";
		std::cout << "|\n";
		std::cout << "|\tEnter a room number and a number to perform action:\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "1" << ANSI_COLOR_RESET << " Check-in (Walk-In)\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "2" << ANSI_COLOR_RESET << " Check-in (Reservation)\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "3" << ANSI_COLOR_RESET << " Check-out\n";
		std::cout << "|\t-------------------------------\n";
		Util::showEscInstruction();
		std::cout << "|\n";
		std::cout << "|\n";
		do {
			action = Util::parseTextInput("Action");
			if (action == __EXIT_CODE__) { isExiting = true; break; }
			else if (!(action == "1" || action == "2" || action == "3")) { Util::showInvalidAction(); std::cout << "|\n"; }
			else { break; }
		} while (true); // Inner do-while for "Action" input validation

		if (isExiting) { break; }

		if (action == "1") {
			std::cout << "|\n";
			std::cout << "|\n";
			std::cout << "|\t" << ANSI_COLOR_GOLD << "1 " << ANSI_COLOR_RESET << "Check-in (Walk-in)\n";
			std::cout << "|\t--------------------\n";
			std::cout << "|\tEnter a room number or multiple (use dashes \"-\") for walk-in\n";
			std::cout << "|\tExample: " << ANSI_COLOR_GOLD << "R005" << ANSI_COLOR_RESET << " or " << ANSI_COLOR_GOLD << "R001-R015-R006\n" << ANSI_COLOR_RESET;
			Util::showEscInstruction();
			std::cout << "|\n";
			do {
				std::string roomsRawInput = Util::parseTextInput("Rooms");
				if (roomsRawInput == __EXIT_CODE__) { break; }

				// String splitting
				std::vector<std::string> temp = Util::split(roomsRawInput, '-');
				std::vector<std::string> rooms(temp.begin(), temp.end());

				if (rooms.size() <= 0) { // If there are no room input
					Util::showNegativeMessage("You must enter at least 1 room number");
				}
				else if (!DBUtil::isRoomNumberExist(rooms)) { // If does NOT exist
					Util::showNegativeMessage("One or more selected rooms do not exist");
				}
				else if (!DBUtil::isRoomAvailable(rooms)) { // If is NOT available
					Util::showNegativeMessage("One or more selected rooms are not available at the moment");
				}
				else {
					renderCI_WalkIn(rooms);
					break;
				}
				std::cout << "|\n";
			} while (true);
		}
		else if (action == "2") {
			std::cout << "|\n";
			std::cout << "|\n";
			std::cout << "|\t" << ANSI_COLOR_GOLD << "2 " << ANSI_COLOR_RESET << "Check-in (Reservation)\n";
			std::cout << "|\t------------------------\n";
			Util::showEscInstruction();
			do {
				std::string bookingID = Util::parseTextInput("Booking ID");
				if (bookingID == __EXIT_CODE__) { break; }

				if (!DBUtil::isBookingIDExist(bookingID)) { // Does not Exist
					Util::showNegativeMessage("Booking ID is not found");
					std::cout << "|\n";
				}
				else if (!DBUtil::isBookingReserved(bookingID)) {
					Util::showNegativeMessage("This booking is not in reservation");
					std::cout << "|\n";
				}
				else if (!DBUtil::isReservationOnToday(bookingID)) {
					Util::showNegativeMessage("This reservation does not start today");
					std::cout << "|\n";
				}
				else {
					renderCI_Reservation(bookingID);
					break;
				}
			} while (true);
		}
		else if (action == "3") {
			std::cout << "|\n";
			std::cout << "|\n";
			std::cout << "|\t" << ANSI_COLOR_GOLD << "3 " << ANSI_COLOR_RESET << "Check-out\n";
			std::cout << "|\t-----------\n";
			Util::showEscInstruction();
			do {
				std::string bookingID = Util::parseTextInput("Booking ID");
				if (bookingID == __EXIT_CODE__) { break; }

				if (!DBUtil::isBookingIDExist(bookingID)) { // Does not Exist
					Util::showNegativeMessage("Booking ID is not found");
					std::cout << "|\n";
				}
				else if (!DBUtil::isBookingCheckedIn(bookingID)) {
					Util::showNegativeMessage("This booking is not checked in yet");
					std::cout << "|\n";
				}
				else {
					renderCO(bookingID);
					break;
				}
			} while (true);
		}
		std::cout << "|\n";
	} while (true);
}


//////////////////////////


void Receptionist::renderBookingHistory() {
	DBConnection db;
	bool isExiting = false;

	do {
		tabulate::Table table = DBUtil::getTable_BookingHistoryByICNumber();
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Booking History" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 8);
		std::cout << "|\t---------------------------\n";
		std::cout << "|\n";
		if (table.size() > 0) {
			std::cout << table << "\n";
			std::cout << "|\t" << ANSI_COLOR_GOLD << table.size() - 1 << " booking(s)\n" << ANSI_COLOR_RESET;
			std::cout << "|\n";
			std::cout << "|\n";
			Util::showEscInstruction();
			std::cout << "|\tEnter a Booking ID to " << ANSI_COLOR_YELLOW << "view details\n" << ANSI_COLOR_RESET;

			std::string bookingID;
			do {
				bookingID = Util::parseTextInput("Booking ID");
				if (bookingID == __EXIT_CODE__) { isExiting = true; break; }

				if (!DBUtil::isBookingIDExist(bookingID)) {
					Util::showNegativeMessage("Booking ID does not exist");
					std::cout << "|\n";
				}
				else {
					Receptionist::viewBookingDetailsOf(bookingID);
					break;
				}
			} while (true);
		}
		else {
			Util::showPassiveMessage("You don't have any bookings yet so far.");
			std::cout << "|\n";
			std::cout << "|\n";
			Util::showHorizontalLine("single");
			std::cout << "|\n";
			Util::showPressAnyKeyToContinue();
			isExiting = true;
		}
		if (isExiting) { break; }
	} while (true);
}


void Receptionist::viewBookingDetailsOf(const std::string& bookingID) {
	tabulate::Table table;
	DBConnection db;
	std::string RoomNumber, RoomType, PricePerNight, StartDate, EndDate, CancelledDate, BookingStatus, TotalDays, TotalNights, NetPrice;

	do {
		// 1. Make query
		db.prepareStatement("SELECT bl.RoomNumber, r.RoomType, r.PricePerNight, bl.StartDate, bl.EndDate, bl.CancelledDate, bl.BookingStatus, DATEDIFF(bl.EndDate, bl.StartDate)+1 AS TotalDays, DATEDIFF(bl.EndDate, bl.StartDate) AS TotalNights, b.NetPrice"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" LEFT JOIN booking b ON bl.BookingID = b.BookingID"
							" WHERE bl.BookingID = ?");
		db.stmt->setString(1, bookingID);
		db.QueryResult();

		// 2. Add to table
		table.add_row({ "RoomNumber", "RoomType", "PricePerNight", "StartDate", "EndDate", "CancelledDate", "BookingStatus" });
		while (db.res->next()) {
			RoomNumber = db.res->getString("RoomNumber");
			RoomType = db.res->getString("RoomType");
			PricePerNight = "RM" + Util::formatCurrencyDecimal(db.res->getString("PricePerNight"));
			StartDate = db.res->getString("StartDate");
			EndDate = db.res->getString("EndDate");
			CancelledDate = db.res->getString("CancelledDate");
			BookingStatus = db.res->getString("BookingStatus");
			table.add_row({ RoomNumber, RoomType, PricePerNight, StartDate, EndDate, CancelledDate, BookingStatus });

			// These below are not shown in table
			TotalDays = db.res->getString("TotalDays");
			TotalNights = db.res->getString("TotalNights");
			NetPrice = Util::formatCurrencyDecimal(db.res->getString("NetPrice"));
			///
		}
		for (int i = 0; i <= db.res->rowsCount(); i++) {
			table[i][0].format()
				.border_left("|\t|")
				.corner("|\t+");
		}

		// 3. OUTPUT Table
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Booking History > Booking Details " << ANSI_COLOR_RESET << Util::writeTodayDate(true, 6);
		std::cout << "|\t---------------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking ID   : " << ANSI_COLOR_GOLD << bookingID << "\n" << ANSI_COLOR_RESET;
		std::cout << table << '\n';
		std::cout << "|\tTotal nights : " << ANSI_COLOR_GOLD << TotalNights << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tNet Price    : " << ANSI_COLOR_GOLD << "RM" << (empty(NetPrice) ? "0" : NetPrice) << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\t------------------------\n";
		std::cout << "|\n";
		Util::showPressAnyKeyToContinue();
		Util::showHorizontalLine("single");
		break;
	} while (true);
}


///////////////////////////


void Receptionist::renderMainMenu() {
	int action = 0;
	bool isContinue = true;

	//setStaffUsername("frontdesk1");   /////////////
	//setPassword("ks|sYHB+7");       /////////////

	do {
		system("cls");
		Util::showHorizontalLine("double"); 
		Util::showLogHeading(getName(), getUserType(), getStaffUsername());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 10);
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Daily Functions <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_GOLD : "") << "> Booking History <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 2 ? ANSI_COLOR_ORANGE : "") << "> LOG OUT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ Daily Functions ]\n" : action == 1 ? "[ Booking History ]\n" : ANSI_COLOR_ORANGE "[ LOG OUT ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 3) % 3;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 3;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				renderDailyFunctionsMenu();
			}
			if (action == 1) {
				renderBookingHistory();
			}
			if (action == 2) {
				isContinue = false;
				Util::showRefreshCountdown();
				break;
			}
		}
	} while (isContinue);
}


