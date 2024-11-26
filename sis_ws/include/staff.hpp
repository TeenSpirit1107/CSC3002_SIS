# ifndef STAFF_HPP
# define STAFF_HPP

// cpp lib
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<tuple>

// sis classes
#include <memory>

#include"client.hpp"
#include"registry.hpp"
#include"course.hpp"

class Course;

class Staff: public Client{

public:

    // courses

    std::unordered_map<std::string,std::vector<short>> courses;
    unordered_set<short> classes;
    // string: course code; short: class code.

    friend class Registry;

    // Member Variables

    // TODO: GUI view submitted create course applicatoins

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

    // Feature 1: Claim Course
    int create_course(const std::string & course_name, const std::string & pre_req, const std::string & year, const std::string & description); // TOOD: finish this
// TODO: automatic prereq verification, given a course database large enough already
    // Feature 2: Grade Students in a course
    static int compute_final_grade(short class_code);

    // Feature 3: Claim Class
    int claim_class(const std::string & course_code,short class_code, vector<int> input_lec, vector<int> input_tut);
    void profile_add_class(short class_code);
};

# endif