//
// Created by Tester on 2024/11/26.
//

// include cpp lib
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "stdio.h"
#include <memory>
#include <unistd.h>

// include sis classes
#include "student.hpp"
#include "client.hpp"
#include "staff.hpp"
#include "course.hpp"

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
    std::string inputID = "123090001"; // Example student ID
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
    std::string inputID = "123090001";     // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string friendID = "123090002"; // Example friend's ID
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
    std::string inputID = "123090002"; // Current student's ID
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
    std::string inputID = "123090001"; // Current student's ID
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
    std::string inputID = "123090001"; // Current student's ID
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

void print_schedule(std::array<short,49> sc) {
    std::string time[7] = {"8:30","10:30","13:30","15:30","18:00","19:00","20:00"};
    printf("\tMON\tTUE\tWED\tTHU\tFRI\tSAT\tSUN\n");
    for (int i = 0;i<7;i++) {
        printf("%s\t",time[i].c_str());
        for (int j= 0;j<7;j++) {
            int cls_code = sc[i*7+j];
            if (cls_code>0) {
                std::string cour = Course::get_courseCode(cls_code);
                printf("%s\t",cour.c_str());
            }
            else {
                printf("\t");
            }
        }
        std::cout<<std::endl;
    }
}

void test_get_schedule(){
  shared_ptr<Student> tym = Student::find_profile("1230002");
    printf("name: %s\n",tym->get_userName().c_str());
    print_schedule(tym->get_schedule());
}

void test_find_schedule() {
    vector<short> v;
    v.push_back(1);
    // v.push_back(4);
    v.push_back(1002);
    std::array<short,49> sc = Client::find_schedule(v);
    if (sc[0]==-2) {
        printf("Time conflict within the classes.\n");
    } else {
        printf("Schedule found.\n");
        print_schedule(sc);
    }

}

void test_validation() {
    int i = -20;
    shared_ptr<Student> s = make_shared<Student>("1230002");

    print_schedule(s->get_schedule());
    std::array<short,49> j= Client::find_schedule({1001,3001});

    if (j[0]==-2) {
        printf("no valid schedule\n");
    }else {
        print_schedule(j);
    }

    // Trial 0: expected outcome 0
    short input0[6] = {1001,3001,-1,-1,-1,-1};
    i = s->class_validation(input0);
    printf("Trial 0, expected outcome 0, actual outcome %d\n",i);

    // Trial 1: exceed unit limit
    short input1[6] = {1001,1002,3001,3002,-1,-1};
    i = s->class_validation(input0);
    printf("Trial 0, expected outcome 1, actual outcome %d\n",i);

    // Trial 2.1: expected outcome 2, currently taking this course.
    short input21[6] = {2,-1,-1,-1,-1,-1};
    i = s->class_validation(input21);
    printf("Trial 2.1, expected outcome 2, actual outcome %d\n",i);

    // Triail 2.2: expected outcome 2, already taken this course
    short input22[6] = {3001,-1,-1,-1,-1,-1};
    i = s->class_validation(input22);
    printf("Trial 2.2, expected outcome 2, actual outcome %d\n",i);

    // Trial 4, expected output: -1002. The prerequisite of 1002 is not satisfied.
    short input4[6] = {1002,-1,-1,-1,-1,-1};
    i = s->class_validation(input4);
    printf("Trial 4, expected outcome -1002, actual outcome %d\n",i);

    // Trial 5: time conflict within new schedule
    short input5[6] = {1111,3011,-1,-1,-1,-1};
    i = s->class_validation(input5);
    printf("Trial 5, expected outcome 5, actual outcome %d\n",i);

    // Trial 6: time conflict with currently enrolled schedule
    short input6[6] = {1011,-1,-1,-1,-1,-1};
    i = s->class_validation(input6);
    printf("Trial 6, expected outcome 6, actual outcome %d\n",i);




}




int main() {
    printf("Testing Student Module...\n");

    //string sampStu = "123090001";
    //printWorkingPathID(sampStu);

    //test_logIn();
    //test_find_profile();

//    // Test adding, accepting, and checking friends
//    test_friend_workflow();

    //test_add_to_shopping_cart();

    // ym's student test
    // test_get_schedule();
    // test_find_schedule();
    test_validation();
    printf("Student Module Testing Complete.\n");
    return 0;
}