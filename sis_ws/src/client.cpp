
// cpp lib
#include <iostream>
#include<string>
#include<fstream>
#include<memory>
#include<stack>
#include <ctime>


// sis classes
#include "client.hpp"

#include <algorithm>
#include <io.h>

#include "staff.hpp"
#include "student.hpp"
#include "course.hpp"


Client::Client(): course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
    this->userID = "0000000";
    this->userName = "default";
    this->passcode = "123456";
}


// [todo] this constructor with many inputs may be erased if it's not used elsewhere.
Client::Client(const std::string &inputID, const std::string &inputName, const std::string &userPass): userID(inputID),
    userName(inputName), passcode(userPass), course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
}

Client::Client(const std::string &inputID): userID(inputID), userName(""), passcode(""), profile_path(""),
                                            course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
}

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

bool Client::id_is_student(const std::string &inputID) {
    return inputID[0] == '1';
}

// Feature: Log In

bool Client::id_exist(const std::string &inputID) {
    char firstChar = inputID[0];

    // Check whether it's a professor or a student. Find the id in the corresponding path.
    std::string input_path = firstChar == '9'
                                 ? Staff::staff_path + inputID + ".txt"
                                 : Student::student_path + inputID + ".txt";
    std::ifstream file(input_path, std::ios::in);

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
bool Client::validate_passcode(const std::string &inputID, const std::string &inputPass) {
    if (!id_exist(inputID)) {
        printf("Failure: ID doesn't exist.\n");
        return false;
    }
    bool valid_pass = false;
    if (id_is_student(inputID)) {
        std::string work_dir = Student::student_path + inputID + ".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file, real_pass);
        valid_pass = real_pass == inputPass;
    } else {
        std::string work_dir = Staff::staff_path + inputID + ".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file, real_pass);
        valid_pass = real_pass == inputPass;
    }
    return valid_pass;
}

void Client::output_basic_info() const {
    std::cout << "passcode: " << passcode << "; userID: " << userID << "; userName: " << userName << std::endl;
}

// Feature: Create Course

// Tools: Formatting
/**
 * @brief Check whether the input course code is valid.
 * However, whether the course code exists in the data base is not checked.
 * Upper and lower case are treated as different.
 * @param inputCourseCode The input course code to be checked.
 * @return true if the input course code is valid, false if not.
 */
bool Client::is_valid_course_code(const std::string &inputCourseCode) {
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

bool Client::is_valid_course_expr(const std::string &inputExpr) {
    std::stack<char> bracketStack;
    int len = inputExpr.size();
    bool expectOperand = true;

    int i = 0;
    while (i < len) {
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
/**
 * @brief Check whether the input name is legal.
 *
 * A legal name must contain a space separating the first and last name,
 * and both the first and last names must consist of alphabetic characters only.
 * But there's no requirement regarding the case of the characters.
 *
 * @param inputName The input name to be checked.
 * @return true if the input name is legal, false otherwise.
 */
bool Client::is_legal_name(const std::string &inputName) {

    size_t spacePos = inputName.find(' ');

    if (spacePos == std::string::npos || spacePos == 0 || spacePos == inputName.length() - 1) {
        return false;
    }

    std::string firstName = inputName.substr(0, spacePos);
    std::string lastName = inputName.substr(spacePos + 1);

    for (char c : firstName) {
        if (!isalpha(c)) {
            return false;
        }
    }

    for (char c : lastName) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Format the input name.
 *
 * This function formats the input name by converting the first letter of the first name to uppercase
 * and the rest to lowercase, and converting the entire last name to uppercase.
 *
 * @param inputName The input name to be formatted.
 * @return The formatted name.
 */
std::string Client::format_name(const std::string &inputName) {
    size_t spacePos = inputName.find(' ');
    std::string firstName = inputName.substr(0, spacePos);
    std::string lastName = inputName.substr(spacePos + 1);

    // Convert first name to proper format
    firstName[0] = toupper(firstName[0]);
    for (size_t i = 1; i < firstName.length(); ++i) {
        firstName[i] = tolower(firstName[i]);
    }

    // Convert last name to upper case
    for (char &c : lastName) {
        c = toupper(c);
    }

    return firstName + " " + lastName;
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
 * @brief Get the ID associated with a given name.
 *
 * This function checks if the given name is legal and retrieves the corresponding ID
 * from the appropriate roster file (either student or staff).
 *
 * @param inputName The name of the client.
 * @param isStudent A boolean indicating if the client is a student.
 * @return The ID associated with the given name, or an error code:
 *         - "i" if the name is illegal. (Note that unformatted name can be handled; but illegal name cannot.)
 *         - "f" if the roster file could not be opened. (unknown error, not supposed to happen)
 *         - "x" if the name was not found in the roster.
 */
std::string Client::name_get_id(const std::string &inputName, bool isStudent) {
    // TODO: complete this. error code comment.
    if (!is_legal_name(inputName)) return"i";
    std::string keyName = format_name(inputName);
    std::string work_dir;
    if (isStudent) {
        work_dir = Student::student_path + "student_roster.txt";
    }else {
        work_dir = Staff::staff_path + "staff_roster.txt";
    }
    std::ifstream file(work_dir);
    if (!file.is_open()) return "f";
    std::string line;
    while (std::getline(file, line)) {
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            std::string id = line.substr(0, 7);
            std::string name = line.substr(spacePos + 1);
            if (name == keyName) {
                return id;
            }
        }
    }
    return "x";
}

/**
 * @brief Updates the index file with the new file name.
 *
 * This function updates the index file by incrementing the count of files and appending the new file name.
 *
 * Return code explanation:
 * 0 - The index file was updated successfully.
 * 1 - The index file could not be opened.
 * 2 - The first line of the index file is not a valid number.
 *
 * @param index_dir The directory of the index file.
 * @param file_name The name of the new file to be added to the index.
 * @return status code
 */
int Client::update_index_file(const std::string & index_dir, const std::string & file_name) {
    std::fstream fs(index_dir); //TODO: change mode later.
    // TODO: is mode correct?
    // TODO: test the whole function.

    if (!fs.is_open()) {
        return 1;
    }

    std::string s1;
    std::getline(fs, s1);
    if (s1.empty()) {
        fs << 1 << std::endl;
    } else if ( std::all_of(s1.begin(), s1.end(), ::isdigit)) {
        int num = std::stoi(s1);
        num += 1;
        fs.seekp(0, std::ios::beg);
        fs << num << std::endl;
    } else {
        return 2;
    }
    fs.seekp(0, std::ios::end);
    fs << file_name << std::endl;
    return 0;
}
