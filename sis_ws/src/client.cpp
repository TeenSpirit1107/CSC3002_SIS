
// cpp lib
#include <iostream>
#include<string>
#include<fstream>
#include<memory>
#include<stack>
#include <ctime>
#include <algorithm>
#include <io.h>
#include <array>


// sis classes
#include "client.hpp"
#include "staff.hpp"
#include "student.hpp"

Client::Client(): course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
    this->userID = "0000000";
    this->userName = "default";
    this->passcode = "123456";
    this->classes;
}

// [todo] this constructor with many inputs may be erased if it's not used elsewhere.
Client::Client(const std::string &inputID, const std::string &inputName, const std::string &userPass): userID(inputID),
    userName(inputName), passcode(userPass), course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
    this->classes;
}

Client::Client(const std::string &inputID): userID(inputID), userName(""), passcode(""), profile_path(""),
                                            course_claim_path_prefix(".\\sis_ws\\data_repo\\course_claim\\") {
    this->classes;
}

Client::~Client() {
    // [todo] finish destructor
}

// setters and getters

std::string Client::get_userID() const {
    return userID;
}

std::string Client::get_passcode() const {
    return passcode;
}

std::string Client::get_userName() const {
    return userName;
}

// Feature: register
/**
 * @brief Registers a new user (student or professor) and generates a new ID.
 *
 * This function handles the registration process for both students and professors.
 * It generates a new unique ID, updates the corresponding school and roster files,
 * and creates a new profile file for the user.
 *
 * @param inputName1 The first name of the user.
 * @param inputName2 The last name of the user.
 * @param inputPass The passcode for the user.
 * @param isStudent A boolean indicating if the user is a student.
 * @param school The school number associated with the user.
 * @return The newly generated user ID.
 */
std::string Client::user_register(const std::string & inputName1, const std::string & inputName2,
    const std::string & inputPass, bool isStudent, int school){

    std::string new_id;
    int school_num;

    //generate new id
    if (!isStudent) {
        // is professor
        std::string school_path = ".\\sis_ws\\data_repo\\staff\\staff_school.txt";
        ifstream schoolReader(school_path);
        vector<std::string> school_lines;
        std::string line;

        while (std::getline(schoolReader, line)) {
            school_lines.push_back(line);
        }

        int school_num;
        try {
            school_num = std::stoi(school_lines[school-1]);
        }
        catch (std::invalid_argument &e) {
        }
        school_num++;
        school_lines[school-1] = std::to_string(school_num);

        // for (int i =0;i<school-1;i++) {
        //     std::getline(schoolReader,line);
        //     school_lines.push_back(line);
        // }
        // schoolReader>>school_num;
        // school_num++;
        // school_lines.push_back(std::to_string(school_num));
        // for (int i = school;i<6;i++) {
        //     std::getline(schoolReader,line);
        //     school_lines.push_back(line);
        // }
        schoolReader.close();

        std::ostringstream oss;
        oss << "9"<< std::to_string(school)  << std::setw(5) << std::setfill('0') << school_num;
        new_id = oss.str();

        // update school
        ofstream schoolWriter(school_path);
        schoolWriter.clear();
        schoolWriter.seekp(0, std::ios::beg);
        for (const auto &l : school_lines) {
            schoolWriter << l << std::endl;
        }

        // update roster
        std::string roster_path = ".\\sis_ws\\data_repo\\staff\\staff_roster.txt";
        ofstream rosterWriter(roster_path, std::ios::app);
        rosterWriter << new_id << " "<< inputName1 << " " << inputName2 << std::endl;
        rosterWriter.close();

        // update profile
        std::string new_profile_path = ".\\sis_ws\\data_repo\\staff\\" + new_id + ".txt";
        ofstream profileWriter(new_profile_path);
        profileWriter << inputPass << std::endl;
        profileWriter << inputName1 << std::endl;
        profileWriter << inputName2 << std::endl;
        profileWriter << "0" << std::endl;
        profileWriter.close();

    }else {
        std::string school_path = ".\\sis_ws\\data_repo\\student\\student_school.txt";
        ifstream schoolReader(school_path);
        vector<std::string> school_lines;
        std::string line;

        while (std::getline(schoolReader, line)) {
            school_lines.push_back(line);
        }

        int school_num;
        try {
            school_num = std::stoi(school_lines[school-1]);
        }
        catch (std::invalid_argument &e) {
        }
        school_num++;
        school_lines[school-1] = std::to_string(school_num);

        // for (int i =0;i<school-1;i++) {
        //     std::getline(schoolReader,line);
        //     school_lines.push_back(line);
        // }
        // schoolReader>>school_num;
        // school_num++;
        // school_lines.push_back(std::to_string(school_num));
        // for (int i = school;i<6;i++) {
        //     std::getline(schoolReader,line);
        //     school_lines.push_back(line);
        // }
        schoolReader.close();

        std::ostringstream oss;
        oss << std::to_string(school) << "24" << std::setw(4) << std::setfill('0') << school_num;
        new_id = oss.str();

        // update school
        ofstream schoolWriter(school_path);
        schoolWriter.clear();
        schoolWriter.seekp(0, std::ios::beg);
        for (const auto &l : school_lines) {
            schoolWriter << l << std::endl;
        }

        // update roster
        std::string roster_path = ".\\sis_ws\\data_repo\\student\\student_roster.txt";
        ofstream rosterWriter(roster_path, std::ios::app);
        rosterWriter << new_id << " "<< inputName1 << " " << inputName2 << std::endl;
        rosterWriter.close();

        // update profile
        std::string new_profile_path = ".\\sis_ws\\data_repo\\student\\" + new_id + ".txt";
        ofstream profileWriter(new_profile_path);
        profileWriter << inputPass << std::endl;
        profileWriter << inputName1 << std::endl;
        profileWriter << inputName2 << std::endl;
        profileWriter << "0" << std::endl;
        profileWriter.close();

        //update addFrd files
        std::string addFriendDiPath = ".\\sis_ws\\data_repo\\addFrds";
        mkdir(addFriendDiPath.c_str());
        std::string addFriendFilePath = ".\\sis_ws\\data_repo\\addFrds"+ new_id + ".txt";
        std::ofstream addFriendFile(addFriendFilePath);

        //update frd file
        std::string FrdDiPath = ".\\sis_ws\\data_repo\\frd" ;
        mkdir(FrdDiPath.c_str());
        std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd" + new_id + ".txt";
        std::ofstream FrdAccFile(FrdFilePath);

        //update transcript file
        std::string transDiPath = ".\\sis_ws\\data_repo\\student\\transcript";
        mkdir(transDiPath.c_str());
        std::string transPath = ".\\sis_ws\\data_repo\\student\\transcript\\"+ new_id +".txt";
        std::ofstream transFile(transPath);
    }
    return new_id;
}


