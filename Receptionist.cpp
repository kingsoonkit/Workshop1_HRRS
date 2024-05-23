#include "Receptionist.h"
#include "Guest.h"


void Receptionist::renderRegisterPrompt() {
	do {
		system("cls");

		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << "Register as Receptionist\n" << ANSI_COLOR_RESET;
		std::cout << "|\t(Enter \"esc\" to return to previous page)\n";
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";

		std::string tempArr[4];

		std::cout << "|\tUsername will be used for login.\n";
		do {
			tempArr[0] = Util::parseUsernameInput();
			if (tempArr[0] == __EXIT_CODE__) { break; }
			if (Staff::isUsernameExist(tempArr[0])) {
				std::cout << "|\t" << ANSI_COLOR_RED << "This username already exists. Try another one\n" << ANSI_COLOR_RESET;
				std::cout << "|\n";
			}
			else {
				std::cout << "|\t" << ANSI_COLOR_GREEN << "[OK]\n" << ANSI_COLOR_RESET;
				std::cout << "|\t-----------------------------------\n";
				break;
			}
		} while (true);

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


void Receptionist::displayRoomDashboard(DBConnection& db) {
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
							" FROM room r LEFT JOIN bookingline b ON r.RoomNumber = b.RoomNumber");
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


void Receptionist::renderCI_Reservation(const std::vector<std::string>& roomNumber, DBConnection dbObj) {
	std::cout << "|\tCheck-in for reservation\n";
}


void Receptionist::renderCI_WalkIn(const std::vector<std::string>& roomNumber, DBConnection dbObj) {

	do {
		std::cout << "| ----------------------------------------------------------------------------------------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\tCheck-In / Check-Out -> Check-in (Walk-In)\n";
		std::cout << "|\t-----------------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\tGuest’s Information\n";
		std::cout << "|\t-------------------\n";
		std::string tempArr[5];

		tempArr[0] = Util::parseICNumberInput();
		if (tempArr[0] == __EXIT_CODE__) { break; }

		tempArr[1] = Util::parseNameInput();
		if (tempArr[1] == __EXIT_CODE__) { break; }

		tempArr[2] = Util::parsePhoneNumberInput();
		if (tempArr[2] == __EXIT_CODE__) { break; }

		std::cout << "|\tStay Duration\n";
		tempArr[3] = Util::parseDateInput();
		if (tempArr[3] == __EXIT_CODE__) { break; }

		tempArr[4] = Util::parseDateInput();
		if (tempArr[4] == __EXIT_CODE__) { break; }


		std::cout << "|\t-------------------\n";


	} while (true);




	DBConnection db;
	std::string query;
	if (Guest::isICNumberExist(tempArr[0])) {
		query = "";
	}
	else {
		query = "";
	}

	db.prepareStatement(query);


	Util::showPositiveMessage("Booking Confirmed");
	Util::showHorizontalLine("double");
	Util::showRefreshCountdown();
}


void Receptionist::renderCO(const std::vector<std::string>& roomNumber, DBConnection dbObj) {
	std::cout << "|\tCheck-out\n";
}


void Receptionist::renderMenuOfCICO() {
	DBConnection db;

	system("cls");
	Util::showHorizontalLine("double");
	displayRoomDashboard(db);
	Util::showHorizontalLine("single");
	std::cout << "|\n";
	std::cout << "|\tCheck-In / Check-Out\n";
	std::cout << "|\t--------------------\n";
	std::cout << "|\n";
	std::cout << "|\tEnter a room number and a number to perform action:\n";
	std::cout << "|\tExample: " << ANSI_COLOR_GOLD << "1-R005" << ANSI_COLOR_RESET << " or " << ANSI_COLOR_GOLD << "3-R001\n" << ANSI_COLOR_RESET;
	std::cout << "|\t(Enter \"esc\" to return to previous page)\n";
	std::cout << "|\t" << ANSI_COLOR_GOLD << "1" << ANSI_COLOR_RESET << " Check-in (Reservation)\n";
	std::cout << "|\t" << ANSI_COLOR_GOLD << "2" << ANSI_COLOR_RESET << " Check-in (Walk-In)\n";
	std::cout << "|\t" << ANSI_COLOR_GOLD << "3" << ANSI_COLOR_RESET << " Check-out\n";
	std::cout << "|\t-------------------------------\n";
	std::cout << "|\t" << ANSI_COLOR_GOLD << "00" << ANSI_COLOR_RESET << " Back to previous page\n";
	std::cout << "|\n";
	std::cout << "|\n";
	do {
		std::string input = Util::parseTextInput();
		if (input == __EXIT_CODE__) { break; }

		// String splitting
		std::vector<std::string> temp = Util::split(input, '-');
		std::vector<std::string> rooms(temp.begin() + 1, temp.end());
		std::string action = temp[0];

		if(action == "00") {
			break;
		}

		if (!Util::isRoomNumberExist(rooms, db)) {
			std::cout << "|\t" << ANSI_COLOR_RED << "Room(s) do not exist\n" << ANSI_COLOR_RESET;
			std::cout << "|\n";
		}
		else {
			if (action == "1") {
				renderCI_Reservation(rooms, db);
			}
			else if (action == "2") {
				renderCI_WalkIn(rooms, db);
			}
			else if (action == "3") {
				renderCO(rooms, db);
			}
		}
	} while (true);
	Util::showHorizontalLine("single");
}



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
				//renderBookingHistorySelection();
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




//std::cout << "|\t-------------------------------------------------------------------------------------------------\n";
//std::cout << "|\t| Room Status  | Room Number\t| Room Name\t|\tPax\t|  Start Date\t| End Date\t|\n";
//std::cout << "|\t-------------------------------------------------------------------------------------------------\n";
//std::cout << "|\t| Available    | R001\t\t| Single Room\t|\t1\t|\t-\t|\t\t|\n";
//std::cout << "|\t| Occupied     | R002\t\t| Single Room\t|\t1\t|  3/4/2024\t|  4/4/2023\t|\n";
//std::cout << "|\t| Overdue   [0]| R003\t\t| Double Room\t|\t2\t|  1/4/2024\t|  3/4/2024\t|\n";
//std::cout << "|\t| Available    | R004\t\t| Double Room\t|\t2\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Available    | R005\t\t| Queen Room\t|\t2\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Available    | R006\t\t| Queen Room\t|\t2\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Occupied     | R007\t\t| Deluxe Room\t|\t3\t|  3/4/2024\t|  5/4/2024 \t|\n";
//std::cout << "|\t| Occupied     | R008\t\t| Deluxe Room\t|\t3\t|  3/4/2024\t|  5/4/2024\t|\n";
//std::cout << "|\t| Reserved  [+]| R009\t\t| Deluxe Room\t|\t3\t|  4/4/2024\t|  6/4/2024\t|\n";
//std::cout << "|\t| Available    | R010\t\t| Deluxe Room\t|\t3\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Available    | R011\t\t| Deluxe Room\t|\t3\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Available    | R012\t\t| Single Room\t|\t1\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Due Out   [#]| R013\t\t| Single Room\t|\t1\t|  3/4/2024\t|  4/4/2024\t|\n";
//std::cout << "|\t| Available    | R014\t\t| King Room\t|\t2\t|\t-\t|\t-\t|\n";
//std::cout << "|\t| Reserved  [+]| R015\t\t| King Room\t|\t2\t|  4/4/2024\t|  6/4/2024\t|\n";
//std::cout << "|\t-------------------------------------------------------------------------------------------------	\n";