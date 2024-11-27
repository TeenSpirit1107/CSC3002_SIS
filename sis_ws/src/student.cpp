#include"student.hpp"

// cpp lib
#include<string>
#include<fstream>
#include<memory>
#include<iostream>
#include <sstream>
#include <ctime>
#include<vector>
#include <string>
#include<direct.h>

// sis classes
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
                    return 2;
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

//返回指针--搜索课程代码 （1127下午说交给ui的同学做，已删除，保留注释）
// vector<shared_ptr<Course>> Student::get_course(std::string &inputID) {
//
//     std::string courseListPath = course_path + inputID + "_class_arrange.txt";
//     std::ifstream courseFile(courseListPath);
//     std::string li;
//
//     // check whether the id exists
//     if(!courseFile.is_open()) {// case 1, id doesn't exist
//         return std::vector<shared_ptr<Course>>();
//     }
//     //从文件获取班级代码
//     vector<short> classID;
//     while (std::getline(courseFile, li)) {
//         if (!li.empty()) {
//             classID.push_back(stoi(li));
//             int q = classID.size();
//             std::vector<shared_ptr<Course>> ptrVec(q);
//             for (int i = 0; i < q; i++) {
//                 shared_ptr<Course> course_pctr=make_shared<Course>(classID[i]);
//                 ptrVec[i]=course_pctr;
//                 return ptrVec;
//             }
//         }else {
//             return std::vector<shared_ptr<Course>>();
//         }
//     }
//
// }


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
                            std::string toDoPath = ".\\sis_ws\\data_repo\\course_drop\\regToDo.txt";
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

//查看教务处审批加退课的todo.txt


//函数--查看公告(ui已融合)


