#include"student.hpp"

// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<iostream>
#include<sstream>

// sis classes
#include <iomanip>

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


Student::~Student() {
    std::cout << "*Student destructor" << std::endl;
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

//函数--加入购物车
void Student::addToShoppingCart(std::string class_number) {
    std::string classSrhPath = ".\\sis_ws\\data_repo\\class\\"+class_number+ ".txt";
    std::ifstream inclassfile(classSrhPath);
    if (inclassfile.is_open()) {
        shoppingCart.push_back(class_number);
    }else{
        cout<< "输入的班级代码不存在" <<endl;
    }
};

//函数--检查先修与时间后向教务处提交选课
//shoppingCart班级代码-》课程代码-》获取先修课（先修课=enrolled=.\data_repo\student\(ID.txt)）
//shoppingCart班级代码-》上课时间
//打开文件夹看先修课，7*7的表格检查时间（注册时0改1）返回成功(加入enrolled)/失败
//这个函数正在写，改完search函数就接着写这个

/*
  **辅助函数：读取txt指定行数据存入string
  */
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

//函数--查看公告与成绩

//函数--加课

//函数--退课

