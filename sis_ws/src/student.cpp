// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<iostream>
#include<sstream>
#include <iomanip>
#include<unordered_set>
#include<set>


// sis classes
#include"student.hpp"
#include"client.hpp"

// Definition of const members
const std::string Student::student_path = ".\\sis_ws\\data_repo\\student\\";
const std::string Student::stuFrd_path= ".\\sis_ws\\data_repo\\addfrds\\";
const std::string Student::class_path= ".\\sis_ws\\data_repo\\class\\";
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

        std::getline(fileReader,psc);
        std::getline(fileReader,name1);
        std::getline(fileReader, name2);

        passcode = psc;
        userName = name1 + " " + name2;

        int class_num;
        fileReader >> class_num;

        for (int i = 0;i<class_num;i++) {
            short cls;
            fileReader>>cls;
            classes.insert(cls);
        }
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

Student::~Student() {
    std::cout << "*Student destructor" << std::endl;
}

// Feature: homework


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

// Feature: friends

//函数--添加好友 (文件夹：addFrds；文件名：（有待处理申请的学号）.txt
//例子：123090001向123090002发送好友申请，addFrds文件夹中有123090002.txt,文件内容为123090001
//返回值：0好友名无效或者找不到好友文件
//1申请发送成功
//2打不开加好友文件
int Student::addFrd(const std::string &friendID) {
    // 学生信息文件路径
    std::string friendFilePath = ".\\sis_ws\\data_repo\\student\\" + friendID + ".txt";
    std::ifstream friendFile(friendFilePath);

    // 检查是否存在好友的个人档案文件
    if (friendFile.is_open()) {
        // 如果好友文件存在，关闭文件流
        friendFile.close();

        // 定义申请好友文件的路径
        std::string addFriendFilePath = ".\\sis_ws\\data_repo\\addFrds\\" + friendID + ".txt";

        // 在 addFrds 文件夹中创建以目标好友学号命名的文件，填入当前用户的id
        std::ofstream addFriendFile(addFriendFilePath);
        if (addFriendFile.is_open()) {
            // 写入当前用户的 ID
            addFriendFile << this->userID << std::endl;
            addFriendFile.close();
            // 输出申请发送的提示
            //std::cout << "已发送申请，请等待对方通过。" << std::endl;
            return 1;
        } else {
            //std::cout << "error:unable to create addFrd file" << std::endl;
            return 2;
        }
    } else {
        // 如果好友文件不存在，输出提示
        //std::cout << "未找到该好友的学号，请检查输入。" << std::endl;
        return 0;
    }
}

