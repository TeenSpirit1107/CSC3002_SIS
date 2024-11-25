# ifndef STAFF_HPP
# define STAFF_HPP

// cpp lib
#include<string>
#include<vector>
#include<tuple>

// sis classes
#include <memory>

#include"client.hpp"
#include"registry.hpp"
#include"course.hpp"

class Course;

class Staff: public Client{

    // Member Variables

    vector<Course> courses_;

    // Member Functions


public:

    friend class Registry;

    // Member Variables

    // The path of the object's profile description file in data_repo.
    static const std::string staff_path;

    // Member Functions

    // Constructor Destructor
    explicit Staff(const std::string & inputID);
    // Retrieve a staff without using passcode; only supposed to be used by someone with permission.
    ~Staff(); // [todo] not yet implemented
    
    static shared_ptr<Staff> find_profile(const std::string & inputID);
    // [todo] shall this be made private, and only be useed by some "friend" classes?

    // Features

    // Feature 1: 创建课程
    int create_course(const std::string & course_name, const std::string & pre_req, const std::string & year, const std::string & description); // TOOD: finish this

    // Featrue 2: Grade Students in a course
    static int compute_final_grade(short class_code);
};

# endif