// Feature: Processing ID

/**
 * @brief Check whether the client is a student.
 * Caveat: When calling this function, you may assume the return value "false" means the client is a staff.
 * However, when client is a registry, it also returns "false".
 * @return true if the client is a student, false if  otherwise.
 */
bool Client::is_student() const {
    return userID[0] == '1';
}


/**
 * @brief Check whether the id is a student's.
 * Caveat: When calling this function, you may assume the return value "false" means the client is a staff.
 * However, when client is a registry, it also returns "false".
 * @return true if the client is a student, false if  otherwise.
 */

bool Client::id_is_student(const std::string &inputID) {
    return inputID[0] == '1';
}

// Feature: Log In

bool Client::id_exist(const std::string &inputID) {
    char firstChar = inputID[0];

    // Check whether it's a professor or a student. Find the id in the corresponding path.
    std::string input_path = firstChar == '9'
                                 ? Staff::staff_path + inputID + ".txt"
                                 : Student::student_path + inputID + ".txt";
    std::ifstream file(input_path, std::ios::in);

    // Case 1: the id doesn't exist.
    if (!file.is_open()) {
        return false;
    }
    // Case 2: the id does exist.
    return true;
}


/**
 * @brief Validate the passcode.
 * @param inputID The ID of the client.
 * @param inputPass The passcode to be validated.
 * @return true if the passcode is correct, false if the ID doesn't exist, or passcode is wrong.
 */
bool Client::validate_passcode(const std::string &inputID, const std::string &inputPass) {
    if (!id_exist(inputID)) {
        printf("Failure: ID doesn't exist.\n");
        return false;
    }
    bool valid_pass = false;
    if (id_is_student(inputID)) {
        std::string work_dir = Student::student_path + inputID + ".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file, real_pass);
        valid_pass = real_pass == inputPass;
    } else {
        std::string work_dir = Staff::staff_path + inputID + ".txt";
        std::ifstream file(work_dir);
        std::string real_pass;
        std::getline(file, real_pass);
        valid_pass = real_pass == inputPass;
    }
    return valid_pass;
}

void Client::output_basic_info() const {
    std::cout << "passcode: " << passcode << "; userID: " << userID << "; userName: " << userName << std::endl;
}

// Feature: Create Course

