#include"student.hpp"

// cpp lib
#include<string>

// pkg
#include"client.hpp"

using namespace std;

// constructor, destructor

Student::Student(int inputID, string inputName, char inputType, string inputPass):
Client(inputID, inputName, inputType, inputPass)
{
    year = 1;
    // assign the student to a random college
    
}

Student::~Student(){
    // tbc
}

string Student::getName(){
    return userName;
}