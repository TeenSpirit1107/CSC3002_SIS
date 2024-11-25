// cpp lib
#include <iostream>
#include<string>
#include<fstream>
#include<memory>
#include<stack>
#include <ctime>
#include <sstream>

// sis classes
#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"
#include "course.hpp"


Client::Client():
    course_claim_path_prefix( ".\\sis_ws\\data_repo\\course_claim\\")
{
    this->userID = "0000000";
    this->userName = "default";
    this->passcode = "123456";
}

// [todo] this constructor with many inputs may be erased if it's not used elsewhere.
Client::Client(std::string & inputID, std::string & inputName, std::string & userPass):
    userID(inputID), userName(inputName),passcode(userPass), course_claim_path_prefix( ".\\sis_ws\\data_repo\\course_claim\\")
{
}

Client::Client(std::string & inputID):
    userID(inputID), userName(""), passcode(""),profile_path(""),  course_claim_path_prefix( ".\\sis_ws\\data_repo\\course_claim\\")
{}

Client::~Client() {
    // [todo] finish destructor
}

// setters and getters

std::string Client::get_userID() const {
    return userID;
}

std::string Client::get_passcode() const {
    return passcode;
}

std::string Client::get_userName() const {
    return userName;
}


// Feature: Processing ID

/**
 * @brief Check whether the client is a student.
 * Caveat: When calling this function, you may assume the return value "false" means the client is a staff.
 * However, when client is a registry, it also returns "false".
 * @return true if the client is a student, false if  otherwise.
 */
bool Client::is_student() const {
    return userID[0] == '1';
}


/**
 * @brief Check whether the id is a student's.
 * Caveat: When calling this function, you may assume the return value "false" means the client is a staff.
 * However, when client is a registry, it also returns "false".
 * @return true if the client is a student, false if  otherwise.
 */

bool Client::id_is_student(std::string & inputID) {
    return inputID[0] == '1';
}

// Feature: Log In

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


/**
 * @brief Validate the passcode.
 * @param inputID The ID of the client.
 * @param inputPass The passcode to be validated.
 * @return true if the passcode is correct, false if the ID doesn't exist, or passcode is wrong.
 */
bool Client::validate_passcode(std::string &inputID, std::string &inputPass) {

    if (!id_exist(inputID)) {
        printf("Failure: ID doesn't exist.\n");
        return false;
    }
    bool valid_pass = false;
    if (id_is_student(inputID)) {
        std::string work_dir = Student::student_path+inputID+".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file,real_pass);
        valid_pass = real_pass == inputPass;
    }else {
        std::string work_dir = Staff::staff_path+inputID+".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file,real_pass);
        valid_pass= real_pass == inputPass;
    }
    return valid_pass;
}



/*

auto Client::log_in(std::string & inputID, std::string & inputPass) {

    // Step 1: check client type.
    bool is_stu = id_is_student(inputID);

    // Step 2: check whether the id exists.
    if(!Client::id_exist(inputID)) {
        return nullptr;
    }

    // Step 3: create a new object in HEAP and return its pointer.
    // Step 4: Validate the passcode.
    if (is_stu) {
        auto new_stu = std::make_shared<Student>(inputID);
        if (new_stu->get_passcode() != inputPass) {
            new_stu.reset();
            return nullptr;
        }
        return new_stu;
    }
    auto new_stf = std::make_shared<Staff>(inputID);
    // TODO: 这里写Student new_stu有红色波浪线；建议改成auto. 稍后研究原理。
    if (new_stf->get_passcode()!=inputPass) {
        new_stf.reset();
        return nullptr;
    }
    return new_stf;

}*/


// Testing
// TODO: may remove those after debugging?

void Client::output_basic_info() const {
    std::cout<< "passcode: " << passcode<<"; userID: " << userID << "; userName: " << userName << std::endl;
}

// Feature: Create Course
/**
 * @brief Check whether the input course code is valid.
 * However, whether the course code exists in the data base is not checked.
 * Upper and lower case are treated as different.
 * @param inputCourseCode The input course code to be checked.
 * @return true if the input course code is valid, false if not.
 */
bool Client::is_valid_course_code(std::string & inputCourseCode) {
    if (!inputCourseCode.size() == 7) return false;
    for (int i = 0; i < 3; i++) {
        if (!isupper(inputCourseCode[i])) return false;
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(inputCourseCode[i])) return false;
    }
    return true;
}

/**
 * @brief Check whether the input expression is a valid course expression.
 * However, it's not checked taht whether the course exists in the current data base.
 * @param inputExpr The input expression to be checked.
 * @return true if the input expression is valid, false if not.
 */

bool Client::is_valid_course_expr(std::string &inputExpr) {
    std::stack<char> bracketStack;
    int len = inputExpr.size();
    bool expectOperand = true;

    int i =0;
    while (i<len) {
        char ch = inputExpr[i];

        if (ch == '(') {
            bracketStack.push(ch);
            expectOperand = true;
        } else if (ch == ')') {
            if (bracketStack.empty() || bracketStack.top() != '(') {
                return false;
            }
            bracketStack.pop();
            expectOperand = false;
        } else if (ch == '&' || ch == '|') {
            if (expectOperand) {
                return false;
            }
            expectOperand = true;
        } else if (isupper(ch)) {
            std::string courseCode = inputExpr.substr(i, 7);
            if (!is_valid_course_code(courseCode)) {
                return false;
            }
            i += 7; // Move index to the end of the course code
            expectOperand = false;
            continue;
        } else {
            return false;
        }
        i++;
    }
    return bracketStack.empty() && !expectOperand;
}


// Function to get the current date and time in the format "YYYYMMDDHHMMSS"
std::string Client::get_current_datetime() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    char buffer[15];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", ltm);
    std::string str(buffer);

    return buffer;
}

/**
 * @brief Get the name associated with a given ID.
 *
 * This function checks if the given ID exists and retrieves the corresponding name
 * from the appropriate roster file (either student or staff).
 *
 * @param inputID The ID of the client.
 * @return The name associated with the given ID, or an empty string if the ID does not exist.
 */

std::string Client::id_get_name(std::string &inputID) {
    if (id_exist(inputID))
    {
        std::string work_dir = ".\\sis_ws\\data_repo\\";
        if (inputID[0]=='9') {
            // then is professor
            work_dir+="staff\\staff_roster.txt";
        }else {
            // we assume it's student; although could also registry
            work_dir += "student\\student_roster.txt";
        }
        std::ifstream file(work_dir);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (line.substr(0, 7) == inputID) {
                    return line.substr(8); // Return the name part
                }
            }
        }
    }
    return "";
}