// Tools: Formatting
/**
 * @brief Check whether the input course code is valid.
 * However, whether the course code exists in the data base is not checked.
 * Upper and lower case are treated as different.
 * @param inputCourseCode The input course code to be checked.
 * @return true if the input course code is valid, false if not.
 */
bool Client::is_valid_course_code(const std::string &inputCourseCode) {
    if (!inputCourseCode.size() == 7) return false;
    for (int i = 0; i < 3; i++) {
        if (!isupper(inputCourseCode[i])) return false;
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(inputCourseCode[i])) return false;
    }
    return true;
}

/**
 * @brief Check whether the input expression is a valid course expression.
 * However, it's not checked taht whether the course exists in the current data base.
 * @param inputExpr The input expression to be checked.
 * @return true if the input expression is valid, false if not.
 */

bool Client::is_valid_course_expr(const std::string &inputExpr) {
    std::stack<char> bracketStack;
    int len = inputExpr.size();
    bool expectOperand = true;

    int i = 0;
    while (i < len) {
        char ch = inputExpr[i];

        if (ch == '(') {
            bracketStack.push(ch);
            expectOperand = true;
        } else if (ch == ')') {
            if (bracketStack.empty() || bracketStack.top() != '(') {
                return false;
            }
            bracketStack.pop();
            expectOperand = false;
        } else if (ch == '&' || ch == '|') {
            if (expectOperand) {
                return false;
            }
            expectOperand = true;
        } else if (isupper(ch)) {
            std::string courseCode = inputExpr.substr(i, 7);
            if (!is_valid_course_code(courseCode)) {
                return false;
            }
            i += 7; // Move index to the end of the course code
            expectOperand = false;
            continue;
        } else {
            return false;
        }
        i++;
    }
    return bracketStack.empty() && !expectOperand;
}
/**
 * @brief Check whether the input name is legal.
 *
 * A legal name must contain a space separating the first and last name,
 * and both the first and last names must consist of alphabetic characters only.
 * But there's no requirement regarding the case of the characters.
 *
 * @param inputName The input name to be checked.
 * @return true if the input name is legal, false otherwise.
 */
bool Client::is_legal_name(const std::string &inputName) {

    size_t spacePos = inputName.find(' ');

    if (spacePos == std::string::npos || spacePos == 0 || spacePos == inputName.length() - 1) {
        return false;
    }

    std::string firstName = inputName.substr(0, spacePos);
    std::string lastName = inputName.substr(spacePos + 1);

    for (char c : firstName) {
        if (!isalpha(c)) {
            return false;
        }
    }

    for (char c : lastName) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Format the input name.
 *
 * This function formats the input name by converting the first letter of the first name to uppercase
 * and the rest to lowercase, and converting the entire last name to uppercase.
 *
 * @param inputName The input name to be formatted.
 * @return The formatted name.
 */
std::string Client::format_name(const std::string &inputName) {
    size_t spacePos = inputName.find(' ');
    std::string firstName = inputName.substr(0, spacePos);
    std::string lastName = inputName.substr(spacePos + 1);

    // Convert first name to proper format
    firstName[0] = toupper(firstName[0]);
    for (size_t i = 1; i < firstName.length(); ++i) {
        firstName[i] = tolower(firstName[i]);
    }

    // Convert last name to upper case
    for (char &c : lastName) {
        c = toupper(c);
    }

    return firstName + " " + lastName;
}

// Function to get the current date and time in the format "YYYYMMDDHHMMSS"
std::string Client::get_current_datetime() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    char buffer[15];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", ltm);
    std::string str(buffer);

    return buffer;
}

/**
 * @brief Get the ID associated with a given name.
 *
 * This function checks if the given name is legal and retrieves the corresponding ID
 * from the appropriate roster file (either student or staff).
 *
 * @param inputName The name of the client.
 * @param isStudent A boolean indicating if the client is a student.
 * @return The ID associated with the given name, or an error code:
 *         - "i" if the name is illegal. (Note that unformatted name can be handled; but illegal name cannot.)
 *         - "f" if the roster file could not be opened. (unknown error, not supposed to happen)
 *         - "x" if the name was not found in the roster.
 */
std::string Client::name_get_id(const std::string &inputName, bool isStudent) {
    // TODO: complete this. error code comment.
    if (!is_legal_name(inputName)) return"i";
    std::string keyName = format_name(inputName);
    std::string work_dir;
    if (isStudent) {
        work_dir = Student::student_path + "student_roster.txt";
    }else {
        work_dir = Staff::staff_path + "staff_roster.txt";
    }
    std::ifstream file(work_dir);
    if (!file.is_open()) return "f";
    std::string line;
    while (std::getline(file, line)) {
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            std::string id = line.substr(0, 7);
            std::string name = line.substr(spacePos + 1);
            if (name == keyName) {
                return id;
            }
        }
    }
    return "x";
}

