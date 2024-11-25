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
    // TODO: Repalce this by "create Client" function
    Client(const std::string &  inputID, const std::string & inputName, const std::string & userPass);
    explicit Client(const std::string & inputID);
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
    static bool id_is_student(const std::string & inputID);

    // Feature: log in
    // Student, Staff has find_profile function which returns their pointers.
    static bool id_exist(const std::string & inputID);
    static bool validate_passcode(const std::string & inputID, const std::string & inputPass);

    // Testing
    // TODO: may delete this after debuggin
    void output_basic_info() const;

    // Tools: Parsing and generating format
    static bool is_valid_course_code(const std::string & inputCourseCode);
    static bool is_valid_course_expr(const std::string & inputExpr);

static bool is_legal_name(const std::string &inputName);
static std::string format_name(const std::string & inputName);

static std::string get_current_datetime();
// static std::string id_get_name(const std::string & inputID);
static std::string name_get_id(const std::string & inputName, bool isStudent);
};
#endif