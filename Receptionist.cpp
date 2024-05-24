#include "Receptionist.h"
#include "Guest.h"


void Receptionist::renderRegisterPrompt() {
	do {
		system("cls");

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Register as Receptionist\n" << ANSI_COLOR_RESET;
		Util::showInputCancelInstruction();
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

		Util::showPositiveMessage("Registration Complete!");
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;
	} while (true);
}


////////////////////


void Receptionist::viewRoomDashboard(DBConnection& db) {
	tabulate::Table table;

	try {
		db.prepareStatement("SELECT "
							" RoomStatus,"
							" r.RoomNumber AS RoomNumber,"
							" RoomName,"
							" Pax,"
							" PricePerNight,"
							" b.StartDate AS StartDate,"
							" b.EndDate AS EndDate"
							" FROM room r LEFT JOIN bookingline b ON r.RoomNumber = b.RoomNumber"
							" ORDER BY RoomNumber");
		db.QueryResult();

		if (db.res->rowsCount() > 0) {
			std::string RoomStatus, RoomNumber, RoomName, Pax, PricePerNight, StartDate, EndDate;

			// Add column headers
			table.add_row({ "RoomStatus", "RoomNumber", "RoomName", "Pax", "PricePerNight", "StartDate", "EndDate" });
			while (db.res->next()) {
				RoomStatus = db.res->getString("RoomStatus");
				RoomNumber = db.res->getString("RoomNumber");
				RoomName = db.res->getString("RoomName");
				Pax = db.res->getString("Pax");
				PricePerNight = Util::truncateDecimal(db.res->getString("PricePerNight"));
				StartDate = db.res->getString("StartDate");
				EndDate = db.res->getString("EndDate");

				table.add_row({ RoomStatus, RoomNumber, RoomName, Pax, PricePerNight, StartDate, EndDate });
			}
			for (int i = 0; i <= db.res->rowsCount(); i++) {
				table[i][0].format()
					.border_left("|\t|")
					.border_right("\b")  //// TODO: Fix Border
					.corner_top_left("|\t+")
					.corner_bottom_left("|\t+");
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}

	// OUTPUT Table
	std::cout << "|\n";
	std::cout << "|\t" << ANSI_COLOR_YELLOW << "TODAY'S ROOM DASHBOARD" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 8);
	std::cout << "|\t----------------------\n";
	std::cout << "|\n";
	std::cout << table << '\n';
	std::cout << "|\n";
}


void Receptionist::renderCI_Reservation(const std::vector<std::string>& rooms, DBConnection dbObj) {
	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Check-in / Check-out -> Check-in (Reservation)" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 6);
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tChecking-in on: " << ANSI_COLOR_GOLD;
		for (int i = 0; i < rooms.size(); i++) {
			std::cout << rooms[i];
			if (i != rooms.size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << ANSI_COLOR_RESET << "\n";
		Util::showInputCancelInstruction();
		std::cout << "|\n";
		std::cout << "|\tGuest Information\n";
		std::cout << "|\t-------------------\n";
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tGuest Name and stuff\n";
		std::cout << "|\n";
		std::cout << "|\n";



		try {
			DBConnection db;

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		Util::showPositiveMessage("Booking Confirmed");
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;
	} while (true);
}


void Receptionist::renderCI_WalkIn(const std::vector<std::string>& rooms, DBConnection dbObj) {
	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Check-in / Check-out -> Check-in (Walk-In)" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 6);
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tChecking-in on: " << ANSI_COLOR_GOLD;
		for (int i = 0; i < rooms.size(); i++) {
			std::cout << rooms[i];
			if (i != rooms.size() - 1) { 
				std::cout << ", "; 
			}
		}
		std::cout << ANSI_COLOR_RESET << "\n";
		Util::showInputCancelInstruction();
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
			db.stmt->setString(2, "frontdesk1");
			//db.stmt->setString(2, getStaffUsername()); 
			db.stmt->setString(3, "Walk-in");
			db.QueryStatement();

			// 2.1 Get the new ID from just now (based on ICNumber)
			db.prepareStatement("SELECT bookingID FROM Booking WHERE ICNumber = ?");
			db.stmt->setString(1, tempArr[0]);
			db.QueryResult();
			std::string bookingID;
			while (db.res->next()) {
				bookingID = db.res->getString("bookingID");
			}

			// 2.2 Now insert the bookingline, must have bookingID and at least ONE RoomNumber
			for (int i = 0; i < rooms.size(); i++) {
				db.prepareStatement("INSERT INTO bookingline (RoomNumber, bookingID, StartDate, EndDate, BookingStatus)"
									" VALUES (?,?,?,?,?);");
				db.stmt->setString(1, rooms[i]); 
				db.stmt->setString(2, bookingID);
				db.stmt->setString(3, tempArr[3]);
				db.stmt->setString(4, tempArr[4]);
				db.stmt->setString(5, "Checked-in");
				db.QueryStatement();
			}
			DBUtil::updateRoomStatuses();
			DBUtil::updateBookingNetPrice(bookingID);

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		Util::showPositiveMessage("Booking Confirmed");
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;
	} while (true);

}


void Receptionist::renderCO(const std::vector<std::string>& rooms, DBConnection dbObj) {
	std::cout << "|\tCheck-out\n";
}


void Receptionist::renderMenuOfCICO() {
	DBConnection db;
	bool isExiting = false;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		viewRoomDashboard(db);
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Check-In / Check-Out" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 8);
		std::cout << "|\t--------------------\n";
		std::cout << "|\n";
		std::cout << "|\tEnter a room number and a number to perform action:\n";
		std::cout << "|\tExample: " << ANSI_COLOR_GOLD << "1-R005" << ANSI_COLOR_RESET << " or multiple " << ANSI_COLOR_GOLD << "3-R001-R015-R006\n" << ANSI_COLOR_RESET;
		Util::showInputCancelInstruction();
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "1-[Room Number]" << ANSI_COLOR_RESET << " Check-in (Reservation)\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "2-[Room Number]" << ANSI_COLOR_RESET << " Check-in (Walk-In)\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "3-[Room Number]" << ANSI_COLOR_RESET << " Check-out\n";
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "00" << ANSI_COLOR_RESET << " Back to previous page\n";
		std::cout << "|\n";
		std::cout << "|\n";

		do {
			std::string input = Util::parseTextInput();
			if (input == __EXIT_CODE__) { isExiting = true; break; }

			// String splitting
			std::vector<std::string> temp = Util::split(input, '-');
			std::vector<std::string> rooms(temp.begin() + 1, temp.end());
			std::string action = temp[0];

			if (action == "00") {
				isExiting = false;
				break;
			}

			if (rooms.size() <= 0) { // If there are no room input
				Util::showNegativeMessage("You must enter at least 1 room number");
			}
			else if (!DBUtil::isRoomNumberExist(rooms, db)) { // If does NOT exist
				Util::showNegativeMessage("Room(s) do not exist");
			}
			else if (!DBUtil::isRoomAvailable(rooms, db)) { // If is NOT available
				Util::showNegativeMessage("The selected room(s) are unavailable at the moment");
			}
			else {
				if (action == "1") {
					//renderCI_Reservation(rooms, db);
					isExiting = true;
				}
				else if (action == "2") {
					renderCI_WalkIn(rooms, db);
					isExiting = true;
					break;
				}
				else if (action == "3") {
					//renderCO(rooms, db);
					isExiting = true;
					break;
				}
			}
			std::cout << "|\n";
		} while (true);

		if (isExiting) {
			break;
		}

	} while (true);
}


//////////////////////////


void Receptionist::renderBookingHistory() {
	tabulate::Table table;
	DBConnection db;
	std::string BookingID, ICNumber, Name, NetPrice, BookingType;

	do {
		try {
			db.prepareStatement("SELECT b.BookingID, g.ICNumber, g.Name, b.NetPrice, b.BookingType"
								" FROM booking b LEFT JOIN guest g ON b.ICNumber = g.ICNumber;");
			db.QueryResult();

			if (db.res->rowsCount() > 0) {

				// Add column headers
				table.add_row({ "BookingID", "ICNumber", "Name", "NetPrice", "BookingType"});
				while (db.res->next()) {
					BookingID = db.res->getString("BookingID");
					ICNumber = db.res->getString("ICNumber");
					Name = db.res->getString("Name");
					NetPrice = Util::truncateDecimal(db.res->getString("NetPrice"));
					BookingType = db.res->getString("BookingType");

					table.add_row({ BookingID, ICNumber, Name, NetPrice, BookingType});
				}
				for (int i = 0; i <= db.res->rowsCount(); i++) {
					table[i][0].format()
						.border_left("|\t|")
						.border_right("\b")  //// TODO: Fix Border
						.corner_top_left("|\t+")
						.corner_bottom_left("|\t+");
				}
			}
		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		// OUTPUT Table
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking History" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 9);
		std::cout << "|\t---------------\n";
		std::cout << "|\n";
		std::cout << table << '\n';
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tEnter a Booking ID to view details\n";
		Util::showInputCancelInstruction();
		std::cout << "|\n";
		std::string input = Util::parseTextInput();

		if (input == __EXIT_CODE__) { break; }

		if (!DBUtil::isBookingIDExist(input)) {
			Util::showNegativeMessage("Booking ID does not exist");
		}
		else {
			Receptionist::viewBookingDetailsOf(input);
		}
		table = tabulate::Table();
	} while (true);
}


void Receptionist::viewBookingDetailsOf(const std::string& bookingID) {
	tabulate::Table table;
	DBConnection db;

	std::string RoomNumber, RoomName, PricePerNight, StartDate, EndDate, CancelledDate, BookingStatus, TotalNights, NetPrice;

	do {
		// 1. Make query
		db.prepareStatement("SELECT bl.RoomNumber, r.RoomName, r.PricePerNight, bl.StartDate, bl.EndDate, bl.CancelledDate, bl.BookingStatus, DATEDIFF(bl.EndDate, bl.StartDate)-1 AS TotalNights, b.NetPrice"
							" FROM bookingline bl"
							" LEFT JOIN room r ON bl.RoomNumber = r.RoomNumber"
							" LEFT JOIN booking b ON bl.BookingID = b.BookingID"
							" WHERE bl.BookingID = ?");
		db.stmt->setString(1, bookingID);
		db.QueryResult();


		// 2. Add to table
		table.add_row({ "RoomNumber", "RoomName", "PricePerNight", "StartDate", "EndDate", "CancelledDate", "BookingStatus"});
		while (db.res->next()) {
			RoomNumber = db.res->getString("RoomNumber");
			RoomName = db.res->getString("RoomName");
			PricePerNight = Util::formatCurrencyDecimal(db.res->getString("PricePerNight"));
			StartDate = db.res->getString("StartDate");
			EndDate = db.res->getString("EndDate");
			CancelledDate = db.res->getString("CancelledDate");
			BookingStatus = db.res->getString("BookingStatus");

			// These 2 are not shown in table
			TotalNights = db.res->getString("TotalNights");
			NetPrice = Util::formatCurrencyDecimal(db.res->getString("NetPrice"));
			///

			table.add_row({ RoomNumber, RoomName, PricePerNight, StartDate, EndDate, CancelledDate, BookingStatus });
		}
		for (int i = 0; i <= db.res->rowsCount(); i++) {
			table[i][0].format()
				.border_left("|\t|")
				.border_right("\b")  //// TODO: Fix Border
				.corner_top_left("|\t+")
				.corner_bottom_left("|\t+");
		}

		// 3. OUTPUT Table
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking History -> Booking Details " << ANSI_COLOR_RESET << Util::writeTodayDate(true, 7);
		std::cout << "|\t---------------------------------\n";
		std::cout << "|\n";
		std::cout << table << '\n';
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Booking ID : " << bookingID << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tTotal nights : " << ANSI_COLOR_YELLOW << TotalNights << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\tNet Price    : " << ANSI_COLOR_YELLOW << NetPrice << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showInputCancelInstruction();
		std::cout << "|\n";
		std::string input = Util::parseTextInput();
		if (input == __EXIT_CODE__) { break; }
	} while (true);
	std::cout << "|\n";
	Util::showHorizontalLine("single");
}


///////////////////////////


void Receptionist::renderMainMenu() {
	int action = 0;
	bool isContinue = true;
	do {
		system("cls");
		Util::showHorizontalLine("double"); 
		Util::showLogHeading(getName(), getStaffUsername(), getUserType());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 10);
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Make Reservation <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_GOLD : "") << "> Check-in / Check-out <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 2 ? ANSI_COLOR_GOLD : "") << "> Booking History <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 3 ? ANSI_COLOR_ORANGE : "") << "> LOG OUT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ Make Reservation ]\n" : action == 1 ? "[ Check-in / Check-out ]\n" : action == 2 ? "[ Booking History ]\n" : ANSI_COLOR_ORANGE "[ LOG OUT ]\n") << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");

		int key = _getch(); // To collect single key input (Windows specific)

		switch (key) {
		case 72: // Up arrow key code = 72
			action = (action - 1 + 4) % 4;
			break;
		case 80: // Down arrow key code = 80
			action = (action + 1) % 4;
			break;
		case 13:  // Enter key code = 13
			if (action == 0) {
				//renderReservationSelection();
			}
			if (action == 1) {
				renderMenuOfCICO();
			}
			if (action == 2) {
				renderBookingHistory();
			}
			if (action == 3) {
				action = 0;
				isContinue = false; // Go to previous page
				break;
			}
		default:
			Util::showInvalidAction();
		}
	} while (isContinue);
}


