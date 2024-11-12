#ifndef CLIENT_HPP
#define CLIENT_HPP

// cpp lib
#include <string>

// namespace
using std::string;

class Client{

protected:
    string userID;
    string userName;
    char userType; // user Type: a: registry; b: professors; c: students; d: default (TBD)
    string passcode;

public:
    // constructor destructor
    Client(int inputID, string inputName, char userType, string userPass);

    Client();
    void setPasscode(string inputPass);
    void output_basic_info();
    ~Client();
};
#endif