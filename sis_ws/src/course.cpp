#include "course.hpp"

// cpp lib
#include<iostream>
#include<string>
#include"stdio.h"

// packages
#include "staff.hpp"
#include "student.hpp"

using namespace std;

Course::Course(string inputCode, string inputName, Staff inputIns, short inputClassCode, int n, int m, int inputLec[], int inputTut[], int inputCap){
    
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
    classCode = inputClassCode;
    num_lec = n; num_tut = m;
    capacity = inputCap;
    for (int i = 0; i < num_lec; i++)
        lec[i] = inputLec[i];
    for (int i = 0; i < num_tut; i++)
        tut[i] = inputTut[i];
}

Course::~Course(){
    // to be completed
}

void Course::printCourse(){
    printf("Course:\n");
    printf("%s: %s\n",courseCode.c_str(), courseName.c_str());
    printf("Instructor: %s\n",instructor.getName().c_str()); // this getName() function is not defined yet.
    printf("Maximum Capacity: %d\n", capacity);
    printf("classCode: %hd\n", classCode);
    printf("Number of lecs %d\n", num_lec);
    for (int i = 0; i < num_lec; i++) printf("%d ", lec[i]); printf("\n");
    printf("Number of tuts %d\n", num_tut);
    for (int i = 0; i < num_tut; i++) printf("%d ", tut[i]); printf("\n");
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