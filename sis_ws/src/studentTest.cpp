
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
    std::string inputID = "1230001";     // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string friendID = "1230002"; // Example friend's ID
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
    std::string inputID = "1230002"; // Current student's ID
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

void test_is_friend(){
    std::string inputID = "1230001";     // Current student's ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string inputID = "1230002"; // Example inputID
        int result = student->addFrd(inputID);

        if (result == 1) {
            printf(" %s is your friend.\n", inputID.c_str());
        } else {
            printf("%s is not your friend.\n", inputID.c_str());
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
        // student->addToShoppingCart(classCode);

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

//在开放OCTE前，一定要get_ProcessOCTE!否则填写OCTE时会找不到文件
void test_get_ProcessOCTE() {
    // 测试获取OCTE处理状态
    std::string inputID = "1230001"; // 示例学生ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        int result = student->get_ProcessOCTE();

        if (result == 0) {
            printf("Unable to open OCTE todo file.\n");
        } else if (result == 1) {
            printf("OCTE todo file created successfully.\n");
        } else if (result == 2) {
            printf("Unable to create OCTE file.\n");
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
}

void test_fill_octe() {
    // 测试填写OCTE申请
    std::string inputID = "1230001"; // 示例学生ID
    std::shared_ptr<Student> student = Student::find_profile(inputID);

    if (student != nullptr) {
        std::string classCode = "6"; // 示例课程代码
        int result = student->fill_octe(classCode);

        if (result == 1) {
            printf("OCTE submitted successfully for class code: %s\n", classCode.c_str());
        } else if (result == 0) {
            printf("Class code %s is not in process class.\n", classCode.c_str());
        } else if (result == 2) {
            printf("Error: Unable to open OCTE question file.\n");
        } else if (result == 3){
            printf("Unable to open OCTE done counting file.\n");
        } else if (result == 4){
            printf("OCTE for class code %s has been finished already, no need to submit again.\n", classCode.c_str());
        }
    } else {
        printf("Student profile not found for ID %s.\n", inputID.c_str());
    }
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
  shared_ptr<Student> tym = Student::find_profile("1230004");
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

void test_search_course() {
    vector<short> v = Course::search_course("MAT1001");
    for (short s: v) {
        std::cout<<s<<" ";
    }
    std::cout<<std::endl;
}

void test_generate_schemes() {

    printf("Generate course selection scheme for Slacker DUDE (Bai zi ge).\n");
    shared_ptr<Student> slacker = Student::find_profile("1230005");
    printf("Trial: MAT1001, without eight am courses. (already tested. not shown here.)\n");
    printf("TRIAL: all scheme containing MAT1001 and CSC3001. Tested, not shown here.\n");
    printf("Trial: MAT1001, CSC3001, MAT1001 specified to take class 1111\n");
    // vector<std::array<short,6>> all_schemes = slacker->generate_schemes({"MAT1001","CSC3001","","","",""},2,false,{},{"","","","","",""},{1111,-1,-1,-1,-1,-1});
    printf("TRIAL: all schemes containing MAT1001, but must be taught by Baoxiang WANG\n"); // or by Xiaokai LIU
    // vector<std::array<short,6>> all_schemes = slacker->generate_schemes({"MAT1001","","","","",""},
    //     1,false,{},{"Baoxiang WANG","","","","",""});
    // printf("TRIAL: all schemes containing MAT1001,CSC3001, but excluding one time slot; MAT1001 must be by Xiaokai LIU\n");
    // vector<std::array<short,6>> all_schemes = slacker->generate_schemes({"MAT1001","CSC3001","","","",""},
    //       2,false,{},{"Xiaokai LIU","","","","",""});
    printf("TRIAL: all schemes containing MAT1001,CSC3001, but excluding one time slot; MAT1001 must be by Xiaokai LIU\n");
    vector<std::array<short,6>> all_schemes = slacker->generate_schemes({"MAT1001","CSC3001","","","",""},
      2,false,{1},{"Xiaokai LIU","","","","",""});
    int n = all_schemes.size();
    printf("Schemes number: %d\n",n);
    for (int i = 0; i < n; i++) {
        printf("Scheme %d\n",i);
        for (int j =0 ;j<6;j++) {
            std::cout<<all_schemes.at(i)[j]<<" ";

        }
        std::cout<<std::endl;
        std::vector<short> scheme_vector(all_schemes.at(i).begin(), all_schemes.at(i).end());
        print_schedule(Client::find_schedule(scheme_vector));
    }

}

void test_erase() {
    vector<std::array<short,6>> v;
    v.push_back({10,20,30,40,50,60});
    v.push_back({-10,-20,-30,-40,-50,-60});
    v.erase(v.begin()+1);
    int n = v.size();
    printf("size: %d\n",n);
    for (int i =0;i<n;i++) {
        for (int j=0;j<6;j++) {
            std::cout<<v.at(i)[j]<<" ";
        }
    }

}


void test_format_userID() {
    int school = 6;
    int school_num = 7;
    std::ostringstream oss;
    oss << "9"<< std::to_string(school)  << std::setw(5) << std::setfill('0') << school_num;
    std::string new_id = oss.str();
    std::cout<<new_id;


    std::ostringstream oss2;
    oss2 << std::to_string(school) << "24" << std::setw(4) << std::setfill('0') << school_num;
    std::cout << oss2.str();
}

void test_register() {
    std::string uid = Client::user_register("John","DOE","jd",true,3);
    std::cout<<"Dear Student, your id is : "<<uid<<std::endl;
    uid = Client::user_register("Cathay","WONG","kw",false,4);
    std::cout<<"Dear Professor, Your id is: "<<uid<<std::endl;
}

void test_remove_class() {
    shared_ptr<Student> lyx = Student::find_profile("1230004");
    lyx->class_remove_student(3011);
    printf("lyx removed from 3011\n");

}

int main() {
    printf("Testing Student Module...\n");

    //string sampStu = "123090001";
    //printWorkingPathID(sampStu);

    //test_logIn();
    //test_find_profile();

    //test_add_to_shopping_cart();

    //test_add_class();
    //test_drop_class();
    //test_view_transcript();
    //test_check_completed();
    //test_update_add();
    //test_update_drop();
    //test_get_ProcessOCTE();
    //test_fill_octe();


    // Test adding, accepting, and checking friends
    //test_add_friend();
    //test_accept_friend();
    //test_check_friend_list();
    //test_is_friend();

    //test_add_to_shopping_cart();

    // ym's student test
    // test_get_schedule();
    // test_find_schedule();
    // test_validation();
    // test_add_class();
    // test_search_course();
    // test_erase();
    // test_generate_schemes();
    // test_format_userID();
    // test_register();
    // test_remove_class();


    printf("Student Module Testing Complete.\n");
    return 0;
}