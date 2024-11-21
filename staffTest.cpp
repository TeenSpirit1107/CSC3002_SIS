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

void printWorkingPathID(string &  inputID) {

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

void printWorkingPath() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }
}

void test_logIn() {
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

void test_create_course_parseCode(int code) {
    if (code==0) {
        cout<<"successfully created\n";
    }else if (code==1) {
        cout<<"the input requisites expression is invalid\n";
    }else if (code==2) {
        cout<<"the file cannot be written into (unknown error)\n";
    }
    else {
        // This is not supposed to happen! 
        cout<<"wrong return code\n";
    }

}

void test_compute_final_grade_parseCode(int code) {
    if (code==0) cout<<"The final grades were computed and written successfully.\n";
    else if (code==1) cout<<"The class code does not exist (file cannot be opened).\n";
    else if (code==2) cout<<"The output file cannot be written into (unknown error).\n";
    else if (code==3) cout<<"The number of students in the file does not match the expected number.\n";
    else cout<<"Wrong return code.\n";
}

void test_create_course(std::string & prereq) {

    std::string inputID = "9100001";
    std::shared_ptr<Staff> s = Staff::find_profile(inputID);
    std::string courseName = "Linear Algebra";
    prereq = "(MAT1001|MAT1005|MAT1011)&(CSC1001|CSC1003)";
    std::string year = "3 1 2 3";
    std::string description = "Introduction to basic topics in linear algebra, including linear independence, bases, and eigenvalue.";

    // case return 0: successfully created
    int i = s->create_course(courseName, prereq, year, description);
    test_create_course_parseCode(i);

    // case return 1: the input requisites expression is invalid
    std::string prereq2 = "(MAT1001|MAT1005|MAT1011)&(CSC1001|CSC1003";
    i = s->create_course(courseName, prereq2, year, description);
    test_create_course_parseCode(i);

    // case return 2: the file cannot be written into . Unknown error or the file already exists.

    // TODO: current problem, overwrite if exists. But no moode could make sure that,
    // 1. if file dne, create;
    // 2. and if file exists, don't open.
    // out: only 1 not 2; __noreplace: only 2 not 1.
    i = s->create_course(courseName, prereq, year, description);
    test_create_course_parseCode(i);

    s.reset();
}

void test_final_grade() {
    test_create_course_parseCode(test_create_course(" (CSC3002|CSC3001)&CSC3100&(MAT1001|MAT1002)
"))
    test_compute_final_grade_parseCode(Staff::compute_final_grade(1));
    test_compute_final_grade_parseCode(Staff::compute_final_grade(2));
}

int main() {

    // test_logIn();
    // test_create_course();
    test_final_grade();

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