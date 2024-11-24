#include"student.hpp"

// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<iostream>

// sis classes
#include"client.hpp"

// Definition of const members
const std::string Student::student_path = ".\\sis_ws\\data_repo\\student\\";
const std::string Student::stuFrd_path= ".\\sis_ws\\data_repo\\Addfrds\\";
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


Student::~Student() {
    std::cout << "Student destructor" << std::endl;
}

// 添加好友
void Student::addFrd(const std::string &friendID) {

    // 学生信息文件路径
    std::string friendFilePath = ".\\sis_ws\\data_repo\\student\\" + friendID + ".txt";
    std::ifstream friendFile(friendFilePath);

    // 检查是否存在好友的个人档案文件
    if (friendFile.is_open()) {
        // 如果好友文件存在，关闭文件流
        friendFile.close();

        // 输出申请发送的提示
        std::cout << "已发送申请，请等待对方通过。" << std::endl;

        // 定义申请好友文件的路径
        std::string addFriendFilePath = ".\\sis_ws\\data_repo\\AddFrds\\" + friendID + ".txt";

        // 在 AddFrds 文件夹中创建以目标好友学号命名的文件，填入当前用户的id
        std::ofstream addFriendFile(addFriendFilePath);
        if (addFriendFile.is_open()) {
            // 写入当前用户的 ID
            addFriendFile << this->userID << std::endl;
            addFriendFile.close();
        } else {
            std::cout << "错误：无法创建申请文件。" << std::endl;
        }
    } else {
        // 如果好友文件不存在，输出提示
        std::cout << "未找到该好友的学号，请检查输入。" << std::endl;
    }
}

//通过好友申请
void Student::acceptFrd() {
    // 定义当前用户的好友申请文件路径
    std::string addFrdFilePath = ".\\sis_ws\\data_repo\\AddFrds\\" + this->userID + ".txt";
    std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd\\" + this->userID + ".txt";
    // 打开好友申请文件
    std::ifstream inFile(addFrdFilePath);

    // 文件不存在
    if (!inFile.is_open()) {
        std::cout << "没有待通过的好友。" << std::endl;
        return;
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

        // 如果文件内容为空
        if (pendingFriends.empty()) {
            std::cout << "没有待通过的好友。" << std::endl;
            return;
        }

        // 处理每个好友申请
        std::vector<std::string> remainingFriends;  // 保存未处理的好友申请
        for (const std::string &friendID : pendingFriends) {
            std::cout << "是否同意同学 " << friendID << " 的好友申请？1是，0否（其余输入为保留申请）" << std::endl;

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
                    std::cout << "错误：好友文件无法写入。" << std::endl;
                }

                std::string FrdAccPath = ".\\sis_ws\\data_repo\\frd" + friendID + ".txt";
                std::ofstream FrdAccFile(FrdAccPath);
                if (FrdAccFile.is_open()) {
                    // 写入当前用户的 ID
                    FrdAccFile << friendID << std::endl;
                    FrdAccFile.close();
                } else {
                    std::cout << "错误：好友文件无法写入。" << std::endl;
                }

                std::cout << "已同意 " << friendID << " 的好友申请。" << std::endl;
            } else if (userChoice == 0) {
                // 用户拒绝好友申请
                std::cout << "已拒绝 " << friendID << " 的好友申请。" << std::endl;
            } else {
                std::cout << "输入无效，已保留" << friendID << " 的好友申请。" << std::endl;
                remainingFriends.push_back(friendID);
            }
        }

        // 将未处理的好友申请写回文件
        std::ofstream outFile(addFrdFilePath, std::ios::trunc);  // 覆盖原文件
        if (outFile.is_open()) {
            for (const std::string &friendID : remainingFriends) {
                outFile << friendID << std::endl;
            }
            outFile.close();
        } else {
            std::cout << "错误：无法更新好友申请文件。" << std::endl;
        }
    }


}
//查看好友列表
void Student::checkFrd() {
    std::string FrdFilePath = ".\\sis_ws\\data_repo\\frd\\" + this->userID + ".txt";
    std::ifstream in(FrdFilePath);
    std::string line;
    if (in.is_open()) {
        while (std::getline(in, line)) {
            cout << line << endl;
            }
        in.close();
    }
    else{
        std::cout << "无法查看好友列表文件。" << std::endl;
        // [todo]创建用户时就在文件夹创建文件
    }
}