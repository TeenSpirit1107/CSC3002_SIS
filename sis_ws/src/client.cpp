// cpp lib
#include <iostream>
#include<string>
#include<fstream>
#include<memory>

// sis classes
#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"
#include "course.hpp"

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
    userID(inputID), userName(""), passcode(""),profile_path("")
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

/**
 * @brief Log in to the system.
 * Caveat: Only for staff and student log_in.
 * @return A shared pointer to the client object.
 */

