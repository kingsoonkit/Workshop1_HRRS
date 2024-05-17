//void miscfunctions::viewTable(string whatTable, int userID, string accType, string searchBy, string keyword, string sortBy, bool orderBy) {
//
//    Account user;
//
//    string tempString, status;
//    const int rowsPerPage = 7, max_length = 20; // Maximum length of the description to be displayed;
//    int currentPage = 1, x = 1;
//
//    sql::Driver* driver;
//    sql::Connection* conn;
//    sql::Statement* stmt;
//    sql::ResultSet* res = nullptr;
//
//    try {
//        // Create a connection
//        driver = get_driver_instance();
//        conn = driver->connect("tcp://127.0.0.1:3306", "root", "");
//        conn->setSchema("vms");
//
//        // Execute query
//        stmt = conn->createStatement();
//        if (whatTable == "tableStaff") {
//            res = stmt->executeQuery("SELECT * FROM staff WHERE staff_status = 1");
//        }
//        else if (whatTable == "tableResident") {
//            res = stmt->executeQuery("SELECT * FROM resident WHERE resident_status = 1");
//        }
//        else if (whatTable == "tableComplaint") {
//
//            if (accType == "Resident") {
//                DBConnection db;
//
//                db.prepareStatement("SELECT DISTINCT accountType FROM account;");
//                db.QueryResult();
//
//                while (db.res->next()) {
//                    string query = "SELECT * FROM complaint WHERE residentID = ? AND (";
//
//                    if (searchBy == "comp_created_at"  searchBy == "comp_updated_at") {
//                        query += "DATE(" + searchBy + ") LIKE ? ) ";
//                    }
//                    else {
//                        query += searchBy + " LIKE ? ) ";
//                    }
//
//                    query += "ORDER BY " + sortBy;
//
//                    if (orderBy == true) {
//                        query += " ASC";
//                    }
//                    else {
//                        query += " DESC";
//                    }
//
//                    // Prepare the SQL query with a parameter for residentID
//                    sql::PreparedStatement* prepStmt = conn->prepareStatement(query);
//
//                    // Set the value of the residentID parameter
//                    prepStmt->setInt(1, userID);
//                    if (searchBy == "comp_created_at"  searchBy == "comp_updated_at") {
//                        prepStmt->setString(2, "%" + keyword + "%");
//                    }
//                    else {
//                        prepStmt->setString(2, keyword + "%");
//                    }
//
//                    // Execute the prepared statement and assign the result to 'res'
//                    res = prepStmt->executeQuery();
//                }
//            }
//            else {
//                if (searchBy == "resident_name") {
//                    searchBy = "r." + searchBy;
//                }
//                else {
//                    searchBy = "c." + searchBy;
//                }
//
//                string query = "SELECT c.*, r.resident_name "
//                    "FROM complaint c "
//                    "JOIN resident r ON c.residentID = r.residentID "
//                    "WHERE(";
//
//                if (searchBy == "c.comp_created_at"  searchBy == "c.comp_updated_at") {
//                    query += "DATE(" + searchBy + ") LIKE ? ) ";
//                }
//                else {
//                    query += searchBy + " LIKE ? ) ";
//                }
//                query += "ORDER BY c." + sortBy;
//
//                if (orderBy == true) {
//                    query += " ASC";
//                }
//                else {
//                    query += " DESC";
//                }
//
//                // Prepare the SQL query with a parameter for residentID
//                sql::PreparedStatement* prepStmt = conn->prepareStatement(query);
//
//                if (searchBy == "c.comp_created_at"  searchBy == "c.comp_updated_at") {
//                    prepStmt->setString(1, "%" + keyword + "%");
//                }
//                else {
//                    prepStmt->setString(1, keyword + "%");
//                }
//
//                // Execute the prepared statement and assign the result to 'res'
//                res = prepStmt->executeQuery();
//            }
//        }
//        Table tableStaff, tableResident, tableComplaint;
//        int size = 0;
//
//        if (whatTable == "tableStaff") {
//            string databaseData[] = { "No", "Staff ID", "IC Number", "Name", "Email", "Phone Number", "Address", "Status", "Joined Date", "Last Update" };
//            size = sizeof(databaseData) / sizeof(databaseData[0]);
//
//            tableStaff.add_row({ databaseData[0], databaseData[1], databaseData[2], databaseData[3], databaseData[4], databaseData[5], databaseData[6], databaseData[7], databaseData[8], databaseData[9] });
//        }
//        else if (whatTable == "tableResident") {
//            string databaseData[] = { "No", "Resident ID", "IC Number", "Name", "Email", "Phone Number", "Address", "Status", "Joined Date", "Last Update" };
//            size = sizeof(databaseData) / sizeof(databaseData[0]);
//
//            tableResident.add_row({ databaseData[0], databaseData[1], databaseData[2], databaseData[3], databaseData[4], databaseData[5], databaseData[6], databaseData[7], databaseData[8], databaseData[9] });
//        }
//        else if (whatTable == "tableComplaint") {
//            if (accType == "Resident") {
//                string databaseData[] = { "No", "Complaint ID", "Complaint Category", "Title", "Description", "Status", "Feedback", "Date Issued", "Last Update" };
//                size = sizeof(databaseData) / sizeof(databaseData[0]);
//
//                tableComplaint.add_row({ databaseData[0], databaseData[1], databaseData[2], databaseData[3], databaseData[4], databaseData[5], databaseData[6], databaseData[7], databaseData[8] });
//            }
//            else {
//                string databaseData[] = { "No", "Complaint ID", "Resident Name", "Complaint Category", "Title", "Description", "Status", "Feedback", "Date Issued", "Last Update" };
//                size = sizeof(databaseData) / sizeof(databaseData[0]);
//
//                tableComplaint.add_row({ databaseData[0], databaseData[1], databaseData[2], databaseData[3], databaseData[4], databaseData[5], databaseData[6], databaseData[7], databaseData[8], databaseData[9] });
//            }
//        }
//
//        // Fetch and display results
//        while (res->next()) {
//            stringstream created_ss, updated_ss;
//            tm created_tm = {}; // Declare created_tm outside of the if-else blocks
//            tm updated_tm = {}; // Declare updated_tm outside of the if-else blocks
//
//            created_ss.str(""); // Clear existing content
//            updated_ss.str(""); // Clear existing content
//            if (whatTable == "tableStaff") {
//                created_ss.str(res->getString("staff_created_at")); // Set new content
//                updated_ss.str(res->getString("staff_updated_at")); // Set new content
//            }
//            else if (whatTable == "tableResident") {
//                created_ss.str(res->getString("resident_created_at")); // Set new content
//                updated_ss.str(res->getString("resident_updated_at")); // Set new content
//            }
//            else if (whatTable == "tableComplaint") {
//                created_ss.str(res->getString("comp_created_at")); // Set new content
//                updated_ss.str(res->getString("comp_updated_at")); // Set new content
//            }
//            created_ss >> get_time(&created_tm, "%Y-%m-%d %H:%M:%S");
//            updated_ss >> get_time(&updated_tm, "%Y-%m-%d %H:%M:%S");
//
//            // Format created_at and updated_at as strings
//            stringstream created_formatted;
//            created_formatted << put_time(&created_tm, "%d-%m-%Y");
//            stringstream updated_formatted;
//            updated_formatted << put_time(&updated_tm, "%d-%m-%Y");
//
//            if (whatTable == "tableStaff") {
//                tableStaff.add_row({ to_string(x++) + ".", to_string(res->getInt("staffID")), res->getString("staff_ICNum"), toUppercase(res->getString("staff_name")), res->getString("staff_email"), res->getString("staff_phoneNum"), (res->getString("staff_address") != "") ? res->getString("staff_address") : "-", "ACTIVE", created_formatted.str(), updated_formatted.str() });
//            }
//            else if (whatTable == "tableResident") {
//                //cout << "anjay" << endl; system("pause");
//                tableResident.add_row({ to_string(x++) + ".", to_string(res->getInt("residentID")), res->getString("resident_ICNum"), toUppercase(res->getString("resident_name")), res->getString("resident_email"), res->getString("resident_phoneNum"), (res->getString("resident_address") != "") ? res->getString("resident_address") : "-", "ACTIVE", created_formatted.str(), updated_formatted.str() });
//            }
//            else if (whatTable == "tableComplaint") {
//                //cout << "anjay" << endl; system("pause");
//                tempString = res->getString("description");
//
//                // Check if the description exceeds the maximum length
//                if (tempString.length() > max_length) {
//                    // Truncate the description and append "..." to indicate truncation
//                    tempString = tempString.substr(0, max_length - 3) + "...";
//                }
//
//                if (res->getString("status") == "1") {
//                    status = "IN PROGRESS";
//                }
//                else {
//                    status = "COMPLETED";
//                }
//                //cout << "accType: " << accType << endl; system("pause");
//                if (accType == "Resident") {
//                    tableComplaint.add_row({ to_string(x++) + ".", to_string(res->getInt("complaintID")), res->getString("complaintCategory"), toUppercase(res->getString("title")), tempString, status, (res->getString("feedback") != "") ? res->getString("feedback") : "-", created_formatted.str(), updated_formatted.str() });
//                }
//                else {
//                    tableComplaint.add_row({ to_string(x++) + ".", to_string(res->getInt("complaintID")), res->getString("resident_name"), res->getString("complaintCategory"), toUppercase(res->getString("title")), tempString, status, (res->getString("feedback") != "") ? res->getString("feedback") : "-", created_formatted.str(), updated_formatted.str() });
//                }
//            }
//
//        }
//
//        if (whatTable == "tableStaff") {
//            // Apply styling to the header row
//            for (size_t i = 0; i < size; ++i) {
//                tableStaff[0][i].format()
//                    .font_color(Color::yellow)
//                    .font_align(FontAlign::center)
//                    .font_style({ FontStyle::bold });
//            }
//
//            // Apply styling to the data rows starting from index [1][0]
//            for (size_t i = 1; i < tableStaff.size(); ++i) {
//                for (size_t j = 0; j < size; ++j) {
//                    tableStaff[i][j].format()
//                        .font_color(Color::white) // Change color for data rows if needed
//                        .font_align(FontAlign::center); // Change alignment for data rows if needed
//                }
//            }
//
//            cout << tableStaff << endl;
//        }
//        else if (whatTable == "tableResident") {
//            // Apply styling to the header row
//            for (size_t i = 0; i < size; ++i) {
//                tableResident[0][i].format()
//                    .font_color(Color::yellow)
//                    .font_align(FontAlign::center)
//                    .font_style({ FontStyle::bold });
//            }
//
//            // Apply styling to the data rows starting from index [1][0]
//            for (size_t i = 1; i < tableResident.size(); ++i) {
//                for (size_t j = 0; j < size; ++j) {
//                    tableResident[i][j].format()
//                        .font_color(Color::white) // Change color for data rows if needed
//                        .font_align(FontAlign::center); // Change alignment for data rows if needed
//                }
//            }
//
//            cout << tableResident << endl;
//        }
//        else if (whatTable == "tableComplaint") {
//            // Apply styling to the header row
//            for (size_t i = 0; i < size; ++i) {
//                tableComplaint[0][i].format()
//                    .font_color(Color::yellow)
//                    .font_align(FontAlign::center)
//                    .font_style({ FontStyle::bold });
//            }
//
//            // Apply styling to the data rows starting from index [1][0]
//            for (size_t i = 1; i < tableComplaint.size(); ++i) {
//                for (size_t j = 0; j < size; ++j) {
//                    tableComplaint[i][j].format()
//                        .font_color(Color::white) // Change color for data rows if needed
//                        .font_align(FontAlign::center); // Change alignment for data rows if needed
//                }
//            }
//
//
//            if (accType == "Resident") {
//                for (size_t i = 1; i < tableComplaint.size(); ++i) { //row
//                    for (size_t j = 5; j < 6; ++j) { // column
//                        if (status == "IN PROGRESS") {
//                            tableComplaint[i][j].format()
//                                .font_color(Color::cyan) // Change color for data rows if needed
//                                .font_align(FontAlign::center); // Change alignment for data rows if needed
//                        }
//                        else {
//                            tableComplaint[i][j].format()
//                                .font_color(Color::green) // Change color for data rows if needed
//                                .font_align(FontAlign::center); // Change alignment for data rows if needed
//                        }
//
//                    }
//                }
//            }
//            else {
//                for (size_t i = 1; i < tableComplaint.size(); ++i) { //row
//                    for (size_t j = 6; j < 7; ++j) { // column
//                        if (status == "IN PROGRESS") {
//                            tableComplaint[i][j].format()
//                                .font_color(Color::cyan) // Change color for data rows if needed
//                                .font_align(FontAlign::center); // Change alignment for data rows if needed
//                        }
//                        else {
//                            tableComplaint[i][j].format()
//                                .font_color(Color::green) // Change color for data rows if needed
//                                .font_align(FontAlign::center); // Change alignment for data rows if needed
//                        }
//
//                    }
//                }
//            }
//
//            cout << tableComplaint << endl;
//        }
//
//        // Free resources
//        delete res;
//        delete stmt;
//        delete conn;
//    } catch (sql::SQLException& e) {
//        displayError(e, nullptr);
//    }
//}