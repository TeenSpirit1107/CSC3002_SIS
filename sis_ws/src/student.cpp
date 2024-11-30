// cpp lib
#include<string>
#include<fstream>
#include<iostream>
#include <sstream>

#include <ctime>
#include<memory>
#include<direct.h>
#include <iomanip>
#include<unordered_set>
#include<set>
 #include<vector>

// sis classes
#include"student.hpp"
#include"client.hpp"
#include"course.hpp"

// Definition of const members
const std::string Student::student_path = ".\\sis_ws\\data_repo\\student\\";
const std::string Student::stuFrd_path= ".\\sis_ws\\data_repo\\addfrds\\";
const std::string Student::course_path= ".\\sis_ws\\data_repo\\course\\";

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

//艺萌1126晚帮忙更新的构造器
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
// TODO: schedule
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
        std::ifstream addFriendFile(addFriendFilePath);

        if (addFriendFile.is_open()) {
            // 写入当前用户的 ID
            addFriendFile.close();
            fstream f;
            //追加写入,在原来基础上加了ios::app
            f.open(addFriendFilePath,ios::out|ios::app);
            //输入你想写入的内容
            f<<this->userID<<endl;
            f.close();
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

// Feature: friends

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

        //保存到数组后删除文件然后创建同名空文件
        remove(addFrdFilePath.c_str());
        std::ofstream ipFile(addFrdFilePath);
        ipFile.close();

        // 如果文件内容为空
        if (pendingFriends.empty()) {
            //std::cout << "没有待通过的好友。" << std::endl;
            return 0;
        }

        // 处理每个好友申请
        std::vector<std::string> remainingFriends;  // 保存未处理的好友申请
        for (const std::string &friendID : pendingFriends) {
            std::cout << "Accept/Reject " << friendID << " s friend adding application?"
                                                        " Type integer 1(Accept) or 0(Reject)"<< std::endl;

            int userChoice;
            std::cin >> userChoice;

            if (userChoice == 1) {
                // 用户同意好友申请，将好友添加到好友列表
                //this->addFriend(friendID);
                std::ifstream FriendFile(FrdFilePath);
                if (FriendFile.is_open()) {
                    // 写入当前用户的 ID
                    FriendFile.close();
                    fstream f;
                    //追加写入,在原来基础上加了ios::app
                    f.open(FrdFilePath,ios::out|ios::app);
                    //输入你想写入的内容
                    f<< friendID <<endl;
                    f.close();
                } else {
                    //std::cout << "错误：好友文件无法写入。" << std::endl;
                    return 2;
                }

                std::string FrdAccPath = ".\\sis_ws\\data_repo\\frd\\" + friendID + ".txt";
                //cout<< FrdAccPath << endl;
                std::ifstream FrdAccFile(FrdAccPath);
                if (FrdAccFile.is_open()) {
                    // 写入当前用户的 ID
                    FrdAccFile.close();
                    fstream f;
                    //追加写入,在原来基础上加了ios::app
                    f.open(FrdAccPath,ios::out|ios::app);
                    //输入你想写入的内容
                    f<< this->userID<<endl;
                    f.close();
                } else {
                    //std::cout << "错误：好友文件无法写入。" << std::endl;
                }
                //std::cout << "已同意 " << friendID << " 的好友申请。" << std::endl;

            } else if (userChoice == 0) {
                // 用户拒绝好友申请
                //std::cout << "已拒绝 " << friendID << " 的好友申请。" << std::endl;
            }
            return 1;
            // } else {
            //     //std::cout << "输入无效，已保留" << friendID << " 的好友申请。" << std::endl;
            //     remainingFriends.push_back(friendID);
            //     return 3;
            // }
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

/**
 * @brief check if a student is user's friend
 *
 * input a student's ID and check if it's user's friend
 *
 * @param inputID The ID if the student being checked
 */
bool Student::isFrd(const string &inputID) {
    int iptID = stoi(inputID);
    std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd\\" + this->userID + ".txt";
    std::ifstream in(FrdFilePath);
    std::string line;
    vector<int> FrdIDs;
    if (in.is_open()) {
        while (std::getline(in, line)) {
            FrdIDs.push_back(stoi(line));
        }
        in.close();
        int k = -1;
        for (int i = 0; i < FrdIDs.size(); i++) {
            if (FrdIDs[i] == iptID) {
                k = 1;
            }
        }
        if (k == -1) {
            return false;
        }else{
            return true;
        }
    }else {
        cout<< "Error: Friend file could not be opened, relationship check failed." << endl;
        return false;
    }
}

//函数--搜索课程代码返回下属班级
vector<short> Student::get_course(std::string &inputID) {

    std::string courseListPath = course_path + inputID + "_class_arrange.txt";
    std::ifstream courseFile(courseListPath);
    std::string li;

    // check whether the id exists
    if(!courseFile.is_open()) {// case 1, id doesn't exist
        return std::vector<short>();
    }
    //从文件获取班级代码
    vector<short> classID;
    while (std::getline(courseFile, li)) {
        if (!li.empty()) {
            classID.push_back(stoi(li));

            return classID;
        }else {
            return std::vector<short>();
        }
    }
}



//函数--加入购物车
//返回值：0输入的班级代码不存在
//1：成功加入购物车
int Student::addToShoppingCart(std::string class_number) {
    std::string classSrhPath = ".\\sis_ws\\data_repo\\class\\"+class_number+ ".txt";
    std::ifstream inclassfile(classSrhPath);
    if (inclassfile.is_open()) {
        shoppingCart.push_back(class_number);
        return 1;
    }else{
        //cout<< "输入的班级代码不存在" <<endl;
        return 0;
    }
}

//函数--检查先修与时间后向教务处提交选课(艺萌在做)
//思路：shoppingCart班级代码-》课程代码-》获取先修课（先修课=enrolled=.\data_repo\student\(ID.txt)）
//shoppingCart班级代码-》上课时间
//打开文件夹看先修课，7*7的表格检查时间（注册时0改1）返回成功(加入enrolled)/失败
//这个函数正在写，改完search函数就接着写这个

/*
  **辅助函数：读取txt指定行数据存入string
  *///这个函数的line从0开始计数
string Student::readTxt(string filename, int line){

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

    // update object
    classes.insert(class_code);

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

    auto it = classes.find(class_code);
    if (it!=classes.end()) {
        classes.erase(it);
    }
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
//返回值：0加课申请文件无法打开
//1:加课申请发送成功
//2:班级总数量文件无法打开
//3:不存在该班级代码
//4:无法查看本学期在修课程文件
//5：本学期已经在修该班级，不能重复加课
//6:已写入加课文件，但无法写入toDo文件
int Student::addClass(int cls_number,std::string add_reason) {

    //检查1班级代码是否存在，2是否选上了，没有就创建加课文件
    std::string clsNumPath = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    std::ifstream clsNumfile(clsNumPath);
    if (clsNumfile.is_open()) {
        string TotalNum;
        clsNumfile >> TotalNum;//这地方没有写检查只有一行有点怕怕的，不过这个文件应该就只有一行一个数字
        //string转int
        std::istringstream sst(TotalNum);
        int tnum;
        sst >> tnum;
        if (cls_number  > tnum || cls_number < 1) {
            return 3;//3:不存在该班级代码
        }else{
            //第四行：class总数，接下去就是这学期的class
            std::string inProcessCls = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
            std::ifstream inProcessfile(inProcessCls);
            if (!inProcessfile.is_open()) {
                return 4;//4:无法查看本学期在修课程文件
            }else{
                std::string inPCls = readTxt(inProcessCls, 4); //这个函数的line从1开始计数
                //cout<< " 4 th string(number of class enrolled): " << inPCls << endl;
                int clsNum = stoi(inPCls);
                vector<int> clsPIDs;
                for (int i = 5; i < 5 + clsNum; i++) { //文件第五行开始为班级，如有2个，读到第六行,检验正确）
                    int clsP = stoi(readTxt(inProcessCls, i));
                    clsPIDs.push_back(clsP) ;
                    //cout<< clsP << endl;
                }
                for (int j = 0; j < clsNum; j++) {
                    if (cls_number == clsPIDs[j]) {
                        return 5;//5：本学期已经在修该班级，不能重复加课
                    }
                }

                string path = ".\\sis_ws\\data_repo\\course_add\\"+ this -> userID ;
                mkdir(path.c_str());

                time_t curtime;
                time(&curtime);
                tm *nowtime = localtime(&curtime);
                int year = 1900 + nowtime->tm_year;
                int month = 1 + nowtime->tm_mon;
                int day = nowtime->tm_mday;
                int hour = nowtime->tm_hour;
                int min = nowtime->tm_min;
                std::string filName = std::to_string(year) + std::to_string(month)  + std::to_string(day)
                        + std::to_string(hour)  + std::to_string(min) ;

                std::string addCrsPath = ".\\sis_ws\\data_repo\\course_add\\"+ this -> userID + "\\"+ filName +".txt";
                std::ofstream clsAdFile(addCrsPath);
                if (!clsAdFile.is_open()) {
                    return 0;//0加课申请文件无法打开
                }else{
                    clsAdFile << this -> userID << std::endl;
                    clsAdFile << cls_number << std::endl;
                    clsAdFile << add_reason << std::endl;
                    clsAdFile.close();

                    std::string toDoPath = ".\\sis_ws\\data_repo\\course_add\\staff2reg.txt";
                    std::ofstream tdFile(toDoPath);
                    if (!tdFile.is_open()) {
                        return 6;//6:已写入加课文件，但无法写入toDo文件
                    }else {
                        tdFile <<  addCrsPath << std::endl;
                        return 1;//1:加课申请发送成功
                    }
                }
            }
        }
    }else {
        return 2;//2:班级总数量文件无法打开
    }
}

//函数--退课
//返回值：0退课申请文件无法打开
//1:退课申请发送成功
//2:班级总数量文件无法打开
//3:不存在该班级代码
//4:无法查看本学期在修课程文件
//5：本学期无该在修该班级，不能退课
int Student::dropClass(int cls_number,std::string drop_reason) {

    //检查1班级代码是否存在，2是否选上了，有就创建加课文件
    std::string clsNumPath = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    std::ifstream clsNumfile(clsNumPath);
    if (clsNumfile.is_open()) {
        string TotalNum;
        clsNumfile >> TotalNum;//这地方没有写检查只有一行有点怕怕的，不过这个文件应该就只有一行一个数字
        //string转int
        std::istringstream sst(TotalNum);
        int tnum;
        sst >> tnum;
        if (cls_number  > tnum || cls_number < 1) {
            return 3;//3:不存在该班级代码
        }else{
            //第四行：class总数，接下去就是这学期的class
            std::string inProcessCls = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
            std::ifstream inProcessfile(inProcessCls);
            if (!inProcessfile.is_open()) {
                return 4;//4:无法查看本学期在修课程文件
            }else{
                std::string inPCls = readTxt(inProcessCls, 4); //这个函数的line从1开始计数
                //cout<< " 4 th string(number of class enrolled): " << inPCls << endl;
                int clsNum = stoi(inPCls);
                vector<int> clsPIDs;
                for (int i = 5; i < 5 + clsNum; i++) { //文件第五行开始为班级，如有2个，读到第六行,检验正确）
                    int clsP = stoi(readTxt(inProcessCls, i));
                    clsPIDs.push_back(clsP) ;
                    //cout<< clsP << endl;
                }
                for (int j = 0; j < clsNum; j++) {
                    if( cls_number == clsPIDs[j]) {
                        string path = ".\\sis_ws\\data_repo\\course_drop\\"+ this -> userID ;
                        mkdir(path.c_str());
                        time_t curtime;
                        time(&curtime);
                        tm *nowtime = localtime(&curtime);
                        int year = 1900 + nowtime->tm_year;
                        int month = 1 + nowtime->tm_mon;
                        int day = nowtime->tm_mday;
                        int hour = nowtime->tm_hour;
                        int min = nowtime->tm_min;
                        std::string filName = std::to_string(year) + std::to_string(month)  + std::to_string(day)
                                + std::to_string(hour)  + std::to_string(min) ;

                        std::string dropCrsPath = ".\\sis_ws\\data_repo\\course_drop\\"+ this -> userID + "\\"+ filName +".txt";
                        std::ofstream clsAdFile(dropCrsPath);
                        if (!clsAdFile.is_open()) {
                            return 0;//0退课申请文件无法打开
                        }else{
                            clsAdFile << this -> userID + "\n"<< std::endl;
                            clsAdFile << cls_number + "\n"<< std::endl;
                            clsAdFile << drop_reason << std::endl;
                            clsAdFile.close();
                            std::string toDoPath = ".\\sis_ws\\data_repo\\course_drop\\stu2staff.txt";
                            std::ofstream tdFile(toDoPath);
                            if (!tdFile.is_open()) {
                                return 6;//6:已写入加课文件，但无法写入toDo文件
                            }else {
                                tdFile <<  dropCrsPath << std::endl;
                                return 1;//1:加课申请发送成功
                            }
                        }
                    }
                }
                return 5;//5：本学期无该在修该班级，不能退课
            }
        }
    }else {
        return 2;//2:班级总数量文件无法打开
    }
}

//查看教务处审批加退课update
//返回值：0没有需要更新的加退课审批结果
//1:加退课审批结果更新成功
//2：无法打开加退课审批结果文件
//3:无法打开学生在修列表
int Student::updateAdd() {
    std::string upAdPath = ".\\sis_ws\\data_repo\\course_add\\"+ this-> userID +"\\reg2stu.txt";
    std::ifstream upAdfile(upAdPath);
    if (upAdfile.is_open()) {
        vector<string> TextToRd;
        string line;
        while (getline(upAdfile, line)) {
            TextToRd.push_back(line);
        }
        upAdfile.close();
        //cout<< "size of vector: " <<TextToRd.size() <<endl;

        //加入数组后删除文件，然后新建一个同名空文件
        remove(upAdPath.c_str());
        std::ofstream ipFile(upAdPath);
        ipFile.close();

        //cout<< "size of vector after deletion: " <<TextToRd.size() <<endl;
        //待阅读列表不为空
        if (TextToRd.size() > 0) {
            //跳过第一行开始打开文件
            for (int i = 0; i < TextToRd.size(); i++) {
                std::string p = ".\\sis_ws\\data_repo\\course_add\\"+ this-> userID +"\\"+TextToRd[i];
                //std::ifstream op(p);
                string result1 = readTxt(p, 4);
                int re1 = stoi(result1);
                //cout<< re1 << endl;
                string adClsCode = readTxt(p, 2);
                if (re1 == 1) {
                    string result2 = readTxt(p, 5);
                    int re2 = stoi(result2);
                    if (re2 == 1) {
                        cout<< "classCode: " << adClsCode << " has been added"<<endl;
                        std::string adSucPath = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
                        std::fstream adfile(adSucPath);
                        if (!adfile.is_open()) {
                            return 3;//3:无法打开学生在修列表
                        }else {
                            adfile.close();
                            int clsNum = stoi(readTxt(adSucPath, 4));
                            clsNum = clsNum + 1;
                            string temp_str = std::to_string(clsNum);
                            char * lm =(char*)temp_str.data();

                            char * ff =(char*)adSucPath.data();

                            ModifyLineData(ff, 4, lm);

                            fstream f;
                            //追加写入,在原来基础上加了ios::app
                            f.open(adSucPath,ios::out|ios::app);
                            //输入你想写入的内容
                            f<<adClsCode<<endl;
                            f.close();

                            //return 1;
                        }
                    }else{
                        string failReasonR = readTxt(p, 6);
                        cout<<"Registry reject your class " << adClsCode << " application. Reason: "<<failReasonR<<endl;
                        //return 1;
                    }
                }else {
                    string failReasonP = readTxt(p, 5);
                    cout<<"Professor reject your " << adClsCode << " application. Reason: "<<failReasonP<<endl;
                    //return 1;
                }
            }
            return 1;
        }else {
            return 0;//0没有需要更新的加退课审批结果
        }
    }else {
        return 2;//2：打不开加退课审批结果文件
    }
}

//查看教务处审批加退课update
//返回值：0没有需要更新的加退课审批结果
//1:加退课审批结果更新成功
//2：无法打开加退课审批结果文件
//3:无法打开学生在修列表
//4：出现了未知错误或者退课的课不在当前修课列表
int Student::updateDrop() {
    std::string upDpPath = ".\\sis_ws\\data_repo\\course_drop\\"+ this-> userID +"\\reg2stu.txt";
    std::ifstream upDpfile(upDpPath);
    if (upDpfile.is_open()) {
        vector<string> TextToRd;
        string line;
        while (getline(upDpfile, line)) {
            TextToRd.push_back(line);
        }
        upDpfile.close();
        //cout<< "size of vector: " <<TextToRd.size() <<endl;

        //加入数组后删除文件，然后新建一个同名空文件
        // remove(upDpPath.c_str());
        // std::ofstream ipFile(upDpPath);
        // ipFile.close();

        //待阅读列表不为空
        if (TextToRd.size() > 0) {
            //跳过第一行开始打开文件

            for (int i = 0; i < TextToRd.size(); i++) {
                std::string p = ".\\sis_ws\\data_repo\\course_drop\\"+ this-> userID +"\\"+TextToRd[i];
                std::ifstream op(p);
                string result1 = readTxt(p, 4);
                int re1 = stoi(result1);
                string dpClsCode = readTxt(p, 2);
                if (re1 == 1) {
                    string result2 = readTxt(p, 5);
                    int re2 = stoi(result2);
                    if (re2 == 1) {

                        std::string dpSucPath = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
                        std::ifstream dpfile(dpSucPath);
                        if (!dpfile.is_open()) {
                            return 3;//3:无法打开学生在修列表
                        }else {
                            int dpClassCode = stoi(dpClsCode);
                            //读取文件，比对班级，删除指定行
                            std::string inProcessCls = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
                            std::ifstream inProcessfile(inProcessCls);
                            if (!inProcessfile.is_open()) {
                                return 3;
                            }else {
                                std::string inPCls = readTxt(inProcessCls, 4); //这个函数的line从1开始计数
                                //cout<< " 4 th string(number of class enrolled): " << inPCls << endl;
                                int clsNum = stoi(inPCls);
                                vector<int> clsPIDs;
                                for (int i = 5; i < 5 + clsNum; i++) { //文件第五行开始为班级，如有2个，读到第六行,检验正确）
                                    int clsP = stoi(readTxt(inProcessCls, i));
                                    clsPIDs.push_back(clsP) ;
                                    //cout<< clsP << endl;
                                }
                                int dpLine = -1;
                                for (int j = 0; j < clsNum; j++) {
                                    if (dpClassCode == clsPIDs[j]) {
                                        dpLine = j;//第5+dpLine应该被删除
                                    }
                                }
                                if (dpLine == -1) {
                                    return 4;//4：出现了未知错误或者退课的课不在当前修课列表
                                }else {
                                    char * f=(char*)inProcessCls.data();

                                    //在修课程总数减一
                                    int upClsNum = clsNum - 1;
                                    string temp_str = std::to_string(upClsNum);
                                    char * q=(char*)temp_str.data();
                                    ModifyLineData(f, 4, q);

                                    //删去5+dpLine行
                                    DelLineData(f, 5+dpLine);
                                    vector<string> newTx;
                                    while(getline(inProcessfile, line)) {
                                        if (line.empty()) {
                                            continue;
                                        }else {
                                            newTx.push_back(line);
                                        }
                                    }

                                    remove(inProcessCls.c_str());
                                    std::ofstream ipFile(inProcessCls);
                                    for (int i = 0; i < newTx.size(); i++) {
                                        ipFile << newTx[i];
                                        ipFile << "\n";
                                    }
                                    ipFile.close();
                                    cout<< "classCode: " << dpClsCode << " has been dropped"<<endl;

                                }

                            }

                        }
                    }else{
                        string failReasonR = readTxt(p, 6);
                        cout<<"Registry reject your class " << dpClsCode << " drop. Reason: "<<failReasonR<<endl;

                    }
                }else {
                    string failReasonP = readTxt(p, 5);
                    cout<<"Registry reject your class " << dpClsCode << " drop. Reason: "<<failReasonP<<endl;

                }
            }
            return 1;
        }
    }else {
        return 2;//2：打不开加退课审批结果文件
    }
}

//辅助函数：char转string
string Student::CharToStr(char * contentChar)
{
    string tempStr;
    for (int i=0;contentChar[i]!='\0';i++)
    {
        tempStr+=contentChar[i];
    }
    return tempStr;
}
//辅助函数：删除指定行
void Student::DelLineData(char* fileName, int lineNum)
{
    ifstream in;
    in.open(fileName);

    string strFileData = "";
    int line = 1;
    char lineData[1024] = {0};
    while(in.getline(lineData, sizeof(lineData)))
    {
        if (line == lineNum)
        {
            strFileData += "\n";
        }
        else
        {
            strFileData += CharToStr(lineData);
            strFileData += "\n";
        }
        line++;
    }
    in.close();

    //写入文件
    ofstream out;
    out.open(fileName);
    out.flush();
    out<<strFileData;
    out.close();
}
//辅助函数：修改指定行
void Student::ModifyLineData(char* fileName, int lineNum, char* lineData)
{
    ifstream in;
    in.open(fileName);

    string strFileData = "";
    int line = 1;
    char tmpLineData[1024] = {0};
    while(in.getline(tmpLineData, sizeof(tmpLineData)))
    {
        if (line == lineNum)
        {
            strFileData += CharToStr(lineData);
            strFileData += "\n";
        }
        else
        {
            strFileData += CharToStr(tmpLineData);
            strFileData += "\n";
        }
        line++;
    }
    in.close();

    //写入文件
    ofstream out;
    out.open(fileName);
    out.flush();
    out<<strFileData;
    out.close();
}


//函数--查看公告(ui已融合)

//函数--返回先修课列表（已删除，艺萌写的更简洁）

//函数--查看成绩
vector<vector<string>> Student::viewTranscript() {
    std::string transPath = ".\\sis_ws\\data_repo\\student\\transcript\\"+ this->userID +".txt";
    std::ifstream transfile(transPath);
    if (!transfile.is_open()) {
        return vector<vector<string>>();
    }else {
        int trans_number = stoi(readTxt(transPath, 1));
        //cout<< trans_number << endl;
        vector<vector<string>> trans(trans_number+1,vector<string>(2));
        trans[trans_number]=vector<string>(3);//最后一行是3个string
        //获取每一行，空格分割放进vector<string>,行数计数器i，将这个vector放进大vector[cnt]
        int cnt = 0;
        for (int cnt = 0; cnt < trans_number; cnt++) {
            string rdL = readTxt(transPath, cnt + 2); //1：第二行开始为成绩
            vector<string> lineV;
            istringstream in(rdL);
            while (getline(in, rdL, ' ')) {	//这里单引号要注意
                lineV.push_back(rdL);
            }
            trans[cnt] = lineV;
        }
        return trans;
    }
}

//函数--创建octe待填文件
//预设前提：填写octe时默认在修不会变动
//返回值：0无法打开在修课程文件
//1：成功创建octe待填文件
//2:无法创建student的OCTE文件
int Student::get_ProcessOCTE() {
    std::string inProcessCls = ".\\sis_ws\\data_repo\\student\\"+ this-> userID +".txt";
    std::ifstream inProcessfile(inProcessCls);
    if (!inProcessfile.is_open()) {
        return 0;
    }else{
        inProcessfile.close();
        vector<string> ProcessCls;
        string clsNum = readTxt(inProcessCls, 4);
        int cNum = stoi(clsNum);
        for (int cnt = 5; cnt < 5 + cNum; cnt++) {
            ProcessCls.push_back(readTxt(inProcessCls, cnt));
        }
        std::string stuOCTEPath = ".\\sis_ws\\data_repo\\octe\\"+ this-> userID +"ToDo.txt";
        std::ofstream stuOctefile(stuOCTEPath);
        if (!stuOctefile.is_open()) {
            return 2;
        }else {
            for (int i = 0; i < ProcessCls.size() ; i++) {
                stuOctefile << ProcessCls[i] << "\n";
            }
            stuOctefile.close();
        }
        return 1;
    }
}

//函数--查看octe
//返回值:0:输入的OCTE班级并非本学期在修班级
//1:填写成功
//2：打不开OCTE题目
//3：打不开已填OCTE的班级代码文件
//4：已填写该班级OCTE，无需重复填写
int Student::fill_octe(string classCode) {
    int clsCode = stoi(classCode);
    //cout << classCode << endl;

    //要做OCTE的班级放进数组里
    std::string stuOCTEPath = ".\\sis_ws\\data_repo\\octe\\"+ this-> userID +"ToDo.txt";
    std::ifstream stuOctefile(stuOCTEPath);
    vector<int> octeCls;
    std::string line;
    while (getline(stuOctefile, line)) {
        //cout<< line << endl;
        octeCls.push_back(stoi(line));
    }
    //cout<< octeCls[0] << endl;

    int searchLine = -1;
    for (int i = 0; i < octeCls.size(); i++) {
        if (octeCls[i] == clsCode) {
            searchLine = i + 1 ;
        }
    }

    //不等于-1表示班级在在修列表中
    if (searchLine == -1) {
        return 0;//0:输入的OCTE班级并非本学期在修班级
    }else {
        std::string DoneOCTEPath =  ".\\sis_ws\\data_repo\\octe\\" + this -> userID +"Done.txt";
        std::ifstream DoneOctefile(DoneOCTEPath);
        if (!DoneOctefile.is_open()) {
            return 3;//3：打不开已填OCTE的班级代码文件
        }else {
            string li;
            vector<int> DoneCls;
            while (getline(DoneOctefile, li)) {
                DoneCls.push_back(stoi(li));
                for (int j = 0; j < DoneCls.size(); j++) {
                    if (DoneCls[j] == clsCode) {
                        return 4;//4：已填写该班级OCTE，无需重复填写
                    }else{
                        std::string OCTEPath = ".\\sis_ws\\data_repo\\octe\\exampleOCTE.txt";
                        std::ifstream Octefile(OCTEPath);
                        if (!Octefile.is_open()) {
                            return 2;//2：打不开OCTE题目
                        }else {
                            string lin;
                            while (getline(Octefile, lin)) {
                                cout << lin << endl;
                            }
                            Octefile.close();

                            string ans;
                            getline(cin,ans);
                            std::string ClsOCTEPath = ".\\sis_ws\\data_repo\\octe\\" + classCode + "\\"+ this -> userID +".txt";
                            std::ofstream ClsOctefile(ClsOCTEPath);
                            if (!ClsOctefile.is_open()) {
                                return 5; //无法打开存储评价的文件
                            }else {
                                ClsOctefile << ans;
                                ClsOctefile.close();

                                std::string DoneOCTEPath = ".\\sis_ws\\data_repo\\octe\\" + this -> userID +"Done.txt";
                                std::ifstream DoneOctefile(DoneOCTEPath);
                                if (!DoneOctefile.is_open()) {
                                    return 3;
                                }else {
                                    DoneOctefile.close();
                                    fstream f;
                                    //追加写入,在原来基础上加了ios::app
                                    f.open(DoneOCTEPath,ios::out|ios::app);
                                    f<<classCode<<endl;
                                    f.close();
                                    return 1;//成功填写OCTE并保存
                                }
                            }
                        }
                    }
                }
            }
            DoneOctefile.close();

            cout<< "nothing's in DoneOctefile\n";

            std::string OCTEPath = ".\\sis_ws\\data_repo\\octe\\exampleOCTE.txt";
            std::ifstream Octefile(OCTEPath);
            if (!Octefile.is_open()) {
                return 2;//2：打不开OCTE题目
            }else {
                string lin;
                while (getline(Octefile, lin)) {
                    cout << lin << endl;
                }
                Octefile.close();

                string ans;
                getline(cin,ans);
                std::string ClsOCTEPath = ".\\sis_ws\\data_repo\\octe\\" + classCode + "\\"+ this -> userID +".txt";
                std::ofstream ClsOctefile(ClsOCTEPath);
                if (!ClsOctefile.is_open()) {
                    return 5; //无法打开存储评价的文件
                }else {
                    ClsOctefile << ans;
                    ClsOctefile.close();

                    std::string DoneOCTEPath = ".\\sis_ws\\data_repo\\octe\\" + this -> userID +"Done.txt";
                    std::ifstream DoneOctefile(DoneOCTEPath);
                    if (!DoneOctefile.is_open()) {
                        return 3;
                    }else {
                        DoneOctefile.close();
                        fstream f;
                        //追加写入,在原来基础上加了ios::app
                        f.open(DoneOCTEPath,ios::out|ios::app);
                        f<<classCode<<endl;
                        f.close();
                        return 1;//成功填写OCTE并保存
                    }
                }
            }
        }
    }
    // return 0;
}
