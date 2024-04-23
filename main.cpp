#include <iostream>
#include "LoginRegistration.h"
#include "Function.h"


using namespace std;

void renderAppHeading() {
	cout << "| =============================================================================================================\n";
	cout << "|\n";
	cout << "|	Hotel Room Reservation System\n";
	cout << "|\n";
	cout << "| =============================================================================================================\n";
}


void renderStartMenu() {
    string action;

    cout << "|\n";
    cout << "|  Welcome!\n";
    cout << "|\n";
    cout << "|  Enter number for desired action (Exp: -#: 1)\n";
    cout << "|  1. LOGIN\n";
    cout << "|  -------------------------------\n";
    cout << "|  2. REGISTER AS Receptionist\n";
    cout << "|  3. REGISTER AS Guest\n";
    cout << "|  -------------------------------\n";
    cout << "|  00. EXIT\n";
    cout << "|\n";
    cout << "|  Action-#: "; cin >> action;
    cout << "|\n";
    cout << "| ------------------------------------------------------------------------------------------------------------\n";


    if (action == "1") {
        Login::renderInputPrompt();
    }
    else if (action == "2") {
        Registration::renderInputPrompt("As Receptionist");
    }
    else if (action == "3") {
        Registration::renderInputPrompt("As Guest");
    }
    else if (action == "00") {
        exit(0);
    }
    else {
        cout << "|  ? Invalid action ?\n";
    }

}


int main() {

	renderAppHeading();
    renderStartMenu();

    Person p;
    p.name;
}