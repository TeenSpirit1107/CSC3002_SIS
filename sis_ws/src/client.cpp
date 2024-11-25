// cpp lib
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<memory>
#include<vector>
#include<tuple>
#include<algorithm>
#include<cmath>

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
Staff::Staff(const std::string &inputID): Client(inputID) {
    profile_path = staff_path + inputID + ".txt";
    ifstream fileReader(profile_path);

    if (!fileReader.is_open()) {
        printf("Sorry, an error was encountered. Please try again.");
        userID = "ERROR";
        userName = "ERROR";
        passcode = "ERROR";
        // [todo] add a detecting mechanism where this is called, to check whether userID is "ERROR". However, I don't think it's required?
    } else {
        std::string psc;
        std::string name1;
        std::string name2;

        std::string classes; // [todo] read classes
        classes = ""; //testing
        std::getline(fileReader, psc);
        std::getline(fileReader, name1);
        std::getline(fileReader, name2);

        passcode = psc;
        userName = name1 + " " + name2;

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
shared_ptr<Staff> Staff::find_profile(const std::string &inputID) {
    std::string find_path = staff_path + inputID + ".txt";

    // check whether the id exists
    if (!Client::id_exist(inputID)) {
        // case 1, id doesn't exist
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
 * 3 if there's error in updating todo.txt file
 * @param course_name The name of the course.
 * @param pre_req The expression of prerequisites of the course. Not assumed to be valid.
 * @param year The year of the course.
 * @param description The description of the course.
 * @return status code
 */
int Staff::create_course(const std::string &course_name, const std::string &pre_req, const std::string &year, const std::string &description) {
    if (!is_valid_course_expr(pre_req)) return 1;
    std::string file_name = get_current_datetime()+ ".txt";
    std::string file_path = course_claim_path_prefix + "registry\\" + file_name;
    std::string index_dir = ".\\sis_ws\\data_repo\\course_claim\\registry\\to_claim_list.txt";
    std::ofstream os(file_path, std::ios::out);

    //std::ios::in means a new file will not be created if a file with the same name exists.
    if (!os.is_open()) {
        return 2;
    }
    if (update_index_file(index_dir, file_name)!=0) {
        return 3;
    }

    os << course_name << std::endl;
    os << this->userID << std::endl;
    if (pre_req=="") {
        os << "None" <<std::endl;
    }else {
        os << pre_req << std::endl;
    }
    os << year << std::endl;
    os << description << std::endl;
    return 0;
}

// TODO: integer return code indicates normal or error.

/**
 * @brief Computes the final grade for a class.
 *
 * This function reads the scores of students from a file, calculates their average scores,
 * sorts them, and assigns letter grades based on their ranking.
 *
 * Return code explanation:
 * 0 - The final grades were computed and written successfully.
 * 1 - The class code does not exist (file cannot be opened).
 * 2 - The output file cannot be written into (unknown error).
 * 3 - The number of students in the file does not match the expected number.
 *
 * @param class_code The code of the class for which to compute final grades.
 * @return int 0 if successful, 1 if the class code does not exist, 2 if the output file cannot be written, 3 if the number of students does not match.
 */

int Staff::compute_final_grade(short class_code) {
    // TODO: assumption:
    // TODO: 更加复杂的计分方式：不固定的作业、考试次数；加权平均、去掉最差、x代表不被记入总分；通过assignmnet，test次数读取;
    // TODO: skipped assignment and exmas
    //test starts
    int SCORE_NUM = 3;
    // test ends
    string str_class_code = std::to_string(class_code);
    std::string input_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\" + str_class_code + ".txt";
    std::ifstream fileReader = std::ifstream(input_dir);
    if (!fileReader.is_open()) {
        return 1; // indicates the class code doesn't exist
    }

    // TODO: when the class profile is re-structure, staff shall read this value from the file.
    // here for testing I just made up a number.
    int enrolled_num = 4;
    if (class_code == 2) {
        enrolled_num = 9;
    }
    // don't need enrolled stu ID?
    // testing part ends

    // student ID, average grade of the student
    vector<std::tuple<int, double> > id_avg; // student ID, average grade
    std::string line;
    int arr_ind = 0;
    while (std::getline(fileReader, line)) {
        std::istringstream iss(line);
        int student_id;
        double score1, score2, score3;
        iss >> student_id >> score1 >> score2 >> score3;
        double average = (score1 + score2 + score3) / 3.0;
        id_avg.push_back(std::make_tuple(student_id, average));
        arr_ind++;
    }
    fileReader.close();

    if (arr_ind != enrolled_num) {
        return 3; // indicates the number of students in the file doesn't match the number of students in the class.
        // should check our data repo. not supposed to happen because of client's operations, but bc of the error in the code.
    }

    std::sort(id_avg.begin(), id_avg.end(), [](const std::tuple<int, double> &a, const std::tuple<int, double> &b) {
        return std::get<1>(a) > std::get<1>(b);
    });


    // TODO: change this to ...
    // grade: ABCDE, 5 even partitioning
    // test
    int PARTITION_NUM = 5;
    char LETTER_GRADE[PARTITION_NUM] = {'a', 'd', 'g', 'j', 'l'};

    std::string output_dir = ".\\sis_ws\\data_repo\\student_grade\\" + str_class_code + ".txt";

    ofstream fileWriter = ofstream(output_dir);
    if (!fileWriter.is_open()) {
        return 2; // indicates the file cannot be written into. Unkown error.
    }

    for (int i = 0; i < enrolled_num; ++i) {
        std::string id_str = std::to_string(std::get<0>(id_avg[i]));
        int grade_ind = std::floor(PARTITION_NUM*i/enrolled_num);
        char grade = LETTER_GRADE[grade_ind];
        std::string grade_str(1, grade);
        fileWriter << id_str << grade_str << std::endl;
    }
    fileWriter.close();
    return 0;
}

/**
 * @brief Creates a class with the given course code, class code, lecture times, and tutorial times.
 *
 * This function writes the class information to a file in the class claim registry.
 *
 * Return code explanation:
 * 0 - The class was created and written to the file successfully.
 * 1 - The file could not be opened (unknown error).
 * 2 - Error in handling to_claim_list.txt
 *
 * @param course_code
 * @param class_code
 * @param input_lec A vector of integers representing lecture time slots, refer to the Communication Notes.
 * @param input_tut A vector of integers representing tutorial time slots, with the same rule.
 * @return status code.
 */
int Staff::claim_class(const std::string & course_code, short class_code, vector<int> input_lec, vector<int> input_tut) {
    // TODO: before, after? logic.
    // TODO: test this.
    // TODO: claim_class, claim_course, add to local directory.
    // TODO: check class exists

    std::string file_name = get_current_datetime() + ".txt";
    std::string work_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\" + file_name;
    std::string index_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\to_claim_list.txt";

    std::ofstream os(work_dir, std::ios::out);
    if (!os.is_open()) {
        return 1;
    }
    if (update_index_file(index_dir, file_name)!=0) {
        return 2;
    }

    os << course_code << std::endl;
    os << this->userID <<std::endl;
    os << class_code << std::endl;
    os << input_lec.size() << std::endl;
    for (int i = 0; i < input_lec.size(); i++) {
        os << input_lec[i] << std::endl;
    }
    os << input_tut.size() << std::endl;
    for (int i = 0; i < input_tut.size(); i++) {
        os << input_tut[i] << std::endl;
    }
    return 0;
}

Staff::~Staff() {
    //[todo] finish destructor
}
