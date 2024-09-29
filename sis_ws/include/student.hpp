# ifndef STUDENT_HPP
# define STUDENT_HPP

// cpp lib
#include<string>
#include<vector>

// packages
#include"client.hpp"
#include"course.hpp"

using namespace std;

class Student: Client{

    // basic info
    string school;
    string major;
    string college;
    int year;

    // academics
    vector<Course> courseEnrolled;
    vector<Course> courseWait;
    void enrollCourse();

    // connections (our new function：加好友)
    vector<Student> frd; // do not write "friend". Friend is a keyword, 友元。

public:
    Student(int inputID, string inputName, char inputType, string inputPass);
    ~Student();

    // getters
    string getName();
    void setSchool();
    void setMajor();

    // connections
    void addFrd();

};

# endif