# ifndef STUDENT_HPP
# define STUDENT_HPP

// cpp lib
#include<string>
#include<vector>
#include<memory>
#include <sstream>
#include <unordered_set>
#include <ctime>
#include<vector>
#include <string>
#include<direct.h>

// packages
#include"client.hpp"
#include"course.hpp"


class Student: public Client{

public:
    // Member Variables
    vector<std::string> shoppingCart;
    unordered_set<int> classes;
    shared_ptr<Course> schedule[49];

    //academic functions
    int searchCourse(std::string coursename);
    int addToShoppingCart(std::string class_number);
    int addClass(int cls_number,std::string add_reason);
    int dropClass(int cls_number,std::string add_reason);
    vector<vector<string>> viewTranscript();
    vector<short> get_course(std::string &inputID);

    int updateAdd();
    int updateDrop();


    //辅助函数
    string readTxt(std::string filename, int line);//返回指定行
    string CharToStr(char * contentChar);
    void DelLineData(char* fileName, int lineNum);//删除指定行
    void ModifyLineData(char* fileName, int lineNum, char* lineData);//修改指定行

    // connections (our new function：加好友)
    vector<Student> frd; // do not write "friend". Friend is a keyword, 友元。
    int addFrd(const std::string &friendID);
    int checkFrd();
    int acceptFrd();

    // The path of the object's profile description file in data_repo.
    static const string student_path;
    static const string stuFrd_path;
    static const string course_path;


    // Member Functions

    explicit Student(const std::string & inputID);
    // Retrieve a student without using passcode; only supposed to be used by someone with permission.
    ~Student();

    // [todo] shall this be made private, but can be visited by "friend" Classes?
    static shared_ptr<Student> find_profile(std::string &inputID);
    //vector<shared_ptr<Course>> Student::get_course(std::string &inputID);

};

# endif