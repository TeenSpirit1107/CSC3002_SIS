#include "client.hpp"

// cpp lib
#include <iostream>
#include <ostream>
#include<string>
#include <utility>
using namespace std;

Client::Client() {
    this->userID = "0000000";
    this->userName = "default";
    this->userType = 'd';
    this->passcode = "123456";
}
Client::Client(int inputID, string inputName, char userType, string userPass){
    this->userID = inputID;
    this->userName = std::move(inputName);
    this->userType = userType;
    this->passcode = std::move(userPass);
}
void Client::setPasscode(string passcode) {
    this->passcode = std::move(passcode);
    cout << "password was changed into: "<< passcode << endl;
}
void Client::output_basic_info() {
    cout << userID << " " << userName << " " << userType << " " << passcode << endl;
}
Client::~Client() {
    cout << userID << " Client destructor" << endl;
}
