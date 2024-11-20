# ifndef STUDENT_HPP
# define STUDENT_HPP

// cpp lib
#include<string>
#include<vector>

// packages
#include"client.hpp"
#include"course.hpp"


class Student: public Client{

    // Member Variables

    // academics
    vector<Course> courseEnrolled;
    vector<Course> courseWait;
    void enrollCourse();

    // connections (our new function：加好友)
    vector<Student> frd; // do not write "friend". Friend is a keyword, 友元。

    // Member Functions


public:

    // Member Variables

    // The path of the object's profile description file in data_repo.
    static const std::string student_path;

    // Member Functions

    explicit Student(std::string & inputID);
    // Retrieve a student without using passcode; only supposed to be used by someone with permission.
    ~Student();

    // [todo] shall this be made private, but can be visited by "friend" Classes?
    static shared_ptr<Student> find_profile(std::string &inputID);



    //
    // // getters
    // string getName();
    // void setSchool();
    // void setMajor();
    //
    // // connections
    // void addFrd();

};



# endif