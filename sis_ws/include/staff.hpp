# ifndef STAFF_HPP
# define STAFF_HPP

// cpp lib
#include<string>

// packages
#include"client.hpp"


class Staff: Client{

    string school;
    void openCourse();

public:
    Staff();
    Staff(string prof_code);
    ~Staff();
    string getName();
};
# endif