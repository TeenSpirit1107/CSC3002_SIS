//
// Created by Lisa on 2024/11/15.
//

// include cpp lib
#include "stdio.h"
#include <memory>

// testing; for printing current working path
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include<fstream>

// include sis classes
#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"


// expected outcome of this testing file:

/*
passcode: 12345678; userID: 9100001; userName: Ruoyu SUN
Sorry, the ID 3141593 cannot be found.
*/

void printWorkingPath(string &  inputID) {

    // also can be done with filesystem, which doesn't support versions below c++17
    // path: "C:\Languages\C_Code\sis_versions\sis1115v1\CSC3002_SIS\cmake-build-debug\CSC3002_SIS.exe"

    // testing; for printing current working path
    ifstream fileReader(".\\sis_ws\\data_repo\\staff\\"+inputID+".txt");
    if (fileReader.is_open()) {
        printf("Is open\n");
    }else {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        } else {
            perror("getcwd() error");
        }
    }

}


int main() {

    // the following code implement a log_in procedure.
    // ?? For reference.

    std::string inputID;
    std::string inputPass;

    // TODO: command line io should be replaced with GUI.
    printf("Please enter your ID: \n");
    std::cin >> inputID;
    while (!Client::id_exist(inputID)) {
        printf("Sorry, the ID %s cannot be found.\n",inputID.c_str());
        printf("Please enter your ID again: \n");
        std::cin >> inputID;
    }
    printf("Please enter your passcode: \n");
    std::cin >> inputPass;
    while (!Client::validate_passcode(inputID, inputPass)) {
        printf("Sorry, the passcode is incorrect.\n");
        printf("Please enter your passcode again: \n");
        std::cin >> inputPass;
    }

    bool is_stu = Client::id_is_student(inputID);
    if (is_stu) {
        std::shared_ptr<Student> student = Student::find_profile(inputID);
        printf("Welcome, student %s!\n",student->get_userName().c_str());
    }else {
        std::shared_ptr<Staff> staff = Staff::find_profile(inputID);
        printf("Welcome, professor %s!\n",staff->get_userName().c_str());
    }

    

}

// testing 11/15
// int main() {
//
//     // TRIAL 1: an ID that exists.
//
//     std::string inputID = "9100001";
//     std::shared_ptr<Staff> staff = Staff::find_profile(inputID);
//
//     if(staff == nullptr) {
//         printf("Sorry, the ID %s cannot be found.",inputID.c_str());
//     }else{
//         staff->output_basic_info();
//     }
//
//     // TRIAL 2: an ID that dne
//
//     inputID = "3141593";
//     staff = Staff::find_profile(inputID);
//
//     if(staff == nullptr) {
//         printf("Sorry, the ID %s cannot be found.",inputID.c_str());
//     }else{
//         staff->output_basic_info();
//     }
//     // a method for "deleting" a shared pointer
//     staff.reset();
//
//     // TRIAL 3: General Login for Students or Professor
//
//     printf("Please enter your ID: \n");
//     std::string inputID2;
//     std::cin >> inputID2;
//     printf("Please enter your passcode: \n");
//     std::string inputPass2;
//     std::cin >> inputPass2;
//
//     if (Client::id_is_student(inputID2)) {
//         std::shared_ptr<Student> student = Student::log_in(inputID2, inputPass2);
//         while (student == nullptr) {
//             printf("Sorry, your ID or passcode is incorrect, please try again.\n");
//             printf("Please enter your ID: \n");
//             std::cin >> inputID2;
//             printf("Please enter your passcode: \n");
//             std::cin >> inputPass2;
//         }
//         student->output_basic_info();
//         printf("Welcome, student %s!\n",student->get_userName().c_str());
//
//     }else {
//         std::shared_ptr<Staff> staff2 = Staff::log_in(inputID2, inputPass2);
//         while (staff2 == nullptr) {
//             printf("Sorry, your ID or passcode is incorrect, please try again.\n");
//             printf("Please enter your ID: \n");
//             std::cin >> inputID2;
//             printf("Please enter your passcode: \n");
//             std::cin >> inputPass2;
//         }
//         staff2->output_basic_info();
//         printf("Welcome, staff %s!\n",staff2->get_userName().c_str());
//     }
//     return 0;
// }
