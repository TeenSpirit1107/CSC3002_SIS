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

## testStaff.cpp 新的测试功能

1. 测试姓名是否合法；若合法转化为符合格式的姓名