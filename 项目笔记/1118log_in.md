# 1118 Log in 功能说明

- 最新更新时间：1118; 更新者：滕艺萌
- 简介：如何实现Student和Staff的log-in
- 有 ?? 的部分为待办事项； !! 为重点关注。可以Ctrl-F搜索。

## 过程

1. 先调用`Client::id_exist(std::string & inpudID)`检查id是否存在
   - 若不存在则要求重新输入id
2. 如果存在，调用`Client::password_match(std::string & inputID, std::string & inputPassword)`检查密码是否匹配
   - 若不匹配则再次要求输入密码
3. 如果匹配，调用`Client::is_student(std::string & inputID)`检查是学生还是教师
4. 创建学生或者教师对象的shared_pointer，调用`Student::find_profile(std::string & inputID)`或者`Staff::find_profile(std::string & inputID)`给该对象赋值。这一步要严谨的话也可以检查返回的是不是空指针。
5. 接下来就能在主函数中访问对应的对象了。

### 测试例子（Terminal)

1. test 1: Student log in
```powershell
C:\Languages\C_Code\sis_versions\sis1115v1\CSC3002_SIS\cmake-build-debug\CSC3002_SIS.exe
Please enter your ID:
1230000
Sorry, the ID 1230000 cannot be found.
Please enter your ID again:
1230001
Please enter your passcode:
GangeNB!
Sorry, the passcode is incorrect.
Please enter your passcode again:
GanGe666
Welcome, student Linyong Gan!

Process finished with exit code 0
```

2. test 2: Staff log in
```powershell
C:\Languages\C_Code\sis_versions\sis1115v1\CSC3002_SIS\cmake-build-debug\CSC3002_SIS.exe
Please enter your ID:
9100001
Please enter your passcode:
attention_is
Welcome, professor Ruoyu SUN!

Process finished with exit code 0

```


### 测试代码

```c++
// staffTest.cpp

int main() {

    // the following code implement a log_in procedure.
    // ?? For reference.

    std::string inputID;
    std::string inputPass;

    // TODO: command line io should be replaced with GUI.
    printf("Please enter your ID: \n");
    std::cin >> inputID;
    while (!Client::id_exist(inputID)) {
        printf("Sorry, the ID %s cannot be found.\n",inputID.c_str());
        printf("Please enter your ID again: \n");
        std::cin >> inputID;
    }
    printf("Please enter your passcode: \n");
    std::cin >> inputPass;
    while (!Client::validate_passcode(inputID, inputPass)) {
        printf("Sorry, the passcode is incorrect.\n");
        printf("Please enter your passcode again: \n");
        std::cin >> inputPass;
    }

    bool is_stu = Client::id_is_student(inputID);
    if (is_stu) {
        std::shared_ptr<Student> student = Student::find_profile(inputID);
        printf("Welcome, student %s!\n",student->get_userName().c_str());
    }else {
        std::shared_ptr<Staff> staff = Staff::find_profile(inputID);
        printf("Welcome, professor %s!\n",staff->get_userName().c_str());
    }

    

}


```

## 下一步工作计划

- `claim_course`
- `register`