#include "Guest.h"

std::string Guest::ICNumber;
std::string Guest::guestUsername;
std::string Guest::name;
std::string Guest::phoneNo;
std::string Guest::password;
std::string Guest::userType;



// Setters 
void Guest::setICNumber(const std::string& ICNumber) {
	Guest::ICNumber = ICNumber;
}
void Guest::setGuestUsername(const std::string& guestUsername) {
	Guest::guestUsername = guestUsername;
}
void Guest::setName(const std::string& name) {
	Guest::name = name;
}
void Guest::setPhoneNo(const std::string& phoneNo) {
	Guest::phoneNo = phoneNo;
}
void Guest::setPassword(const std::string& password) {
	Guest::password = password;
}
void Guest::setUserType(const std::string& userType) {
	Guest::userType = userType;
}

// Getters
std::string Guest::getICNumber() {
	return ICNumber;
}
std::string Guest::getGuestUsername() {
	return guestUsername;
}
std::string Guest::getName() {
	return name;
}
std::string Guest::getPhoneNo() {
	return phoneNo;
}
std::string Guest::getPassword() {
	return password;
}
std::string Guest::getUserType() {
	return userType;
}



// Functionalities
bool Guest::isICNumberExist(const std::string& ICNumber) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT ICNumber FROM Guest WHERE ICNumber = ?");
		db.stmt->setString(1, ICNumber);
		db.QueryResult();
		if (db.res->rowsCount() == 1) { return true; }

	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}

