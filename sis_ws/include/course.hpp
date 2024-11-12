# ifndef COURSE_HPP
# define COURSE_HPP

// cpp lib
# include <string>
# include <queue>

// packages
# include "staff.hpp"
# include "student.hpp"

using namespace std;

class Course{

// private, by default

    // attributes
    string major;
    string courseCode;
    string courseName;
    int capacity;
    int classCode;

    // people
    Staff instructor;
    queue<Student> stuEnrol;
    queue<Student> stuWait;
    
    // to be completed: define a struct requirement, requiring the student's major and year to be ....
    // typedef

public:

    // constructor destructor
    Course(string inputCode, short inputName, Staff inputIns, int inputCap=100);
    ~Course();
    
    // print
    void printCourse();
    void addStu(Student newStu);

};
# endif