//函数--通过好友申请 (文件夹：addFrds；文件名：（有待处理申请的学号）.txt
//访问文件输出待处理的学号，同意1（在文件夹frd两人学号txt中加入对方）或者拒绝0，操作后删除已处理学号，更新加好友文件)
//返回值：0没有待通过的好友
//1通过/拒绝好友成功
//2好友文件无法写入
//3键入值无效，保留该对象的申请
//4保留申请写入申请文件失败
int Student::acceptFrd() {
    // 定义当前用户的好友申请文件路径
    std::string addFrdFilePath = ".\\sis_ws\\data_repo\\addFrds\\" + this->userID + ".txt";
    std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd\\" + this->userID + ".txt";
    // 打开好友申请文件
    std::ifstream inFile(addFrdFilePath);

    // 文件不存在
    if (!inFile.is_open()) {
        //std::cout << "没有待通过的好友。" << std::endl;
        return 0;
    }else{
        // 读取文件内容到内存
        std::vector<std::string> pendingFriends;  // 保存文件中的好友申请学号
        std::string line;
        while (std::getline(inFile, line)) {
            if (!line.empty()) {
                pendingFriends.push_back(line);
            }
        }
        inFile.close();
        //TODO 删除txt文件并新建空文件,但是两个括号删不了文件，我后续再改，目前加好友是不会有影响的
        remove("FrdFilePath");

        // 如果文件内容为空
        if (pendingFriends.empty()) {
            //std::cout << "没有待通过的好友。" << std::endl;
            return 0;
        }

        // 处理每个好友申请
        std::vector<std::string> remainingFriends;  // 保存未处理的好友申请
        for (const std::string &friendID : pendingFriends) {
            std::cout << "Accept/Reject " << friendID << " s friend adding application?"
                                                        " Type integer 1(Accept) or 0(Reject), "
                                                        "any other number be skip&remain the application" << std::endl;

            int userChoice;
            std::cin >> userChoice;

            if (userChoice == 1) {
                // 用户同意好友申请，将好友添加到好友列表
                //this->addFriend(friendID);
                std::ofstream FriendFile(FrdFilePath);
                if (FriendFile.is_open()) {
                    // 写入当前用户的 ID
                    FriendFile << friendID << std::endl;
                    FriendFile.close();
                } else {
                    //std::cout << "错误：好友文件无法写入。" << std::endl;
                    return 2;
                }

                std::string FrdAccPath = ".\\sis_ws\\data_repo\\frd\\" + friendID + ".txt";
                //cout<< FrdAccPath << endl;
                std::ofstream FrdAccFile(FrdAccPath);
                if (FrdAccFile.is_open()) {
                    // 写入当前用户的 ID
                    FrdAccFile << this->userID << std::endl;
                    FrdAccFile.close();
                } else {
                    //std::cout << "错误：好友文件无法写入。" << std::endl;
                }
                //std::cout << "已同意 " << friendID << " 的好友申请。" << std::endl;
                return 1;
            } else if (userChoice == 0) {
                // 用户拒绝好友申请
                //std::cout << "已拒绝 " << friendID << " 的好友申请。" << std::endl;
                return 1;
            } else {
                //std::cout << "输入无效，已保留" << friendID << " 的好友申请。" << std::endl;
                remainingFriends.push_back(friendID);
                return 3;
            }
        }

        // 将未处理的好友申请写回文件
        std::ofstream outFile(addFrdFilePath, std::ios::trunc);  // 覆盖原文件
        if (outFile.is_open()) {
            for (const std::string &friendID : remainingFriends) {
                outFile << friendID << std::endl;
            }
            outFile.close();
            return 1;
        } else {
            //std::cout << "错误：无法更新好友申请文件。" << std::endl;
            return 4;
        }
    }
}

//函数--查看好友列表（打开frd文件夹，输出（学号）.txt内容）
//返回值：0好友文件无法打开
//1好友列表输出成功

int Student::checkFrd() {
    std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd\\" + this->userID + ".txt";
    std::ifstream in(FrdFilePath);
    std::string line;
    if (in.is_open()) {
        while (std::getline(in, line)) {
            cout << line << endl;
            }
        in.close();
        return 1;
    }
    else{
        //std::cout << "无法查看好友列表文件。" << std::endl;
        return 0;
        // [todo]创建用户时就在文件夹创建文件
    }
}

//返回指针--搜索课程代码，返回班级名以及教授，教学时间，课室
//返回值：0输入的课程代码无效
//1搜索成功
//2无法打开Course List.txt
//3无法打开班级文件
//返回班级信息对象，这个部分根据更新正在重写，需要做ptr指针返回