bool Guest::isUser(const std::string& ICNumber) {
	try {
		DBConnection db;
		db.prepareStatement("SELECT UserType FROM Guest WHERE ICNumber = ?");
		db.stmt->setString(1, ICNumber);
		db.QueryResult();
		if (db.res->rowsCount() == 1) {
			db.res->next();
			if (db.res->getString("UserType") != "NoAccount") {
				return true;
			}
		}
	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	return false;
}

bool Guest::isPasswordCorrect(const std::string& username, const std::string& password) {
	try {
		const std::string hashedText = Util::hashText(password);

		DBConnection db;
		db.prepareStatement("SELECT Password"
							" FROM Guest"
							" WHERE GuestUsername = ? AND Password = ?");
		db.stmt->setString(1, username);
		db.stmt->setString(2, hashedText);
		db.QueryResult();


		if (db.res->rowsCount() <= 0) {
			return false;
		}
		else if (db.res->rowsCount() == 1) {
			while (db.res->next())
				if (hashedText != db.res->getString("Password"))
					return false;
		}

	} catch (sql::SQLException& e) {
		std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}

	return true;
}



// Rendering
void Guest::renderRegisterPrompt() {
	bool isExiting = false;
	bool isNewGuest = true;
	do {
		std::string tempArr[5];

		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Register as Guest\n" << ANSI_COLOR_RESET;
		Util::showEscInstruction();
		std::cout << "|\t------------------------------------------\n";
		std::cout << "|\n";

		// Parsing IC Number input
		do {
			tempArr[0] = Util::parseICNumberInput();
			if (tempArr[0] == __EXIT_CODE__) { isExiting = true; break; }

			if (Guest::isICNumberExist(tempArr[0]) && Guest::isUser(tempArr[0])) {
				std::cout << "|\t" << ANSI_COLOR_RED << "This IC number is already in the system. You may return to login\n" << ANSI_COLOR_RESET;
				std::cout << "|\n";
			}
			else if (Guest::isICNumberExist(tempArr[0])) {
				std::cout << "|\t" << ANSI_COLOR_YELLOW << "It looks like you have previous bookings in our system.\n" << ANSI_COLOR_RESET;
				std::cout << "|\t" << ANSI_COLOR_YELLOW << "Now let's register you as a guest user!\n" << ANSI_COLOR_RESET;
				std::cout << "|\t" << ANSI_COLOR_GREEN << "[OK]\n" << ANSI_COLOR_RESET;
				std::cout << "|\n";
				isNewGuest = false;
				break;
			}
			else {
				std::cout << "|\t" << ANSI_COLOR_GREEN << "[OK]\n" << ANSI_COLOR_RESET;
				std::cout << "|\t-----------------------------------\n";
				break;
			}
		} while (true);
		if (isExiting) { break; }
		
		// Parsing IC Number input
		isExiting = false;
		std::cout << "|\tUsername will be used for login.\n";
		do {
			tempArr[1] = Util::parseUsernameInput();
			if (tempArr[1] == __EXIT_CODE__) { isExiting = true; break; }
			if (DBUtil::isUsernameExist(tempArr[1])) {
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
			break;
		}

		tempArr[2] = (isNewGuest) ? Util::parseNameInput() : "";
		if (tempArr[2] == __EXIT_CODE__) { break; }

		tempArr[3] = (isNewGuest) ? Util::parsePhoneNumberInput() : "";
		if (tempArr[3] == __EXIT_CODE__) { break; }

		tempArr[4] = Util::parsePasswordInput(false, true);
		if (tempArr[4] == __EXIT_CODE__) { break; }

		try {
			DBConnection db;
			if (isNewGuest) {
				db.prepareStatement("INSERT INTO guest (ICNumber, GuestUsername, Name, PhoneNo, Password, UserType) VALUES (?,?,?,?,?,?)");
				db.stmt->setString(1, tempArr[0]);
				db.stmt->setString(2, tempArr[1]);
				db.stmt->setString(3, tempArr[2]);
				db.stmt->setString(4, tempArr[3]);
				db.stmt->setString(5, Util::hashText(tempArr[4]));
				db.stmt->setString(6, GUEST_USERTYPE);
				db.QueryStatement();
			}
			else {
				db.prepareStatement("UPDATE guest SET GuestUsername = ? , Password = ? , UserType = ? WHERE ICNumber = ?");
				db.stmt->setString(1, tempArr[1]);
				db.stmt->setString(2, Util::hashText(tempArr[4]));
				db.stmt->setString(3, GUEST_USERTYPE);
				db.stmt->setString(4, tempArr[0]);
				db.QueryStatement();
			}
		} catch (sql::SQLException& e) {
			std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
		}
		Util::showPositiveMessage("Registration Complete!", true);
		Util::showHorizontalLine("double");
		Util::showRefreshCountdown();
		break;
	} while (true);
}


void Guest::renderAddReservationMenu() {
	DBConnection db;
	std::string bookingID;
	bool isExiting = false;

	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Add Reservation" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 8);
		std::cout << "|\t----------------------------\n";
		std::cout << "|\n";
		Util::showEscInstruction();

		// INPUT
		std::string startDate = Util::parseDateInput("Start Date", true);
		if (startDate == __EXIT_CODE__) { break; }
		std::string endDate = Util::parseDateInput("End Date", false, startDate);
		if (endDate == __EXIT_CODE__) { break; }

		tabulate::Table table = DBUtil::getTable_AvailableRoomsFrom(startDate, endDate);

		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Available Rooms\n" << ANSI_COLOR_RESET;
		std::cout << table << "\n";
		std::cout << "|\t" << ANSI_COLOR_GOLD << table.size()  - 1  << " rooms" << ANSI_COLOR_RESET << " are available from " << ANSI_COLOR_GOLD << startDate << ANSI_COLOR_RESET << " until " << ANSI_COLOR_GOLD << endDate << "\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\n";
		std::cout << "|\tEnter one or more room numbers (use dashes \"-\")\n";
		std::cout << "|\tExample: " << ANSI_COLOR_GOLD << "R005" << ANSI_COLOR_RESET << " or " << ANSI_COLOR_GOLD << "R001-R015-R006\n" << ANSI_COLOR_RESET;
		Util::showEscInstruction();
		std::cout << "|\n";

		do {
			// INPUT
			std::string roomsRawInput = Util::parseTextInput("Rooms");
			if (roomsRawInput == __EXIT_CODE__) { break; }

			// String splitting
			std::vector<std::string> temp = Util::split(roomsRawInput, '-');
			std::vector<std::string> roomNumbers(temp.begin(), temp.end());

			if (roomNumbers.size() <= 0) { // If there are no room input
				Util::showNegativeMessage("You must enter at least 1 room number");
			}
			else if (!DBUtil::isRoomNumberHere(roomNumbers, table, 0)) { // If does NOT exist
				Util::showNegativeMessage("One or more of the selected rooms is not available");
			}
			else {
				try {
					// 1. Add booking details
					db.prepareStatement("INSERT INTO Booking (ICNumber, BookingType) VALUES (?,?)");
					db.stmt->setString(1, getICNumber());
					db.stmt->setString(2, __BOOKING_TYPE_RESERVATION__);
					db.QueryStatement();

					// 2.1 Get the new ID from just now (based on ICNumber)
					db.prepareStatement("SELECT bookingID FROM Booking WHERE ICNumber = ?");
					db.stmt->setString(1, getICNumber());
					db.QueryResult();
					while (db.res->next()) {
						bookingID = db.res->getString("bookingID");
					}

					// 2.2 Now insert the bookingline, must have bookingID and at least ONE RoomNumber
					for (std::string roomNumber : roomNumbers) {
						db.prepareStatement("INSERT INTO bookingline (RoomNumber, bookingID, StartDate, EndDate, BookingStatus)"
											" VALUES (?,?,?,?,?);");
						db.stmt->setString(1, roomNumber);
						db.stmt->setString(2, bookingID);
						db.stmt->setString(3, startDate);
						db.stmt->setString(4, endDate);
						db.stmt->setString(5, __BOOKING_STATUS_RESERVED__);
						db.QueryStatement();
					}
				} 
				catch (sql::SQLException& e) {
					std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
				}
				DBUtil::updateBookingStatuses();
				DBUtil::updateRoomStatuses();
				DBUtil::updateBookingNetPrice(bookingID);
				Util::showPositiveMessage("Reservation is Booked Successfully", true);
				Util::showBookingIDDisplay(bookingID);
				Util::showPressAnyKeyToContinue();
				Util::showRefreshCountdown();
				isExiting = true;
				break;
			}
			std::cout << "|\n";
		} while (true);
		if (isExiting) { break; }
	} while (true);
}


/////////////////////


void Guest::renderCancelReservationMenu() {
	DBConnection db;
	bool isExiting = false;
	int action = 0;
	do {
		tabulate::Table table = DBUtil::getTable_ActiveReservationsByICNumber(getICNumber());
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > " << ANSI_COLOR_ORANGE << "Cancel Room Reservation" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 7);
		std::cout << "|\t-----------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Your reservations:\n" << ANSI_COLOR_RESET;
		if (table.size() > 0) {
			std::cout << table << "\n";
			std::cout << "|\t" << ANSI_COLOR_GOLD << DBUtil::numOfBookingIDs << ANSI_COLOR_RESET << " booking(s), " << ANSI_COLOR_GOLD << table.size() - 1 << ANSI_COLOR_RESET << " active room reservation(s)\n";
			std::cout << "|\n";
			std::cout << "|\n";
			Util::showEscInstruction();
			std::cout << "|\tEnter a booking ID associated with the room\n";
			do {
				std::string bookingID = Util::parseTextInput("Booking ID");
				if (bookingID == __EXIT_CODE__) { isExiting = true; break; }

				if (!DBUtil::isBookingIDHere(bookingID, table, 0)) {
					Util::showNegativeMessage("You don't have this booking ID");
					std::cout << "|\n";
				}
				else {
					std::cout << "|\n";
					std::cout << "|\n";
					std::cout << "|\tFor " << ANSI_COLOR_YELLOW << "Booking ID " << ANSI_COLOR_GOLD << bookingID << ANSI_COLOR_RESET << ", enter one or more room numbers that you wish to cancel\n";
					std::cout << "|\tExample: " << ANSI_COLOR_GOLD << "R005" << ANSI_COLOR_RESET << " or " << ANSI_COLOR_GOLD << "R001-R015-R006\n" << ANSI_COLOR_RESET;
					do {
						std::string roomRawInput = Util::parseTextInput("Room number(s)");
						if (roomRawInput == __EXIT_CODE__) { isExiting = true; break; }

						// String splitting
						std::vector<std::string> temp = Util::split(roomRawInput, '-');
						std::vector<std::string> roomNumbers(temp.begin(), temp.end());

						if (roomNumbers.size() <= 0) { // If there are no room input
							Util::showNegativeMessage("You must enter at least 1 room number");
							std::cout << "|\n";
						}
						else if (!DBUtil::isRoomNumberHere(roomNumbers, table, 4)) { // If is NOT available
							Util::showNegativeMessage("One or more selected room numbers are not in your reservations");
							std::cout << "|\n";
						}
						else {
							viewCancelConfirmation(roomNumbers, bookingID);
							break;
						}
					} while (true);
					break;
				}
			} while (true);
		}
		else {
			std::cout << "|\n";
			Util::showPassiveMessage("You don't have any reservations currently.");
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


void Guest::viewCancelConfirmation(const std::vector<std::string>& roomNumbers, const std::string& bookingID) {
	int action = 0;
	bool isExiting = false;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu > Cancel Room Reservation" << ANSI_COLOR_RESET << Util::writeTodayDate(true, 7);
		std::cout << "|\t-----------------------------------\n";
		std::cout << "|\n";
		std::cout << "|\tCancelling reservation for " << Util::writeRoomList(true, roomNumbers);
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_ORANGE << "Confirm cancellation?\n" << ANSI_COLOR_RESET;
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
			if (action == 0) { isExiting = true; break; }
			if (action == 1) {
				try {
					DBConnection db;
					for (std::string roomNumber : roomNumbers) {
						db.prepareStatement("UPDATE bookingline"
											" SET BookingStatus = 'Cancelled', CancelledDate = CURRENT_DATE"
											" WHERE BookingStatus = 'Reserved' AND BookingID = ? AND RoomNumber = ?");
						db.stmt->setString(1, bookingID);
						db.stmt->setString(2, roomNumber);
						db.QueryStatement();
					}
				} catch (sql::SQLException& e) {
					std::cerr << "|\tSQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
				}
				DBUtil::updateBookingStatuses();
				DBUtil::updateRoomStatuses();
				DBUtil::updateBookingNetPrice(bookingID);
				Util::showPositiveMessage("Reservation has successfully cancelled", true);
				Util::showHorizontalLine("double");
				Util::showRefreshCountdown();
				isExiting = true;
			}
			break;
		}
		if (isExiting) { break; }
	} while (true);
}


/////////////////////


void Guest::renderBookingHistory() {
	bool isExiting = false;
	do {
		tabulate::Table table = DBUtil::getTable_BookingHistoryByICNumber(getICNumber());
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

				if (!DBUtil::isBookingIDExist(bookingID, getICNumber())) {
					Util::showNegativeMessage("Booking ID does not exist");
					std::cout << "|\n";
				}
				else {
					Guest::viewBookingDetailsOf(bookingID);
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


void Guest::viewBookingDetailsOf(const std::string& bookingID) {
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


/////////////////////



void Guest::renderMainMenu() {

	//setGuestUsername("king222"); /////////////
	//setICNumber("020507030961"); /////////////
	//setPassword("ks|sYHB+7");  /////////////


	bool isContinue = true;
	int action = 0;
	do {
		system("cls");
		Util::showHorizontalLine("double");
		Util::showLogHeading(getName(), getUserType(), getGuestUsername());
		Util::showHorizontalLine("single");
		std::cout << "|\n";
		std::cout << "|\t" << ANSI_COLOR_YELLOW << "Main Menu\n" << ANSI_COLOR_RESET;
		std::cout << "|\t---------\n";
		std::cout << "|\n";
		std::cout << "|\tSelect an option:\n";
		std::cout << "|\t" << (action == 0 ? ANSI_COLOR_GOLD : "") << "> Add Reservation <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 1 ? ANSI_COLOR_GOLD : "") << "> Cancel Room Reservation <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t" << (action == 2 ? ANSI_COLOR_GOLD : "") << "> Booking History <\n" << ANSI_COLOR_RESET;
		std::cout << "|\t-------------------------------\n";
		std::cout << "|\t" << (action == 3 ? ANSI_COLOR_ORANGE : "") << "> LOG OUT <\n" << ANSI_COLOR_RESET;
		std::cout << "|\n";
		std::cout << "|\tUse arrow-up key or arrow-down key to select, and then press enter\n";
		std::cout << "|\tSELECTING: " << ANSI_COLOR_GOLD << (action == 0 ? "[ Add Reservation ]\n" : action == 1 ? "[ Cancel Room Reservation ]\n" : action == 2 ? "[ Booking History ]\n" : ANSI_COLOR_ORANGE "[ LOG OUT ]\n") << ANSI_COLOR_RESET;
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
				renderAddReservationMenu();
			}
			if (action == 1) {
				renderCancelReservationMenu();
			}
			if (action == 2) {
				renderBookingHistory();
			}
			if (action == 3) {
				isContinue = false;
				Util::showRefreshCountdown();
				break;
			}
		}
	} while (isContinue);
}


