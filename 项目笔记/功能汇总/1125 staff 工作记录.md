# 11/25 staff 工作记录

## todo list
已完成

1. 修复testStaff.cpp中的bug
2. 所有改成const & std::string
3. 姓名处理；通过姓名查找ID

---

3. 新功能

   1. claim course
   2. 审批课程
   3. Client注册（包括staff student); 注册加到花名册里
      - 花名册的排序（在特定位置插入）
      - 检查名称是否合法
      - 自动分配学号
4. extra features
   1. **(重要亮点)** 生成课程表
   2. topological sort检查先修课

## 处理之前的遗留问题

1. 加课
   1. 更改文件名，只有数字
   2. 加入到todolist里
   3. add course成功，加入教授的课程列表，个人档案
   4. add course失败，加入教授的信息列表

---

# "View Response"

1. 