// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<vector>
#include<iostream>

// sis lib
#include "staff.hpp"

// Definition of const members

const std::string Staff::staff_path = ".\\sis_ws\\data_repo\\staff\\";


/**
* @brief Constructs a Staff object with the given input ID.
*
* This constructor initializes a Staff object by reading the staff profile
* from a file. If the file cannot be opened, the userID, userName, and passcode
* are set to "ERROR". But it's assumed that, before calling this constructor,
* the existence of this ID must have been verified.
*
* @param inputID The ID of the staff member. Guaranteed to exist.
*/
Staff::Staff(std::string & inputID):
    Client(inputID)
{
    profile_path=staff_path+inputID+".txt";
    ifstream fileReader(profile_path);

    if (!fileReader.is_open()) {
        printf("Sorry, an error was encountered. Please try again.");
        userID = "ERROR";
        userName = "ERROR";
        passcode = "ERROR";
        // [todo] add a detecting mechanism where this is called, to check whether userID is "ERROR". However, I don't think it's required?
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
        courses_ = std::vector<Course>();
        
    }
}


/**
 * @brief Finds the profile of a staff member by their ID.
 *
 * This function checks if a staff member with the given ID exists. If the ID exists,
 * it creates a new Staff object on the heap and returns a shared pointer to it.
 * If the ID does not exist, it returns a nullptr.
 *
 * @param inputID The ID of the staff member to find.
 * @return shared_ptr<Staff> A shared pointer to the Staff object if found, otherwise nullptr.
 */
shared_ptr<Staff> Staff::find_profile(std::string &inputID) {
    std::string find_path = staff_path + inputID + ".txt";

    // check whether the id exists
    if(!Client::id_exist(inputID)) {// case 1, id doesn't exist
        return nullptr;
    }
    // case 2, id does exists. Then create a new object in HEAP and return its pointer.
    shared_ptr<Staff> staff_ptr = std::make_shared<Staff>(inputID);
    return staff_ptr;
}

// Feature 1: Create Course


// TODO: 流程。先验证输入，如果不对就提示用户重新输入；对就使用下面这个函数。
// TODO: year这里，用户界面做成“多选”的那种形式？
// TODO: finish comment on this function. return error code.

/**
 * @brief Create a course with the given information.
 *
 * First, validate expression. If valid, then, create the corresponding file.
 * Return code explanation:
 * 0 successfully created and written into file
 * 1 the input requisites expression is invalid -- should ask the user to re-enter.
 * 2 the file cannot be written into (unknown error; or a file with the same name exists)
 * @param course_name The name of the course.
 * @param pre_req The expression of prerequisites of the course. Not assumed to be valid.
 * @param year The year of the course.
 * @param description The description of the course.
 * @return 0 if the course is created successfully, 1 if the input is invalid, 2 if the file cannot be opened.
 */
int Staff::create_course(std::string & course_name, std::string & pre_req, std::string & year, std::string & description) {

    if (!is_valid_course_expr(pre_req)) return 1;
    std::string file_name = course_name + "_"+get_current_datetime()+".txt";
    std::string file_path = course_claim_path_prefix+"registry\\"+file_name;
    std::ofstream os(file_path, std::ios::out) ; //std::ios::in means a new file will not be created if a file with the same name exists.
    printf("Path: %s\n",file_path.c_str());
    if (!os.is_open()) {
        return 2;
        // TODO: add explanation to error code
    }
    os << course_name << std::endl;
    os << this->userID<< std::endl;
    os << pre_req << std::endl;
    os << year << std::endl;
    os << description << std::endl;
    return 0;
}

Staff::~Staff() {
//[todo] finish destructor
}