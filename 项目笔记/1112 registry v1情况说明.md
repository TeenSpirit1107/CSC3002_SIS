# 11.12 registry v1 情况说明

registry v1已经完成proposal中registry-a功能，即增设课程。相对本功能联系单（10.21），有以下修改：

1. 工号(userID)已被修改为string
2. 新增了文件".\data_repo\course\Course Number.txt"和".\data_repo\course\Course List.txt"，存储课程总数和名单，方便访问不同课程代号对应的文件。class也有相同的设计。
3. 原本计划将课程安排记录在“.\data_repo\course\课程代号.txt”下，现将其单独记录在同目录的"课程代号_class_arrange.txt"内

以下内容需要大家同步或协作或运行我的代码时需要注意的：

1. 需要设置工作目录到CSC3002_SIS
2. [to tym] 需要一个通过教授ID获取姓名的函数
3. [to tym] 在教授申报class成功后需要将此加入其个人信息页，这个文件（目前是".\data_repo\staff\教授编号.txt"下）应该怎么构成？我目前是直接把新设立的班级编号加在文档最后，但可能还需要根据你的要求调整
4. [to zxy] 我还不确定下一个功能做c还是d，如果是d(处理学生的加课申请)可能需要和你协商合作单，你可完成后告知我。
5. [to zxy] 注意到后面的dean list和publish gpa对我其实就是类似于发布公告，而stu client可能其他压力较大，要不等你基本框架搭起来后这部分的学生端我来写？版本控制应该不是大问题，我们应该不会同时写，而且是函数封装，直接拷贝也可以
6. [to all, especially to ls and lyx] 需要一个接受announcement的模块，或者说这部分数据库应该怎么处理？我目前会把申报成功后提醒教授的内容放在对应的claim/staff文件夹下，但是不是需要单独弄一个announcement的文件方便每一个用户登录时被提醒
7. [to all] b功能为自动生成课程代号，目前我对此还没有很清晰的想法。我不太清楚拓扑应该如何解决这一问题，恳请大家能给点思路
