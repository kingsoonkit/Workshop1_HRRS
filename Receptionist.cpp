#include "Receptionist.h"


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

		tempArr[0] = Util::parseUsernameInput();
		if (tempArr[0] == __EXIT_CODE__) { break; }

		tempArr[1] = Util::parseNameInput();
		if (tempArr[1] == __EXIT_CODE__) { break; }

		tempArr[2] = Util::parsePhoneNumberInput();
		if (tempArr[2] == __EXIT_CODE__) { break; }

		setUserType("Receptionist");

		tempArr[3] = Util::hashText(Util::parsePasswordInput(false, true));
		if (tempArr[3] == __EXIT_CODE__) { break; }


		try {
			DBConnection db;
			db.prepareStatement("INSERT INTO Staff (StaffUsername, Name, PhoneNo, UserType, Password) VALUES (?,?,?,?,?)");
			db.stmt->setString(1, tempArr[0]);
			db.stmt->setString(2, tempArr[1]);
			db.stmt->setString(3, tempArr[2]);
			db.stmt->setString(4, getUserType());
			db.stmt->setString(5, tempArr[3]);
			db.QueryStatement();

		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}

		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_GREEN << "[ Registration Complete! ]\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();

		break;
	} while (true);
}


void Receptionist::renderCI_Reservation(const std::string& roomNumber, DBConnection dbObj) {
	std::cout << "|\tCheck-in for reservation\n";
}


void Receptionist::renderCI_WalkIn(const std::string& roomNumber, DBConnection dbObj) {
	while (dbObj.res->next()) {
		if (roomNumber == dbObj.res->getString("RoomNumber")) {
			if (dbObj.res->getString("RoomStatus") == "Available") {


				/*std::string tempArr[4];

				tempArr[0] = Util::parseUsernameInput();
				if (tempArr[0] == __EXIT_CODE__) { break; }

				tempArr[1] = Util::parseNameInput();
				if (tempArr[1] == __EXIT_CODE__) { break; }

				tempArr[2] = Util::parsePhoneNumberInput();
				if (tempArr[2] == __EXIT_CODE__) { break; }

				setUserType("Guest");

				tempArr[3] = Util::hashText(Util::parsePasswordInput(false, true));
				if (tempArr[3] == __EXIT_CODE__) { break; }*/


				//  GUEST ASSOCIATION PROBLEM   //



				DBConnection db;
				db.prepareStatement("UPDATE room SET RoomStatus = ? WHERE RoomNumber = ?");
				db.stmt->setString(1, "Available");
				db.stmt->setString(1, roomNumber);
				db.QueryStatement();


				std::cout << "|\t" << ANSI_COLOR_GREEN << "[ Checked-in: " << roomNumber << " for " << name << "]\n" << ANSI_COLOR_RESET;
			}
			std::cout << "|\t" << ANSI_COLOR_RED << "Room is unavailable at the moment\n" << ANSI_COLOR_RESET;
			break;
		}
	}
	std::cout << "|\t" << ANSI_COLOR_RED << "Room does not exist" << ANSI_COLOR_RESET;
}


void Receptionist::renderCO(const std::string& roomNumber, DBConnection dbObj) {
	std::cout << "|\tCheck-out\n";
}


void Receptionist::renderCICOMenu() {
	DBConnection db;
	db.prepareStatement("SELECT "
						" RoomStatus,"
						" r.RoomNumber AS RoomNumber,"
						" RoomName,"
						" Pax,"
						" PricePerNight,"
						" b.StartDate AS StartDate,"
						" b.EndDate AS EndDate"
						" FROM room r LEFT JOIN booking b ON r.RoomNumber = b.RoomNumber");
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		tabulate::Table table;
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

		std::cout << "|\n";
		std::cout << "|\t[  " << ANSI_COLOR_YELLOW << "TODAY'S ROOM DASHBOARD" << ANSI_COLOR_RESET << "  ]                       Today: " << Util::getCurrentDate() << "\n";
		std::cout << "|\t----------------------------\n";
		std::cout << "|\n";
		std::cout << table << '\n';
		//std::cout << "|\n";
		//std::cout << "|\t[+] : Checking in today\n";
		//std::cout << "|\t[#] : Checking out today\n";
		//std::cout << "|\t[0] : Check-out overdue\n";
		std::cout << "|\n";
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\tEnter a room number and a number to perform action:\n";
		std::cout << "|\tExample: 1 R099\n";
		std::cout << "|\t1. Check-in (Self-reservation)\n";
		std::cout << "|\t2. Check-in (Walk-In)\n";
		std::cout << "|\t3. Check-out\n";
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t00. Back to previous page\n";
		std::cout << "|\n";
		std::cout << "|\n";
		do {
			std::string action;
			std::cout << "|\tAction-#: " << ANSI_COLOR_GOLD; std::cin >> action;
			std::cout << ANSI_COLOR_RESET;

			if (action == "00") { break; }
			using namespace std;
			std::vector<std::string> temp = Util::splitString(action, ' ');
			if (temp[0] == "1") {
				renderCI_Reservation(temp[1], db);
			}
			else if (temp[0] == "2") {
				renderCI_WalkIn(temp[1], db);
			}
			else if (temp[0] == "3") {
				renderCO(temp[1], db);
			}
			else {
				Util::showInvalidAction();
			}

			std::cout << "|\n";

		} while (true);
		Util::showHorizontalLine("single");
		
	}
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
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu\n" << ANSI_COLOR_RESET;
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
				renderCICOMenu();
			}
			if (action == 2) {
				//renderBookingHistorySelection();
			}
			if (action == 3) {
				action = 0;
				isContinue = false; // Go to previous page
				break;
			}
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