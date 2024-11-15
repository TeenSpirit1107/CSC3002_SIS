// cpp lib
#include <iostream>
#include<string>
#include<fstream>


// sis classes
#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"

Client::Client() {
    this->userID = "0000000";
    this->userName = "default";
    this->passcode = "123456";
    
}

// [todo] this constructor with many inputs may be erased if it's not used elsewhere.
Client::Client(std::string & inputID, std::string & inputName, std::string & userPass):
    userID(inputID), userName(inputName),passcode(userPass)
{
}

Client::Client(std::string & inputID):
    userID(inputID), userName(""), passcode("")
{}

Client::~Client() {
    // [todo] finish destructor
}

bool Client::id_exist(std::string & inputID) {

    char firstChar = inputID[0];

    // Check whether it's a professor or a student. Find the id in the corresponding path.
    std::string input_path = firstChar == '9' ? Staff::staff_path+inputID+".txt" : Student::student_path+inputID+".txt";
    std::ifstream file(input_path,std::ios::in);

    // Case 1: the id doesn't exist.
    if (!file.is_open()) {
        return false;
    }
    // Case 2: the id does exist.
    return true;
}

void Client::output_basic_info() {
    std::cout<< "passcode: " << passcode<<"; userID: " << userID << "; userName: " << userName << std::endl;
}
