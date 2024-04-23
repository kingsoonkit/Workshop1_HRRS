#pragma once
#include <string>

class Function {
public:
	static std::string getCurrentDate();
	static void renderHeading(const std::string& name, const std::string& id, const std::string& userType);

};

