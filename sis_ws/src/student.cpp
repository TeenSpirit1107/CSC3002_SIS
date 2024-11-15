#include"student.hpp"

// cpp lib
#include<string>
#include<fstream>
#include<memory>

// sis classes
#include"client.hpp"

// Definition of const members
const std::string Student::student_path = ".\\sis_ws\\data_repo\\staff\\";

//Student::Student(int inputID, string inputName, char inputType, string inputPass):
//Client(inputID, inputName, inputType, inputPass)
//{
//    year = 1;
//    // assign the student to a random college
//
//}

/**
* @brief Constructs a Student object with the given input ID.
*
* This constructor initializes a Student object by reading the student profile
* from a file. If the file cannot be opened, the userID, userName, and passcode
* are set to "ERROR". But it's assumed that, before calling this constructor,
* the existence of this ID must have been verified.
*
* @param inputID The ID of the student. Guaranteed to exist.
*/

Student::Student(std::string & inputID):
    Client(inputID),profile_path(student_path+inputID+".txt")
{
    ifstream fileReader(profile_path);
    if (!fileReader.is_open()) {
        printf("Sorry, an error was encountered. Please try again.");
        userID = "ERROR";
        userName = "ERROR";
        passcode = "ERROR";
    }
    else {

        std::string psc;
        std::string name1;
        std::string name2;
        std::string classes; // [todo] read classes
        classes = ""; //testing

        std::getline(fileReader,psc);
        std::getline(fileReader,name1);
        std::getline(fileReader, name2);

        passcode = psc;
        userName = name1 + " " + name2;
// [todo] read and add classes; add friends
    }
}

/**
* @brief Finds the profile of a student by their ID.
*
* This function checks if a student with the given ID exists. If the ID exists,
* it creates a new Student object on the heap and returns a shared pointer to it.
* If the ID does not exist, it returns a nullptr.
*
* @param inputID The ID of the student to find.
* @return shared_ptr<Student> A shared pointer to the Student object if found, otherwise nullptr.
*/
shared_ptr<Student> Student::find_profile(std::string &inputID) {
    std::string find_path = student_path + inputID + ".txt";

    // check whether the id exists
    if(!Client::id_exist(inputID)) {// case 1, id doesn't exist
        return nullptr;
    }
    // case 2, id does exists. Then create a new object in HEAP and return its pointer.
    shared_ptr<Student> student_ptr = std::make_shared<Student>(inputID);
    return student_ptr;
}

Student::~Student(){
    // tbc
}
