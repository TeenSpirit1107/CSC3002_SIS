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
#include "staff.hpp"


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

    // TRIAL 1: an ID that exists.

    std::string inputID = "9100001";
    std::shared_ptr<Staff> staff = Staff::find_profile(inputID);

    if(staff == nullptr) {
        printf("Sorry, the ID %s cannot be found.",inputID.c_str());
    }else{
        staff->output_basic_info();
    }

    // a method for "deleting" a shared pointer
    staff.reset();

    // TRIAL 2: an ID that dne

    inputID = "3141593";
    staff = Staff::find_profile(inputID);

    if(staff == nullptr) {
        printf("Sorry, the ID %s cannot be found.",inputID.c_str());
    }else{
        staff->output_basic_info();
    }
    // a method for "deleting" a shared pointer
    staff.reset();

    return 0;
}