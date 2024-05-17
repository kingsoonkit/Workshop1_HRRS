#ifndef LOGIN
#define LOGIN

#include "Guest.h"
#include "Receptionist.h"
#include "Manager.h"
#include "Util.h"

class Login {

public:
	static void renderStartMenu();
	static void renderLoginPrompt();
	static void prepareMainMenuAccess(const std::string& username);
};

#endif



//////////////////////////////////////////////////////


//void Account::DeleteAcc() {
//	try {
//		DBConnection db;
//		db.prepareStatement("DELETE FROM account WHERE accountID=?");
//		db.stmt->setInt(1, accountID);
//		db.QueryStatement();
//		db.~DBConnection();
//	} catch (sql::SQLException& e) {
//		// Handle the exception - print or log the error details
//		std::cerr << "SQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
//		Login("pause");
//	}
//}

/////////////////////////////////////////////////////////

//void Account::UpdateNewUsername() {
//	try {
//		DBConnection db;
//		db.prepareStatement("UPDATE account SET username=?,acc_updated_at=? WHERE accountID=?");
//		db.stmt->setString(1, getUsername());
//		db.stmt->setDateTime(2, getAccUpdatedAt());
//		db.stmt->setInt(3, getAccountID());
//		db.QueryStatement();
//		db.~DBConnection();
//	} catch (sql::SQLException& e) {
//		// Handle the exception - print or log the error details
//		std::cerr << "SQL Exception: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
//		Login("pause");
//	}
//}

