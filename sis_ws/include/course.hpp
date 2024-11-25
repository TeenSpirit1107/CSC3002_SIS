# ifndef COURSE_HPP
# define COURSE_HPP

// cpp lib
# include <string>
# include <queue>


// packages
# include "staff.hpp"
//# include "student.hpp"



class Course{

    // private, by default
public:
    // attributes
    string major;
    int unit;
    string courseCode;
    string courseName;
    int capacity;
    short classCode;

    // enrolled student ID
    int enrolled_stu_id[140]{};
    int enrolled_num;

    int num_lec, num_tut;
    int lec[28]{}, tut[28]{};
    // people
    string instructor;
    string lec_classroom[28], tut_classroom[21];
    // Why the length of tut_classroom and tut_sessions doesn't match?
    //queue<Student> stuEnrol;
    //queue<Student> stuWait;

    // to be completed: define a struct requirement, requiring the student's major and year to be ....
    // typedef



    // constructor destructor
    Course(string CourseCode, string CourseName, string inputIns, int inp_unit, short inputClassCode, int n, int m,
               int inputLec[], int inputTut[], int inputCap);

    explicit Course(short classCode);
    ~Course();

    // print
    void printCourse();
    void print2File();
    //void addStu(Student newStu);

    static bool is_valid_course_code(string & inputCourseCode);
    static bool is_valid_course_expr(string & inputExpr);

};
# endif