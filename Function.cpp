#include "Function.h"
#include <iostream>
#include <ctime>
#include <iomanip>


std::string Function::getCurrentDate() {
    // Get current time
    std::time_t currentTime;
    std::time(&currentTime);

    // Convert to local time
    std::tm localTime;
    localtime_s(&localTime, &currentTime);

    // Format the date as dd/mm/yyyy
    std::ostd::stringstream oss;
    oss << std::setw(2) << std::setfill('0') << localTime.tm_mday << "/"
        << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1) << "/"
        << (localTime.tm_year + 1900);

    return oss.str();
}



void Function::renderHeading(const std::string& name, const std::string& id, const std::string& userType) {
    std::cout << "|	LOGGED AS: " << name << "   ID: " << id << " " << " [" << userType << "]\n";
    std::cout << "| =============================================================================================================\n";

}
