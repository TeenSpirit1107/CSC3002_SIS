
// include cpp lib
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "stdio.h"
#include <memory>
#include <unistd.h>
#include <ctime>
#include<vector>
#include <string>

// include sis classes
#include "student.hpp"
#include "client.hpp"
#include "staff.hpp"

// expected outcome of this testing file:
/*
1. Test logging in as a student and checking basic profile information.
2. Test adding, accepting, and checking friends.
3. Test shopping cart functionality.
4. Test file reading and handling errors.
*/

void printWorkingPathID(string &  inputID) {

    // also can be done with filesystem, which doesn't support versions below c++17
    // path: "C:\Languages\C_Code\sis_versions\sis1115v1\CSC3002_SIS\cmake-build-debug\CSC3002_SIS.exe"

    // testing; for printing current working path
    ifstream fileReader(".\\sis_ws\\data_repo\\student\\"+inputID+".txt");
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

    std::string inputID;
    std::string inputPass;

    // TODO: command line io should be replaced with GUI.
    printf("Please enter your ID: \n");
    std::cin >> inputID;
    //printWorkingPathID(inputID);
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
void test_find_profile() {
    std::string inputID = "1230901"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student == nullptr) {
        printf("Student with ID %s does not exist.\n", inputID.c_str());
    } else {
        printf("Student profile found. Name: %s, ID: %s\n",
               student->get_userName().c_str(),
               student->get_userID().c_str());
    }
}

void test_add_friend() {
    // Test adding a friend
    std::string inputID = "1230901";     // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string friendID = "1230902"; // Example friend's ID
        int result = student->addFrd(friendID);

        if (result == 1) {
            printf("Friend request sent to ID %s.\n", friendID.c_str());
        } else if (result == 0) {
            printf("Friend ID %s not found or invalid.\n", friendID.c_str());
        } else if (result == 2) {
            printf("unable to create addFrd file\n", friendID.c_str());
        } else {
            printf("Unexpected error occurred while adding friend.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_accept_friend() {
    // Test accepting a friend request
    std::string inputID = "1230902"; // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int result = student->acceptFrd();

        if (result == 0) {
            printf("No pending friend requests.\n");
        } else if (result == 1) {
            printf("Friend request accepted or rejected successfully.\n");
        } else if (result == 2) {
            printf("Error: Unable to write to friend file.\n");
        } else if (result == 3) {
            printf("Friend request retained for later.\n");
        } else if (result == 4) {
            printf("Error: Unable to update friend request file.\n");
        } else {
            printf("Unexpected error occurred while accepting friend request.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_check_friend_list() {
    // Test checking the friend list
    std::string inputID = "1230901"; // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int result = student->checkFrd();

        if (result == 1) {
            printf("Friend list displayed successfully.\n");
        } else if (result == 0) {
            printf("Error: Unable to open friend list file.\n");
        } else {
            printf("Unexpected error occurred while checking the friend list.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_add_to_shopping_cart() {
    // Test adding a class to the shopping cart
    std::string inputID = "1230901"; // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string classCode = "CSC3002"; // Example class code
        student->addToShoppingCart(classCode);

        printf("Shopping cart updated. Added class code: %s\n", classCode.c_str());
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_friend_workflow() {
    // Comprehensive test for adding, accepting, and checking friends
    test_add_friend();
    test_accept_friend();
    test_check_friend_list();
}

void test_add_class() {
    // Test adding a class
    std::string inputID = "1230001"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int classNumber = 2; // Example class number
        std::string addReason = "Quota is full, but this course is in my school package."; // Example reason for adding a class
        int result = student->addClass(classNumber, addReason);

        if (result == 1) {
            printf("Class add application handed in successfully. Class number: %d\n", classNumber);
        } else if (result == 0) {
            printf("Error: Add class request file could not be opened.\n");
        } else if (result == 2) {
            printf("Error: Class number file could not be opened.\n");
        } else if (result == 3) {
            printf("Error: Class number %d does not exist.\n", classNumber);
        } else if (result == 4) {
            printf("Error: Unable to view current enrolled classes.\n");
        } else if (result == 5) {
            printf("Error: Already enrolled in class number %d.\n", classNumber);
        } else {
            printf("Unexpected error occurred while adding class.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_drop_class() {
    // Test dropping a class
    std::string inputID = "1230001"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int classNumber = 1; // Example class number
        std::string dropReason = "No longer need this class."; // Example reason for dropping a class
        int result = student->dropClass(classNumber, dropReason);

        if (result == 1) {
            printf("Class drop application handed in successfully. Class number: %d\n", classNumber);
        } else if (result == 0) {
            printf("Error: Drop class request file could not be opened.\n");
        } else if (result == 2) {
            printf("Error: Class number file could not be opened.\n");
        } else if (result == 3) {
            printf("Error: Class number %d does not exist.\n", classNumber);
        } else if (result == 4) {
            printf("Error: Unable to view current enrolled classes.\n");
        } else if (result == 5) {
            printf("Error: Not enrolled in class number %d, cannot drop this class.\n", classNumber);
        } else {
            printf("Unexpected error occurred while dropping class.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_view_transcript() {
    // Test viewing the transcript
    std::string inputID = "1230001"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::vector<std::vector<std::string>> transcript = student->viewTranscript();

        if (!transcript.empty()) {
            printf("Transcript for student ID %s:\n", inputID.c_str());
            for (const auto& record : transcript) {
                for (const auto& field : record) {
                    printf("%s ", field.c_str());
                }
                printf("\n");
            }
        } else {
            printf("Error: Unable to retrieve transcript for student ID %s.\n", inputID.c_str());
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_update_add() {
    // Test updating add class approval
    std::string inputID = "1230001"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int result = student->updateAdd();

        if (result == 0) {
            printf("No pending add class approval updates.\n");
        } else if (result == 1) {
            printf("Add class approval updates retrieved successfully.\n");
        } else if (result == 2) {
            printf("Error: Unable to open add class approval result file.\n");
        } else if (result == 3) {
            printf("Error: Unable to open current enrolled classes file.\n");
        } else {
            printf("Unexpected error occurred while updating add class approval.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_update_drop() {
    // Test updating drop class approval
    std::string inputID = "1230001"; // Example student ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int result = student->updateDrop();

        if (result == 0) {
            printf("No pending drop class approval updates.\n");
        } else if (result == 1) {
            printf("Drop class approval updates retrieved successfully.\n");
        } else if (result == 2) {
            printf("Error: Unable to open drop class approval result file.\n");
        } else if (result == 3) {
            printf("Error: Unable to open current enrolled classes file.\n");
        } else {
            printf("Unexpected error occurred while updating drop class approval.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

int main() {
    printf("Testing Student Module...\n");

    //string sampStu = "123090001";
    //printWorkingPathID(sampStu);

    //test_logIn();
    //test_find_profile();

        // Test adding, accepting, and checking friends
    //test_friend_workflow();

    //test_add_to_shopping_cart();

    //test_add_class();
    //test_drop_class();
    //test_view_transcript();
    //test_check_completed();
    //test_update_add();
    //test_update_drop();

    printf("Student Module Testing Complete.\n");
    return 0;
}