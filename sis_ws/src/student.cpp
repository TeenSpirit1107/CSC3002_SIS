#include"student.hpp"

// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<sstream>

// sis classes
#include <iomanip>

#include"client.hpp"

// Definition of const members
const std::string Student::student_path = ".\\sis_ws\\data_repo\\student\\";

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

Student::Student(const std::string & inputID):
    Client(inputID)
{
    profile_path=student_path+inputID+".txt";
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
shared_ptr<Student> Student::find_profile(const std::string &inputID) {
    std::string find_path = student_path + inputID + ".txt";

    // check whether the id exists
    if(!Client::id_exist(inputID)) {// case 1, id doesn't exist
        return nullptr;
    }
    // case 2, id does exists. Then create a new object in HEAP and return its pointer.
    shared_ptr<Student> student_ptr = std::make_shared<Student>(inputID);
    return student_ptr;
}

// Feature: homework
/**
 * @brief Retrieves the homework scores for a specific class.
 *
 * This function reads the homework scores for the student from a file
 * corresponding to the given class code. It returns a vector containing
 * the scores for the three homework assignments.
 *
 * @param class_code The code of the class.
 * @return vector<double> A vector containing the scores for all three homework assignments, rounded to 2nd decimal; unfinished hw are -1.
 */
vector<double> Student::get_hw_scores(const short class_code) {
    vector<double> v = vector<double>();
    v.resize(3);
    std::string class_code_str = std::to_string(class_code);
    std::string work_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\"+class_code_str+".txt";
    ifstream fileReader(work_dir);
    std::string line;
    std::string key = userID;
    if (fileReader.is_open()) {
        while (std::getline(fileReader,line)) {
            std::istringstream iss(line);
            std::string student_id;
            double score1, score2, score3;
            iss >> student_id >>score1 >> score2 >> score3;
            if (student_id==key) {
                v[0] = score1;
                v[1] = score2;
                v[2] = score3;
                fileReader.close();
                return v;
            }
        }
    }
    return v;
}

/**
 * @brief Sets the homework score for a specific class and homework number.
 *
 * This function updates the homework score for a specific class and homework number
 * for the student. It reads the current scores from a file, updates the score for the
 * specified homework, and writes the updated scores back to the file.
 *
 * @param class_code The code of the class.
 * @param hw_num The homework number (1-based index). (1-BASED INDEX!!!!!)
 * @param new_score The new score to set for the specified homework.
 */
void Student::set_hw_scores(const short class_code, const int hw_num, double new_score) {

    // the front part is similar get_hw_scores()'s file handling
    vector<double> v = vector<double>();
    vector<std::string> lines = vector<std::string>();
    std::string class_code_str = std::to_string(class_code);
    std::string work_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\"+class_code_str+".txt";
    ifstream fileReader(work_dir);
    std::string line;
    std::string key = userID;

    if (!fileReader.is_open()) return;
    while (std::getline(fileReader,line)) {
        if (line.substr(0, 7) == key) {
            std::istringstream iss(line);
            std::string student_id;
            double scores[3];
            iss >> student_id >> scores[0] >> scores[1] >> scores[2];
            if (scores[hw_num-1]==-1) scores[hw_num-1] = new_score; // double check: this is the student's 1st time doing this homework.
            std::ostringstream oss;
            oss << student_id << " " <<std::setprecision(2)<< scores[0] << " " << scores[1] << " " <<scores[2];
            line = oss.str();
        }
        lines.push_back(line);
    }

    fileReader.close();

    ofstream fileWriter(work_dir);
    if (!fileWriter.is_open()) return;
    fileWriter.clear();
    fileWriter.seekp(0, std::ios::beg);
    for (const auto &l : lines) {
        fileWriter << l << std::endl;
    }
}

Student::~Student(){
    // tbc
}
