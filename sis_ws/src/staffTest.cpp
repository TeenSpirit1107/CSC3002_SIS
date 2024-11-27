//
// Created by Lisa on 2024/11/15.
//

// include cpp lib
#include "stdio.h"
#include <memory>

// testing; for printing current working path
#include <iostream>
#include <unistd.h>
#include<fstream>

// include sis classes
#include <iomanip>

#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"

// expected outcome of this testing file:

/*
passcode: 12345678; userID: 9100001; userName: Ruoyu SUN
Sorry, the ID 3141593 cannot be found.
*/

// Tool: Working Directory
void printWorkingPathID(string &inputID) {
    // also can be done with filesystem, which doesn't support versions below c++17
    // path: "C:\Languages\C_Code\sis_versions\sis1115v1\CSC3002_SIS\cmake-build-debug\CSC3002_SIS.exe"

    // testing; for printing current working path
    ifstream fileReader(".\\sis_ws\\data_repo\\staff\\" + inputID + ".txt");
    if (fileReader.is_open()) {
        printf("Is open\n");
    } else {
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

// Feature: Profile Retrieval

void test_logIn() {
    // the following code implement a log_in procedure.
    // ?? For reference.

    std::string inputID;
    std::string inputPass;

    // TODO: command line io should be replaced with GUI.
    printf("Please enter your ID: \n");
    std::cin >> inputID;
    while (!Client::id_exist(inputID)) {
        printf("Sorry, the ID %s cannot be found.\n", inputID.c_str());
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
        printf("Welcome, student %s!\n", student->get_userName().c_str());
    } else {
        std::shared_ptr<Staff> staff = Staff::find_profile(inputID);
        printf("Welcome, professor %s!\n", staff->get_userName().c_str());
    }
}

void test_find_profile() {

    // ui在做的时候麻烦输入输出改成用户页面的

    // shared_ptr<Staff> ys = std::make_shared<Staff>("9200001");
    shared_ptr<Staff> ys = Staff::find_profile("9200001");
    auto it = (ys->courses.find("STA2001"));
    if (it == (ys->courses).end()) {
        printf("Something's wrong: this is not found in map\n");
        return;
    }
    vector<short> temp_v = it->second;
    Course c = Course(temp_v.at(1));
    c.printCourse();
}

void test_student_find_profile() {
    shared_ptr<Student> tym = Student::find_profile("1230002");
    printf("tym's name: %s\n", tym->get_userName().c_str());
    printf("tym's course number: %d\n", (tym->classes).size());
}

// Feature: Name, ID Handling

void test_find_id_parseCode(int i) {
    if (i == 0) {
    }
}

void test_schedule() {
    shared_ptr<Student> tym = Student::find_profile("1230002");

    (Course(4)).printCourse();
    printf("MON\tTUE\tWED\tTHU\tFRI\tSAT\tSUN\t\n");
    std::array<short, 49> schedule = tym->get_schedule();

    shared_ptr<Course> cour_sche[7][7];

    // value assignment
    int k = 0;
    for (int i =0;i<7;i++) {
        for (int j=0;j<7;j++) {
            if (schedule[k]>0) {
                shared_ptr<Course> c = make_shared<Course>(schedule[k]);
                cour_sche[i][j] = c;
            }else {
                cour_sche[i][j]=nullptr;
            }
            k++;
        }
    }

    for (int i =0;i<7;i++) {
        for (int j=0;j<7;j++) {
            if (!(cour_sche[i][j])) {
                printf("\t");
            }else {
                std::cout<<(cour_sche[i][j])->courseCode<<"\t";
            }


        }
        printf("\n");
    }
}

void test_name_process() {
    std::string illegal_names[5] = {"yimeng", "y1 meng", "   yimeng TENG", "yimeng   TENG", "yimeng TENG   "};
    std::string legal_name6 = "yImEnG tENG";
    for (std::string n: illegal_names) {
        if (Client::is_legal_name(n)) {
            printf("Error: illegal name mistaken as legal.");
            break;
        }
    }
    if (!Client::is_legal_name(legal_name6)) {
        printf("Error: legal name mistaken as illegal.");
    }

    printf("unformatted name: %s\n", legal_name6.c_str());
    std::string formatted_name6 = Client::format_name(legal_name6);
    printf("formatted name: %s\n", formatted_name6.c_str());
}

void test_name_get_id() {
    // Case 1: illegal name (failure)
    if (Client::name_get_id("ym", true) == "i") printf("illegal name test passed.\n");

    // Case 2: non-existing name (failure)
    if (Client::name_get_id("Yimeng TENG", false) == "x") printf("non-existing name test passed.\n");
    // yimeng TENG doesn't exist in the staff repo; but in the student repo. So here it returns "x"

    // Case 3: legal, existing name, student. (success)
    std::string name1 = "yImeng tenG";

    printf("Student Name: %s; Student ID: %s\n", name1.c_str(), Client::name_get_id(name1, true).c_str());
    // Case 4: legal, existing name, staff. (success)
    std::string name2 = "Yangsheng XU";
    printf("Staff Name: %s; Staff ID: %s\n", name2.c_str(), Client::name_get_id(name2, false).c_str());
}


// Feature: course, class create

void test_create_course_parseCode(int code) {
    if (code == 0) {
        cout << "successfully created\n";
    } else if (code == 1) {
        cout << "the input requisites expression is invalid\n";
    } else if (code == 2) {
        cout << "the file cannot be written into (unknown error)\n";
    } else if (code == 3) {
        cout << "error in todo.txt handling\n";
    } else {
        // This is not supposed to happen!
        cout << "wrong return code\n";
    }
}

void test_create_course(const std::string &prereq) {
    std::string inputID = "9100001";
    std::shared_ptr<Staff> s = Staff::find_profile(inputID);
    std::string courseName = "Linear Algebra";
    std::string year = "3 1 2 3";
    std::string description =
            "Introduction to basic topics in Linear Algebra, including linear independence, bases, and eigenvalue.";

    // case return 0: successfully created
    int i = s->create_course(courseName, prereq, year, description);
    test_create_course_parseCode(i);

    // case return 1: the input requisites expression is invalid
    std::string prereq2 = "(MAT1001|MAT1005|MAT1011)&(CSC1001|CSC1003";
    i = s->create_course(courseName, prereq2, year, description);
    test_create_course_parseCode(i);

    // // case return 2: the file cannot be written into . Unknown error or the file already exists.
    //
    // // TODO: current problem, overwrite if exists. But no moode could make sure that,
    // // 1. if file dne, create;
    // // 2. and if file exists, don't open.
    // // out: only 1 not 2; __noreplace: only 2 not 1.
    // i = s->create_course(courseName, prereq, year, description);
    // test_create_course_parseCode(i);

    s.reset();
}

void test_claim_class() {
    std::string inputID = "9100001";
    std::shared_ptr<Staff> s = Staff::find_profile(inputID);
    int i = s->claim_class("CSC3002", 1, {1, 3}, {3});
    if (i == 0) cout << "Class claimed successfully.\n";
    else cout << "Error in claiming class.\n";
    s.reset();
}

void test_profile_add_class () {
    shared_ptr<Staff> ys = Staff::find_profile("9200001");
    //!!!!!!!!!!!!!!!!!!!!!!!!!!
    // MUST READ THE CORRESPONDING FILE (1126 ...) BEFORE DOING THIS!! OTHERWISE MAY CAUSE UNWANTED MODIFICATION TO THE PROFILE.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!
    // ys->profile_add_class(4);
    //
    // for (short st : ys->classes) {
    //     Course c = Course(st);
    //     c.printCourse();
    // }

    // ys->profile_add_class(6);
    // for (short st : ys->classes) {
    //     Course c = Course(st);
    //     c.printCourse();
    // }

    ys.reset();
}

// Feature: Assignment

void test_do_hw() {
    shared_ptr<Student> gly = Student::find_profile("1230001");
    if (gly==nullptr) {
        printf("Error: student not found.\n");
        return;
    }
    int class_code = 1;
    Course c = Course(class_code);
    printf("GLY is going to do %s assignment. He wants to open HW1.\n", c.courseName.c_str());

    // function 1: get homework scores and the homeworks yet to complete.
    vector<double> prev_scr = gly->get_hw_scores(1);
    if (prev_scr.size() == 0) {
        printf("Error: no previous scores found.\n");
        return;
    }

    // list all the home work not yet completed.

    for (int i = 0;i<3;i++) {
        if ((prev_scr.at(i))!=-1) {
            printf("The score for HW %d is %.2f.\n", i+1, prev_scr.at(i));
        }
        else {
            printf("HW %d is not completed.\n", i+1);
        }
    }

    if (!(prev_scr.at(0)==-1)) {
        printf("He has already completed HW1 so he cannot open it. He goes to HW2. \n");
    }else {
        printf("Error: unexpected in test. hw1 is supposed to be done. return.");
        return;
    }

    if (!(prev_scr.at(1)==-1)) {
        printf("Error: unexpected in test. hw2 is NOT supposed to be done. return.");
        return;
    }else {
        printf("He hasn't completed HW2 yet. So he open it.\n");
    }

    ifstream fileReader(".\\sis_ws\\data_repo\\hw\\"+c.courseCode+"\\"+"2.txt"); // 最后的2.txt因为是hw2所以.
    if (!fileReader.is_open()) {
        printf("Error: file not found.\n");
        return;
    }
    std::string line;
    std::getline(fileReader,line);
    printf("The Topic of HW2 is: %s\n", line.c_str());


    int question_num;


    vector<int> v_stu;
    vector<int> v_true;

    fileReader >> question_num;
    v_stu.resize(question_num);
    v_true.resize(question_num);
    double total_score = 0.0;
    double ques_score = double(1)/question_num;

    for (int i = 0;i<question_num;i++) {

        fileReader>>v_true.at(i);
        fileReader.ignore(); // rather than fileReader>>line, which consumes the first character from the next line, too.
        for (int j = 0;j<5;j++) {
            // read one question
            std::getline(fileReader,line);
            std::cout<<line<<std::endl;
        }
        printf("Your answer is: \n"); // 这里1234而不是abcd回答，只是为了处理控制台输入方便。
        std::cin >> v_stu.at(i);

        if (v_stu.at(i)==v_true.at(i)) {
            printf("Your answer is correct.\n");
            total_score+=ques_score;
        }else {
            char c = 96+v_true.at(i);
            std::cout<<"Your answer is wrong, the correct answer is "<<c<<std::endl;
        }
    }
    printf("Your total score is %.2f.\n", total_score);
    gly->set_hw_scores(class_code,2,total_score); // 这里的2是第二次作业。从1开始计数，而不是从0
}


void test_compute_final_grade_parseCode(int code) {
    if (code == 0) cout << "The final grades were computed and written successfully.\n";
    else if (code == 1) cout << "The class code does not exist (file cannot be opened).\n";
    else if (code == 2) cout << "The output file cannot be written into (unknown error).\n";
    else if (code == 3) cout << "The number of students in the file does not match the expected number.\n";
    else cout << "Wrong return code.\n";
}

void test_final_grade() {
    std::string pre = "(CSC3002|CSC3001)&CSC3100&(MAT1001|MAT1002)";
    test_create_course(pre);
    test_compute_final_grade_parseCode(Staff::compute_final_grade(1));
    test_compute_final_grade_parseCode(Staff::compute_final_grade(2));
}


int main() {
    // test_logIn();
    // test_create_course("(CSC3002|CSC3001)&CSC3100&(MAT1001|MAT1002)");
    // test_final_grade();
    // test_name_process();
    // test_name_get_id();
    // test_claim_class();
    // test_profile_add_class();
    // test_find_profile();
    // test_rewrite_file();
    // test_do_hw();
    // test_student_find_profile();
    test_schedule();




    // TODO: course.cpp reading problem?


}
