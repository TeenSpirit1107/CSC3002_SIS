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
    ~Registry();
};
# endif