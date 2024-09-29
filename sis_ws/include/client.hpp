#ifndef CLIENT_HPP
#define CLIENT_HPP

// cpp lib
#include<string>

// namespace
using std::string;

class Client{

protected: 
    int userID;
    string userName;
    char userType; // user Type: student, staff, registry
    string passcode;

public:
    // constructor destructor
    Client(int inputID, string inputName, char userType, string userPass);
    ~Client();
    void setPasscode(string inputPass);
};
#endif