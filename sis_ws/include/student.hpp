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
#include"course.hpp"


class Student: public Client{

public:
    // Member Variables
    vector<std::string> shoppingCart;

    //functions
    int searchCourse(std::string coursename);
    void addToShoppingCart(std::string class_number);
    void enrollCourse();
    //辅助函数：返回txt文件指定行
    string readTxt(string filename, int line);

    // connections (our new function：加好友)
    vector<Student> frd; // do not write "friend". Friend is a keyword, 友元。
    int addFrd(const std::string &friendID);
    int checkFrd();
    int acceptFrd();
    // Member Functions


public:

    // Member Variables

    // The path of the object's profile description file in data_repo.
    static const string student_path;
    static const string stuFrd_path;
    static const string class_path;


    // Member Functions

    explicit Student(const std::string & inputID);
    // Retrieve a student without using passcode; only supposed to be used by someone with permission.
    ~Student();

    // [todo] shall this be made private, but can be visited by "friend" Classes?

    static shared_ptr<Student> find_profile(std::string &inputID);
    //static shared_ptr<Student> search_class(std::string &inputID);



    // Feature: homework
    vector<double> get_hw_scores (const short class_code);
    void set_hw_scores (const short class_code, const int hw_num, double new_score);
};

# endif