string Student::readTxt(const string & filename, int line){
    //line行数限制 1 - lines
    ifstream text;
    text.open(filename, ios::in);

    vector<string> strVec;
    while (!text.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
    {
        string inbuf;
        getline(text, inbuf, '\n');
        strVec.push_back(inbuf);
    }
    return strVec[line - 1];
}

// Feature: Enrollment
/**
 * @brief Validates the new class schedule for the student.
 *
 * This function checks if the new class schedule provided in `new_class` is valid.
 * It performs various checks including total unit limit, course repetition, grade requirements,
 * prerequisite satisfaction, and time conflicts.
 *
 * @param new_class An array of short integers representing the new class codes.
 * @return int A code indicating the result of the validation:
 *         - 0: Valid schedule.
 *         - A minus number indicates the corresonding class's prerequisite is not satisfied.
 *         - 1: Exceeds total unit limit.
 *         - 2: Course repetition detected.
 *         - 3: Grade requirement not satisfied.
 *         - 5: Time conflict within the new schedule.
 *         - 6: Time conflict with currently enrolled classes.
 *
 */
int Student::class_validation(const short new_class[6]) {

    // Decide the true size
    vector<short> new_class_vec;
    int ctn = 1;
    while (ctn<6) {
        if (new_class[ctn]<=0) {
            // more specifically, should be -1
            break;
        }
        ctn++;
    }

    // 1. Check total unit
    if (ctn+classes.size()>=7) {
        return 1;
    } // exceeds 18 units limit

    // interpret class code to class
    new_class_vec.reserve(ctn);
    for (int i = 0;i<ctn;i++) {
        new_class_vec.push_back(new_class[i]);
    }


    vector<Course> new_classes;
    vector<Course> cur_classes;
    vector<std::string> learnt_vec = get_taken_courses();
    std::set<std::string> learnt_str;
    std::set<std::string> cur_str;

    // 1. new
    for (int i = 0;i<ctn;i++) {
        new_classes.push_back(Course(new_class[i]));
    }
    // 2. current
    for (short s: classes) {
        Course cour = Course(s);
        cur_classes.push_back(cour);
        cur_str.insert(cour.courseCode);
    }
    //3. learnt
    for (std::string s: learnt_vec) {
        learnt_str.insert(s);
    }

    // Check time conflict
    std::array<short,49> cur_sche = get_schedule();
    std::array<short,49> new_sche = find_schedule(new_class_vec);

    if (new_sche[0]==-2) {
        // indicating a conflict within the submitted classes
        return 5; // time conflict within new schedule
    }
    for (int i = 0;i<49;i++) {
        if (new_sche[i]>0&&cur_sche[i]>0) {
            return 6; // time conflict with old schedule
        }
    }

    for (Course nc: new_classes) {

        // Check COURSE repetition
        std::string new_course_code = nc.courseCode;
        if (learnt_str.find(new_course_code)!=learnt_str.end()||cur_str.find(new_course_code)!=cur_str.end()) return 2; // repetition

        // Check grade
        int g;
        int entranceYear = std::stoi(userID.substr(1, 2));
        g = 24 - entranceYear; // 0-indexing; at Term 1, year 2024
        if (!nc.grade[g]) return 3; // not matching grade requirement

        // Check prerequisite
        std::string pr = nc.prereq;

        if (!(Course::pre_request_test(pr,learnt_str))) {
            int ret = -(short(nc.classCode));
            return ret;
        }
    }
    return 0;
    // TODO: Registration
    // TODO: Schedule generation
    // TODO: When submit class schedule, only allow prof select the available time.
    // TODO: after enroll, add studnet to class.
}


/**
 * @brief Adds the student to a specified class.
 *
 * This function updates the class file to include the student and updates the student's profile
 * to include the new class. It reads the current class file, increments the student count, and
 * appends the student's ID with default homework scores. It also updates the student's profile
 * to reflect the new class enrollment.
 *
 * @param class_code The code of the class to add the student to.
 */
void Student::class_add_student(const short class_code) {

    // 1. Add student to course

    std::string class_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\"+std::to_string(class_code)+".txt";
    std::ifstream fileReader(class_dir);
    if (!fileReader.is_open()) return;

    // 1.1 reading backup
    int first_line_number;
    std::vector<std::string> lines;
    std::string line;
    // first line: student count.
    fileReader>>first_line_number;
    while (std::getline(fileReader, line)) {
        lines.push_back(line);
    }
    fileReader.close();
    if (!lines.empty()) {
        lines[0] = std::to_string(first_line_number + 1);
    }

    // 1.2. writing new file with modification
    std::ofstream fileWriter(class_dir);
    if (!fileWriter.is_open()) return;

    for (const auto &l : lines) {
        fileWriter << l << std::endl;
    }
    fileWriter << userID << " -1 -1 -1" << std::endl;
    fileWriter.close();

    // 2. add course to student
    int class_num;
    std::ifstream fileReader2(profile_path);
    if (!fileReader2.is_open()) return;
    std::vector<std::string> lines2;
    std::string line2;
    // read first 3 lines
    for (int i = 0;i<3;i++) {
        std::getline(fileReader2,line2);
        // fileReader>>line2;
        lines2.push_back(line2);
    }
    // read class number
    std::getline(fileReader2,line2);
    try {
        class_num = std::stoi(line2);
    }catch(std::invalid_argument &e) {
    }
    lines2.push_back(std::to_string(class_num+1));


    // while(std::getline(fileReader2,line2)) {
    //     lines2.push_back(line2);
    // }
    for (int i =0;i<class_num;i++) {
        fileReader2>>line2;
        lines2.push_back(line2);
    }
    lines2.push_back(std::to_string(class_code));

    std::ofstream fileWriter2(profile_path);
    if (!fileWriter2.is_open()) return;
    fileWriter2.clear();
    fileWriter2.seekp(0, std::ios::beg);
    for (const auto &l : lines2) {
        fileWriter2 << l << std::endl;
    }

}


void Student::class_remove_student(const short class_code) {

    // 1. read temporary grade; store; do changes

    std::string class_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\"+std::to_string(class_code)+".txt";
    std::ifstream fileReader(class_dir);
    if (!fileReader.is_open()) return;

    int first_line_number;
    int vector_index;
    int line_ctn;
    std::vector<std::string> lines;
    std::string line;
    fileReader>>first_line_number;
    fileReader.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (std::getline(fileReader, line)) {
        if (userID==line.substr(0,7)) vector_index = line_ctn;
        lines.push_back(line);
        line_ctn++;
    }
    fileReader.close();

    if (!lines.empty()) {
        lines[0] = std::to_string(first_line_number -1);
    }

    line_ctn = 0;

    std::ofstream fileWriter(class_dir);
    if (!fileWriter.is_open()) return;

    for (const auto &l : lines) {
        if (line_ctn!=vector_index) {
            fileWriter << l << std::endl;
        }
        line_ctn++;
    }
    fileWriter.close();

    // 2. delete course from student
    int class_num;
    std::string class_code_str = std::to_string(class_code);
    std::ifstream profileReader(profile_path);
    std::vector<std::string> lines2;
    std::string line2;
    for (int i = 0;i<3;i++) {
        std::getline(profileReader,line2);
        lines2.push_back(line2);
    }
    std::getline(profileReader,line2);
    try {
        class_num = std::stoi(line2);
    }catch(std::invalid_argument &e) {

    }
    lines2.push_back(std::to_string(class_num-1));
    while (std::getline(profileReader, line2)) {

        if (class_code_str!=line2) {
            lines2.push_back(line2);
        }

    }

    std::ofstream profileWriter(profile_path);
    if (!profileWriter.is_open()) return;
    for (const auto &l : lines2) {
        profileWriter << l << std::endl;

    }
    profileWriter.close();
}

/**
 * @brief Retrieves the list of courses taken by the student.
 *
 * This function reads the student's transcript file and extracts the course codes
 * of all the courses the student has taken. It returns a vector of strings containing
 * the course codes.
 *
 * @return vector<std::string> A vector containing the course codes of the taken courses.
 */
vector<std::string> Student::get_taken_courses() {
    std::string work_dir =".\\sis_ws\\data_repo\\student\\transcript\\"+userID+".txt";
    ifstream fileReader(work_dir);
    vector<std::string> v;
    if (!fileReader.is_open()) return v;
    std::string line;
    std::getline(fileReader,line); // discard the first line
    while (std::getline(fileReader,line)) {
        v.push_back(line.substr(0,7)); // coursecode only the first seven characters
    }
    return v;
}


/**
 * @brief Generates possible class schedules based on given constraints.
 *
 * Please strictly follow the input rule!
 *
 * @param new_course An array of strings representing the new courses to be considered. Course number <=6; filled from left to right; empty positions filled with empty string "".
 * @param end The number of courses to consider from the new_course array. Namely, the number of non-empty strings parameter "new_course"
 * @param no_eight_am A boolean indicating whether to exclude 8 AM classes. True to exclude.
 * @param unavialable_time A vector of integers representing time slots that are unavailable. 0-index, namely 8:30 a.m., Monday is represented by 0. The largest value is 48.
 * @param prof An array of strings representing preferred professors for each course. Empty strings "" indicate no preference.
 * @param must_class An array of short integers representing specific classes that must be included. -1 indicates no specific class.
 */
vector<std::array<short,6>> Student::generate_schemes(
    const std::string (&new_course)[6],
    const int end,
    bool no_eight_am,
    vector<int> unavialable_time,
    const std::string (&prof)[6],
    std::array<short,6> must_class)
{
    // Initialize a schedule for occupation
    std::array<bool,49> occupied = {false};

    // Value 1: current schedule
    std::array<short,49> my_sche = get_schedule();
    for (int i =0;i<49;i++) {
        if (my_sche[i]>0) {
            occupied[i] = true;
        }
    }

    // Value 2: Excluding eight am
    if (no_eight_am) {
        for (int i = 0;i<7;i++) {
            occupied[i] = true;
        }
    }

    // Value 3: Unavailable time
    for (int i = 0; i<unavialable_time.size();i++) {
        occupied[unavialable_time[i]] = true;
    }

    std::array<short,6> cur_scheme = {-1,-1,-1,-1,-1,-1}; // -1 denotes not selecting


    vector<std::array<short,6>> all_schemes;


    all_schemes=gc_rec(occupied,vector<std::array<short,6>>(), cur_scheme,new_course,0,end);

    // further processing the schemes

        // Iterate over
    for (int k = 0; k<all_schemes.size();k++){
        std::array<short,6> one_scheme = all_schemes[k];
        for (int i =0;i<end;i++) {
            // if there's no valid courses
            if (one_scheme[i]==-1) break;

            // 1. consider must class

            if (i < must_class.size() && must_class[i]!=-1 && must_class[i]!=one_scheme[i]) {

                // all_schemes.erase(std::remove(all_schemes.begin(),all_schemes.end(),one_scheme),all_schemes.end());
                all_schemes.erase(all_schemes.begin()+k);
                k--;
                break;
            }

            // 2. consider instructor

            if (prof[i]!= "" && Course(one_scheme[i]).instructor!=name_get_id(prof[i],false)) {
                all_schemes.erase(all_schemes.begin()+k);
                k--;
                break;
            }
        }
    }

    return all_schemes;
}

/**
 * @brief Recursive function to generate all possible class schedules. A part of the function generate_schemes
 *
 * This function is a recursive function that generates all possible class schedules
 * based on the given constraints. It is called by the generate_schemes function.
 *
 * @param occupied An array of boolean values representing the occupation of each time slot.
 * @param all_schemes A vector of arrays of short integers representing all possible class schedules.
 * @param cur_scheme An array of short integers representing the current class schedule being generated.
 * @param new_course An array of strings representing the new courses to be considered.
 * @param start The starting index of the new_course array.
 * @param end The ending index of the new_course array.
 * @return vector<std::array<short,6>> A vector of arrays of short integers representing all possible class schedules.
 */
vector<std::array<short,6>> Student::gc_rec(
    std::array<bool,49> occupied,
    vector<std::array<short,6>> all_schemes,
    std::array<short,6> cur_scheme,
    const std::string (&new_course)[6],
    int start, int end)
{
    if (start==end) {
        // pased all trials, then a feasible solution.
        // deep copy
        std::array<short,6> temp_scheme;

        // std::copy(std::begin(cur_scheme), std::end(cur_scheme), std::begin(temp_scheme));
        for (int i = 0;i<6;i++) {
            temp_scheme[i] = cur_scheme[i];
        }
        all_schemes.push_back(temp_scheme);

        return all_schemes;
    }


    for (int i =start;i<end;i++) {

        std::string course_code = new_course[i];
        vector<short> all_classes = Course::search_course(course_code);

        for (int j=0; j<all_classes.size(); j++) {
            short cls = all_classes.at(j);

            Course c = Course(cls);
            vector<int> all_time = c.get_class_time(cls);
            bool conflict = false;
            for (int t: all_time) {
                if (occupied[t]) {
                    conflict = true;
                    break; // cannot select
                }
            }
            if (conflict) continue; // move to the next available class
            // else, choose this class
            for (int t: all_time) {
                occupied[t] = true;
            }
            cur_scheme[start] = cls;
            all_schemes = gc_rec(occupied,all_schemes,cur_scheme,new_course,start+1,end);

            // not choose this class
            for (int t: all_time) {
                occupied[t] = false;

            }
        }
    }
    return all_schemes;
}

/**
 * @brief Prints all class schemdule in a scheme of schedules.
 */
void Student::print_scheme(vector<std::array<short,6>> scheme) {
    for (int i = 0;i<scheme.size();i++) {
        std::array<short,6> s = scheme[i];
        for (int j = 0;j<6;j++) {
            printf("%d ",s[j]);
        }
        printf("\n");
    }
}

/**
 * @brief Prints a single class schedule.
 *
 * This function takes an array representing a class schedule and prints
 * each class code in the schedule.
 *
 * @param s An array of short integers representing the class schedule.
 */
void Student::print_scheme_individual(std::array<short,6> s) {
    for (int j = 0;j<6;j++) {
        printf("%d ",s[j]);
    }
}

//函数--查看公告与成绩

//函数--加课

//函数--退课

