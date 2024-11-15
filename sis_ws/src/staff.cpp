// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<vector>

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
    Client(inputID),
    profile_path(staff_path+inputID+".txt")
{

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

Staff::~Staff() {
//[todo] finish destructor
}