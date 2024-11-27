# ifndef REGISTRY_HPP
# define REGISTRY_HPP

// std lib
#include<string>

// packages
#include "client.hpp"
using namespace std;

class Registry: public Client{

protected:
    int class_num = 0;

public:
    Registry();
    void claim_course();
    void claim_class();
    void classroom_arrangement();
    void stu_final_grade();
    void Add_and_Drop(bool addition);
    ~Registry();
};
// class Announcement: public Client{
// protected:
//     int textLineCount;
//     string title;
//     string text;
//     string promoter;
//     string targetUser;
//     string promoteTime;
// public:
//     Announcement();
//     void setContent(string title, string text);
//     void setPromoter(string promoter);
//     void setTarget(string idCode);
//     void send();
//     ~Announcement();
// };
# endif