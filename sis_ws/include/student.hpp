# ifndef STUDENT_HPP
# define STUDENT_HPP

// cpp lib
#include<string>
#include<vector>
#include<memory>
#include <sstream>
#include <unordered_set>
#include<map>

// packages
#include"client.hpp"

class Student: public Client{

public:
    // Member Variables
    // courses, classes
    // shared_ptr<Course> schedule[49];

    vector<std::string> shoppingCart;

    // The path of the object's profile description file in data_repo.
    static const string student_path;
    static const string stuFrd_path;
    static const string class_path;

    //functions
    vector<int> searchCourse(std::string coursename);
    //static shared_ptr<Student> search_class(std::string &inputID);
    //辅助函数：返回txt文件指定行
    string readTxt(const std::string &filename, int line);

    // connections (our new function：加好友)
    vector<Student> frd; // do not write "friend". Friend is a keyword, 友元。
    int addFrd(const std::string &friendID);
    int checkFrd();
    int acceptFrd();
    // Member Functions

    // Member Variables

    explicit Student(const std::string & inputID);
    // Retrieve a student without using passcode; only supposed to be used by someone with permission.
    ~Student();

    // [todo] shall this be made private, but can be visited by "friend" Classes?

    static shared_ptr<Student> find_profile(const std::string &inputID);


    // Feature: homework
    vector<double> get_hw_scores (const short class_code);
    void set_hw_scores (const short class_code, const int hw_num, double new_score);

    // Feature: Normal Course Registration
    int class_validation(const short new_class[6]);
    void class_enroll();
    void class_add_student(const short class_code);
    void class_remove_student(const short class_code);
    vector<std::string> get_taken_courses();
    vector<std::array<short,6>> generate_schemes(
        const std::string (&new_course)[6],
        const int end, bool no_eight_am = false,
        vector<int> unvailable_time = {},
        const std::string (&prof)[6] = {"","","","","",""},
        std::array<short,6> must_class= {-1,-1,-1,-1,-1,-1});
    vector<std::array<short,6>> gc_rec(
        std::array<bool,49> occupied,
        vector<std::array<short,6>> all_schemes,
        std::array<short,6> cur_scheme,
        const std::string (&new_course)[6],
        int start, int end);

    static void print_scheme(vector<std::array<short,6>> scheme);
    static void print_scheme_individual(std::array<short,6> s);
};

# endif