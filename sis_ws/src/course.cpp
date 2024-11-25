#include "course.hpp"

// cpp lib
#include <fstream>
#include<iostream>
#include<string>
#include"stdio.h"

// packages
#include "staff.hpp"
#include "student.hpp"

using namespace std;

Course::Course(string inputCode, string inputName, string inputIns, int inp_unit, short inputClassCode, int n, int m,
               int inputLec[], int inputTut[], int inputCap) {
    
    // Validate
    if (inputCode.length()!=7){
        cout<<"[System Message] Sorry, the input code is invalid."<<endl;
    }
    if (inputCap<=0){
        cout<<"[System Message] Sorry, the input capacity is invalid."<<endl;
    }

    // if valid, continue to create.
    major = inputCode.substr(0,3);
    courseCode = inputCode;
    courseName = inputName;
    instructor = inputIns;
    unit = inp_unit;
    classCode = inputClassCode;
    num_lec = n; num_tut = m;
    capacity = inputCap;
    for (int i = 0; i < num_lec; i++)
        lec[i] = inputLec[i];
    for (int i = 0; i < num_tut; i++)
        tut[i] = inputTut[i];

    // 11/21 test compute grade
    // TODO: Change this to actual value.
    enrolled_num = 4;
    enrolled_stu_id[0] =1230001;
    enrolled_stu_id[1] =1230002;
    enrolled_stu_id[2] =1230003;
    enrolled_stu_id[3] =1230004;
}

Course::Course(short class_Code) {

    // 11/21test staff grade student
    //
    // TODO: Change this to actual value.
    enrolled_num = 4;
    enrolled_stu_id[0] =1230001;
    enrolled_stu_id[1] =1230002;
    enrolled_stu_id[2] =1230003;
    enrolled_stu_id[3] =1230004;
    // the following are not testing part
    classCode = class_Code;
    string work_dir = ".\\sis_ws\\data_repo\\class\\"+to_string(classCode)+".txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    if (file== nullptr) {
        cout<<"[System Message] File could not be opened."<<endl;
        return;
    }
    char fn[2000];
    fscanf(file, "%s", fn);
    instructor = string(fn);
    fscanf(file, "%s", fn);
    courseCode = string(fn);
    fscanf(file, "%d", &capacity);
    fscanf(file, "%d", &num_lec);
    for (int i = 0; i < num_lec; i++)
        fscanf(file, "%d", &lec[i]);
    fscanf(file, "%d", &num_tut);
    for (int i = 0; i < num_tut; i++)
        fscanf(file, "%d", &tut[i]);
    for (int i = 0; i < num_lec; i++) {
        fscanf(file, "%s", fn);
        lec_classroom[i] = string(fn);
    }
    for (int i = 0; i < num_tut; i++) {
        fscanf(file, "%s", fn);
        tut_classroom[i] = string(fn);
    }
    fclose(file);
    major = courseCode.substr(0,3);

    work_dir = ".\\sis_ws\\data_repo\\course\\"+courseCode+".txt";
    ifstream f(work_dir);
    f >> courseName;
    string tmp;
    f >> tmp;
    f >> tmp;
    f >> tmp;
    f >> unit;
    // [todo] read more information from this file.
    f.close();
}

Course::~Course(){
    // to be completed
}

void Course::printCourse(){
    printf("Course:\n");
    printf("%s: %s\n",courseCode.c_str(), courseName.c_str());
    // printf("Instructor: %s\n",Staff::getName(instructor).c_str()); // this getName() function is not defined yet.
    // [todo] temporarily disabled because the corresponding function has been changed in Staff: can get an object but not a name.
    printf("Maximum Capacity: %d\n", capacity);
    printf("classCode: %hd\n", classCode);
    printf("Number of lecs %d\n", num_lec);
    for (int i = 0; i < num_lec; i++) printf("%d ", lec[i]); printf("\n");
    printf("Number of tuts %d\n", num_tut);
    for (int i = 0; i < num_tut; i++) printf("%d ", tut[i]); printf("\n");
}
void Course::print2File() {
    string work_dir = ".\\sis_ws\\data_repo\\class\\"+to_string(classCode)+".txt";
    FILE *file = fopen(work_dir.c_str(), "w");
    fprintf(file, "%s\n", instructor.c_str());
    fprintf(file, "%s\n", courseCode.c_str());
    fprintf(file, "%d\n", capacity);
    fprintf(file, "%d\n", num_lec);
    for (int i = 0; i < num_lec; i++) fprintf(file, "%d ", lec[i]); printf("\n");
    fprintf(file, "%d\n", num_tut);
    for (int i = 0; i < num_tut; i++) fprintf(file, "%d ", tut[i]); printf("\n");
    for (int i = 0; i < num_lec; i++) fprintf(file, "%s\n", lec_classroom[i].c_str());
    for (int i = 0; i < num_tut; i++) fprintf(file, "%s\n", tut_classroom[i].c_str());
    fclose(file);
}


/*void Course::addStu(Student newStu){

    // case 1: full
    if(stuEnrol.size()==capacity){
        printf("[System Message] Fail to add student %s course %s, because the class is full.\n",(newStu.getName().c_str(),courseCode.c_str()));
        return;
    }

    // case 2: not full
    printf("[System Message] The student %s is successfully added to the course %s. Remaining quota: %d.\n",
    newStu.getName().c_str(),
    courseCode.c_str(),
    capacity - stuEnrol.size());
}*/