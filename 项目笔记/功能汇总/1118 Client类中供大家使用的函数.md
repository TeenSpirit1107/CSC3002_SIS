# 1118 Client中供大家使用的函数

- 最新更新时间：1118；更新者：滕艺萌
- 简介: 一些函数已经写好，别的类可以直接调用，不需要自己写了。

```c++

// Client.hpp
    bool is_student() const; // 非静态，对对象调用
    static bool id_is_student(std::string & inputID); // 静态，有id即可调用
    static bool id_exist(std::string & inputID); // 静态，检查ID是否存在
    static bool id_exist(std::string & inputID);
    static bool validate_passcode(std::string & inputID, std::string & inputPass);
    
// Staff.hpp
shared_ptr<Staff> Staff::find_profile(std::string & inputID);

// Student.hpp
shared_ptr<Student> Student::find_profile(std::string & inputID);
  
```
