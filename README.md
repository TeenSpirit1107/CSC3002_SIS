# CSC3002: sis_ws
## GUI
- src/guitest3.cpp 为登录界面的测试代码，可单独编译。仅绘制界面及控件，还未设计登录系统。编译需先安装EasyX图形库，本人环境为clion+minGW。


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