/**
 * @brief Updates the index file with the new file name.
 *
 * This function updates the index file by incrementing the count of files and appending the new file name.
 *
 * Return code explanation:
 * 0 - The index file was updated successfully.
 * 1 - The index file could not be opened.
 * 2 - The first line of the index file is not a valid number.
 *
 * @param index_dir The directory of the index file.
 * @param file_name The name of the new file to be added to the index.
 * @return status code
 */
int Client::update_index_file(const std::string & index_dir, const std::string & file_name) {
    std::fstream fs(index_dir); //TODO: change mode later.
    // TODO: is mode correct?
    // TODO: test the whole function.

    if (!fs.is_open()) {
        return 1;
    }

    std::string s1;
    std::getline(fs, s1);
    if (s1.empty()) {
        fs << 1 << std::endl;
    } else if ( std::all_of(s1.begin(), s1.end(), ::isdigit)) {
        int num = std::stoi(s1);
        num += 1;
        fs.seekp(0, std::ios::beg);
        fs << num << std::endl;
    } else {
        return 2;
    }
    fs.seekp(0, std::ios::end);
    fs << file_name << std::endl;
    return 0;
}

/**
 * @brief Retrieves the current schedule of the client.
 *
 * This function returns an array representing the current schedule of the client.
 * Each element in the array corresponds to a time slot, and the value indicates the class code
 * scheduled for that time slot. If no class is scheduled for a time slot, the value is -1.
 *
 * This is Different from the function below, which is a static fucntion that gives the schedule generated by a bunch of input classes.
 * @return std::array<short, 49> An array representing the current schedule.
 */
std::array<short, 49> Client::get_schedule() {
    std::array<short,49> schedule;
    for (int i = 0;i<49;i++) {
        schedule[i] = -1;
    }
    for (short srt : classes) {

        // 1. add lectures
        Course c = Course(srt);
        int nl = c.num_lec;

        for (int i = 0;i< nl;i++) {
            schedule[c.lec[i]-1] = srt; // 1-indexing
        }
        // 2. add tutorials
        int nt = c.num_tut;
        for (int i= 0; i<nt; i++) {
            schedule[c.tut[i]+27] = srt;
        }
    }
    return schedule;
}

/**
 * @brief Generates a schedule based on the provided class codes.
 *
 * This function takes a vector of class codes and returns an array representing the schedule.
 * Each element in the array corresponds to a time slot, and the value indicates the class code
 * scheduled for that time slot. If no class is scheduled for a time slot, the value is -1.
 * If there is a conflict in the schedule, the first element of the array is set to -2.
 *
 * @param input_class A vector of class codes to generate the schedule.
 * @return std::array<short, 49> An array representing the generated schedule.
 */
std::array<short, 49> Client::find_schedule(vector<short> input_class) {

    bool err = false;

    // initialized as -1
    std::array<short,49> schedule;
    for (int i = 0;i<49;i++) {
        schedule[i] = -1;
    }

    // update with input classes
    for (short srt : input_class) {
        if (srt==-1) continue;
        //testing
        // 1. add lectures
        Course c = Course(srt);
        int nl = c.num_lec;
        int nt = c.num_tut;

        for (int i = 0;i< nl;i++) {
            if (schedule[c.lec[i]-1]>0){
                err = true;
                break;
            }
            schedule[c.lec[i]-1] = srt; // 0-indexing
        }
        if (err) {
            schedule[0]=-2;
            return schedule;
        }
        // 2. add tutorials
        for (int i= 0; i<nt; i++) {
            if (schedule[c.tut[i]+27]>0) { // + 28 -1
                err = true;
                break;
            }
            schedule[c.tut[i]+27] = srt;
        }
        if (err) {
            schedule[0]=-2;
            return schedule;
        }
    }
    return schedule;
}

// a function for testing.
void Client::print_schedule(const std::array<short,49> (&sc)) {
    std::string time[7] = {"8:30","10:30","13:30","15:30","18:00","19:00","20:00"};
    printf("\tMON\tTUE\tWED\tTHU\tFRI\tSAT\tSUN\n");
    for (int i = 0;i<7;i++) {
        printf("%s\t",time[i].c_str());
        for (int j= 0;j<7;j++) {
            int cls_code = sc[i*7+j];
            if (cls_code>0) {
                std::string cour = Course::get_courseCode(cls_code);
                printf("%s\t",cour.c_str());
            }
            else {
                printf("\t");
            }
        }
        std::cout<<std::endl;
    }
}