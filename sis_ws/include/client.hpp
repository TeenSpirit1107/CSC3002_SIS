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
    std::string profile_path;

    // Paths in data_repo


public:

    // constructor destructor
    Client(std::string &  inputID, std::string & inputName, std::string & userPass);
    explicit Client(std::string & inputID);
    Client();
    ~Client(); // [todo] to finish the destructor

    // constant member variables
    const std::string course_claim_path_prefix;

    // setters and getters
    std::string get_userID() const;
    std::string get_passcode() const;
    std::string get_userName() const;

    // Feature: Processing ID
    bool is_student() const;
    static bool id_is_student(std::string & inputID);

    // static std::string get_path(std::string & inputID); //TODO: 是否有必要写这个？


    // Feature: log in
    // static auto log_in(std::string & inputID, std::string & inputPass);
    // prof and student has get_profile() function
    static bool id_exist(std::string & inputID);
    static bool validate_passcode(std::string & inputID, std::string & inputPass);

    // Testing
    void output_basic_info() const;

    // Tools: Parsing and generating format
    static bool is_valid_course_code(std::string & inputCourseCode);
    static bool is_valid_course_expr(std::string & inputExpr);

    static std::string get_current_datetime();
};
#endif