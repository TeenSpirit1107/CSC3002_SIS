# CSC3002: sis_ws
## GUI
- src/guitest3.cpp 为登录界面的测试代码，绘制了界面及创建控件，可单独编译。
- ![image](https://github.com/user-attachments/assets/d8a9d61a-72a8-4089-9777-00dd0f4ab55d)
- 编译需先安装EasyX图形库，本人环境为clion+minGW。clion配置EasyX：https://blog.csdn.net/qq_43556317/article/details/126253455 记得确定安装的minGW是clion中采用的那个minGW
- Win11下编译好的程序脱离环境运行可能会出现键盘输入无效的问题，需要调整系统设置：https://blog.csdn.net/2301_76302542/article/details/135176309
- 文本框已实现输入功能，如clion中运行无效，需要在Edit Configuration中将Run in external console打钩 ![image](https://github.com/user-attachments/assets/79cc0478-ec03-43df-a970-b7d0d80566d0)
- 已知问题：Clion+cmke3.29编译后的exe单独运行可能会缺库。vscode+cmake3.29编译则正常，可以单exe运行



## 继承
- Client类是基类，Registry, Staff, Student都是它的衍生类。
- Course类会在Registry, Staff, Studetn ... 等类中被用到，但它和它们没有继承关系。
```
Client
|
--Registry
|
--Staff
|
--Student
```

## 项目结构
- include用来放声明文件。
- src用来放定义文件，以及运行文件（`startup.cpp`，没有对应的.hpp文件）
- data_repo用来存储数据。有点类似于服务器。
      - 也许data_repo里可以有个文档存储当前系统信息。
    - 待办：实际上我们可以随意打开data_repo, 只是当运行程序的时候不一定能用某个程序打开，如果没有权限的话。如此是否不够严谨？

## 示例文件
- 以下文件已经写好一些完整的函数，作为示例，展示：分离式编译、继承关系、ADT作为成员变量。
    1. client.hpp
    2. client.cpp
    3. student.hpp
    4. student.cpp
    5. course.hpp
    6. course.cpp

## 团队合作编程习惯
- 给自己的代码写注释；写.md文档来描述代码的功能
- 上传到github的时候先创建分支branch，再pull request. 每次更新时最好标注这次更新了什么内容。

## 
- **和大家一组很幸运！love you guys <3**
