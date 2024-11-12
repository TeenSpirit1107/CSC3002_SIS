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
  * ".\data_repo\course_claim\registry\发起时间.txt"，文件内包含 课程名称 教授工号 授课时间 先修课程代号 年级限制 等。举例如下：

**"20241021201700.txt":**

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
  
  **注意：工号为string**

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
  * ".\data_repo\course\Course Number.txt"
    * 课程代号总数
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

**"CSC3100_class_arrange.txt"**

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
  * ".\data_repo\class_claim\registry\发起时间.txt"

**"20241021210500.txt":**

CSC3002

0000001

2

3

4

4

6

7

8

9

**样例解释**

第一行表示课程代号

第二行为教授的工号

第三行为一个整数n，表示每周lec课时，接下来n行每行一个整数，表示对应的课时

接下来一行为一个整数m，表示每周tut课时，接下来m行每行一个整数，表示对应的课时，课时对应表如下：

|            | Mon  | Tue  | Wed  | Thu  | Fri  | Sat  | Sun  |
| ---------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| 上午第一节 | 1    | 2    | 3    | 4    | 5    | 6    | 7    |
| 上午第二节 | 8    | 9    | 10   | 11   | 12   | 13   | 14   |
| 下午第一节 | 15   | 16   | 17   | 18   | 19   | 20   | 21   |
| 下午第二节 | 22   | 23   | 24   | 25   | 26   | 27   | 28   |
| 晚上第一节 | 1    | 2    | 3    | 4    | 5    | 6    | 7    |
| 晚上第二节 | 8    | 9    | 10   | 11   | 12   | 13   | 14   |

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

  * 修改".\data_repo\course\课程代号_class_arrange.txt"最后的班级个数和编号
  * ".\data_repo\staff\工号.txt"加入班级代号
  * ".\data_repo\class\班级代号.txt"

  "0001.txt":

  0000001

  CSC3100

  2

  3

  17

  4

  8

  9

  15

  16

  **解释**：教授工号、课程代号、上课时间
