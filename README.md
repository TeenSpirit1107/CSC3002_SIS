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

## 命名规范

- 面向对象编程时，成员变量若为私有变量private, 则在变量名后加一个下划线。例如：“Staff::course_”


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

## 合作联系单

### template

**发起时间：**

**发起方：**

**接收方：**

**联系功能简述：**

**数据传输简述：**

### 教授信息表的建议

**发起时间：**Oct 21

**发起方：**Reg

**接收方：**Staff

**联系功能简述：**构建教授信息表

**数据传输简述：**

".\data_repo\staff\工号.txt"，包含教授的姓名和教授班级

**举例：**

"0000001.txt":

Rui

HUANG

0001

0002

**解释：**黄教授教0001和0002两个班级，这两个班级可以在".\data_repo\class\0001.txt"（或"0002.txt"）中了解到具体的信息，如课程代号、授课时间等，具体见1.a.2的联系单

### 1.a.1: 创建课程

**发起时间：**Oct. 21

**发起方：**Reg

**接收方：**Staff

**联系功能简述：**1.a.1: 创建课程

**数据传输与构成简述：**

* Staff --> reg: 
  * ".\data_repo\course_claim\registry\课程名称+发起时间.txt"，文件内包含 课程名称 教授工号 授课时间 先修课程代号 年级限制 等。举例如下：

**"Data Structure_20241021201700.txt":**

Data Structure

0000001

(CSC1001|CSC1003)&MAT1001

3 2 3 4

It's a course for undergraduate students to learn more about data structure. 

**样例解释：**

文件名中，202410212017表示发起时间为2024.10.21 20:17:00

第一行为课程名称，为一个string

第二行为工号，7位数字

接下来一行为先修课表达式，|为或 &为且

接下来一行中第一个数字为可选的年级个数，后面跟若干个数为年级数

接下来一行为对课程的陈述。

**教务处处理完成后删除这个文件。**



* Reg --> staff:

  ".\data_repo\course_claim\staff\教授工号_succ.txt"，采用增加("a+")的方式，将新内容增加在文件最后。举例如下：

**"0000001_succ.txt":**

  CSC3100

Data Structure

  CSC1003

Java Programming

  **样例解释：**0000001号教授，申请了两门课，分别得到课程代号CSC3100和CSC1003。

  用于通知教授成功的课程代号。

  **这个文件永久存档，告知教授成功申请过哪些课程以及其代号**

​	".\data_repo\course_claim\staff\教授工号_fail.txt"，举例如下：

**"0000001_fail.txt"**

In dialogue with humanity

President Xu doesn't like this course. 

Fundamental Physics

A relative course has been set up. 

**样例解释：**申请的课程名称和拒绝理由



* Database:
  * ".\data_repo\course\Course List.txt"
    * 在最后加入审批通过的课程代号
  * ".\data_repo\course\课程代号.txt"
    * 参考staff --> reg，举例如下：

**"CSC3100.txt":**

Data Structure

0000001

(CSC1001|CSC1003)&MAT1001

3 2 3 4

It's a course for undergraduate students to learn more about data structure. 

2

0001

0002

**样例解释：**

前若干行同理，倒数第三行的2表示这个课程一共有2个班级（将在下一张单子中具体说明），班级代号分别为0001和0002。

### 1.a.2 创建班级

**发起时间：**Oct. 21

**发起方：** Reg

**接收方：** Staff

**联系功能简述：**1.a.2 创建班级

**数据传输简述：**

* Staff --> Reg
  * ".\data_repo\class_claim\registry\课程代号+发起时间.txt"


"CSC3002_20241021210500.txt":

0000001

2

1 3

3 3

4

2 1

2 2

3 1

3 2

**样例解释**

第一行表示教授的工号

第二行为一个整数n，表示每周lec课时，接下来n行每行两个整数，分别表示周几的第几节

接下来一行为一个整数m，表示每周tut课时，接下来m行每行两个整数，分别表示周几的第几节

* Reg --> Staff

  * ".\data_repo\class_claim\staff\工号_succ.txt"

  "0000001_succ.txt":

  CSC3002

  0003

  DDA2001

  0005

  * ".\data_repo\class_claim\staff\工号_fail.txt"

  "0000001_fail.txt":

  CSC3100

  Dean doesn't allow you to teach this course. 

* Database

  * 修改".\data_repo\course\课程代号.txt"最后的班级个数和编号
  * ".\data_repo\staff\工号.txt"加入班级代号
  * ".\data_repo\class\班级代号.txt"

  "0001.txt":

  0000001

  CSC3100

  2

  1 3

  3 3

  4

  2 1

  2 2

  3 1

  3 2

  **解释**：教授工号、课程代号、上课时间

- **和大家一组很幸运！love you guys <3**
