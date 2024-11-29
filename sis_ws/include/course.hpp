# ifndef COURSE_HPP
# define COURSE_HPP

// cpp lib
# include <string>
# include <queue>
#include <bits/stdc++.h>

// packages
// # include "staff.hpp"
//# include "student.hpp"

using namespace std;

class Course{

    // private, by default
public:
    // attributes

    // used
    short classCode;
    string courseCode;
    string courseName;
    string instructor;
    int capacity;

    string prereq;
    bool grade[4];

    // enrolled student ID
    int enrolled_num;

    // not used
    string major;
    int unit;

    int num_lec, num_tut;
    int lec[28]{}, tut[28]{};
    string lec_classroom[28], tut_classroom[21];

    // constructor destructor
    Course(string CourseCode, string CourseName, string inputIns, int inp_unit, short inputClassCode, int n, int m,
               int inputLec[], int inputTut[], int inputCap);

    explicit Course(short classCode);
    Course();
    ~Course();

    // Filehandling
    void print2File();

    // Tool: print
    void printCourse();

    // Tool: Expression Handling
    static bool is_valid_course_code(string & inputCourseCode);
    static bool is_valid_course_expr(string & inputExpr);
    static std::string requirement2expression(std::string requirement, std::set<std::string> learnt);
    static char cal(char x, char y, char op);
    static bool pre_request_test(const std::string & requirement, std::set<std::string> learnt);

    // Tool: Search
    static std::string get_courseCode(short srt);
    static vector<short> search_course(const std::string & courseCode);
    static vector<int> get_class_time(short class_code);

};
# endif