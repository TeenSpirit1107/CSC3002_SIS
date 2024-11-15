#ifndef CLIENT_HPP
#define CLIENT_HPP

// cpp lib
#include <string>

// namespace

class Client{

protected:
    std::string userID;
    std::string passcode;
    std::string userName;

public:
    // constructor destructor
    Client(std::string &  inputID, std::string & inputName, std::string & userPass);
    explicit Client(std::string & inputID);
    Client();
    ~Client(); // [todo] to finish the destructor

    static bool id_exist(std::string & inputID);
    void output_basic_info();
};
#endif