
// cpp lib
#include <fstream>
#include<iostream>
#include<string>
#include"stdio.h"
#include <bits/stdc++.h>

// sis

#include "course.hpp"

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
}

Course::Course(short class_Code) {

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
    getline(f,courseName);
    string tmp;
    getline(f,tmp);
    getline(f,prereq);
    int num;
    f>>num;

    for (int i = 0; i < num; i++) {
        int x;
        f>>x;
        grade[x-1] = true;
    }
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

// expression handling
std::string Course::requirement2expression(std::string requirement, std::set<std::string> learnt)
{
    int len = requirement.length();
    std::string expression = "";
    for(int i = 0; i < len; i++)
    {
        if(!isalpha(requirement[i])) expression+=requirement[i];
        else
        {
            std::string courseCode = requirement.substr(i, 7);
            if(learnt.find(courseCode) == learnt.end()) expression += "F";
            else expression += "T";
            i += 6;
        }
    }
    return expression;
}
char Course::cal(char x, char y, char op)
{
    if(op == '|') return (x == 'T' || y == 'T')?'T':'F';
    if(op == '&') return (x == 'T' && y == 'T')?'T':'F';
    return 'F'; // not supposed to happen
}
bool Course::pre_request_test(const std::string & requirement, std::set<std::string> learnt)
{
    if (requirement=="None") return true;
    std::string expression = requirement2expression(requirement, learnt);
    std::stack<char> op, num;
    int len = expression.length();
    for(int i = 0; i < len; i++)
    {
        if(expression[i] == 'T' || expression[i] == 'F') num.push(expression[i]);
        else if(expression[i] == '(') op.push(expression[i]);
        else if(expression[i] == ')')
        {
            while(!op.empty() && op.top() != '(')
            {
                char x = num.top(); num.pop();
                char y = num.top(); num.pop();
                num.push(cal(x, y, op.top())); op.pop();
            }
            if(!op.empty()) op.pop();
        }
        else
        {
            while(!op.empty() && op.top() != '(')
            {
                char x = num.top(); num.pop();
                char y = num.top(); num.pop();
                num.push(cal(x, y, op.top())); op.pop();
            }
            op.push(expression[i]);
        }
    }
    while(!op.empty())
    {
        char x = num.top(); num.pop(); char y = num.top(); num.pop();
        num.push(cal(x, y, op.top())); op.pop();
    }
    return num.top() == 'T'?true:false;
}

//search
/**
 * @brief Retrieves the class times for a given class code.
 *
 * This function takes a class code and returns a vector of integers representing
 * the time slots for the lectures and tutorials of the class. The time slots are
 * 0-indexed.
 *
 * @param class_code The class code for which to retrieve the time slots.
 * @return vector<int> A vector of integers representing the time slots for the class.
 */
vector<int> Course::get_class_time(short class_code) {
    vector<int> all_time;
    Course c = Course(class_code);
    int nl = c.num_lec;
    int nt = c.num_tut;
    all_time.reserve(c.num_lec+c.num_tut);
    // 1. add lectures
    for (int i = 0;i< nl;i++) {
        all_time.push_back(c.lec[i]-1); // 1-indexing
    }
    // 2. add tutorials
    for (int i= 0; i<nt; i++) {
        all_time.push_back(c.tut[i]+27);
    }
    return all_time;
}

vector<short> Course::search_course(const std::string & courseCode) {
    vector<short> v;
    std::string work_dir = ".\\sis_ws\\data_repo\\course\\"+courseCode+"_class_arrange.txt";
    ifstream fileReader(work_dir);
    if (!fileReader.is_open()) return v; // course not found or something
    int n;
    fileReader>>n;
    for (int i = 0;i<n;i++) {
        short s ;
        fileReader>>s;
        v.push_back(s);
    }
    return v;
}
/**
 * @brief Retrieves the course code for a given course.
 */
std::string Course::get_courseCode(short srt) {
    Course c = Course(srt);
    return c.courseCode;
}