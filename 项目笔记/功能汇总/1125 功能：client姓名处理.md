# 功能: Client姓名处理

- 更新日期：11/25；更新者：Staff

## 函数

- Client类中的public成员函数：
    ```c++
    static bool is_legal_name(const std::string &inputName);
    static std::string format_name(const std::string & inputName);
    static std::string name_find_ID(const std::string & inputName);
    ```
- 检测姓名是否合法(legal)；若合法，生成符合格式的姓名(formatted). 功能详见注释。
    - 用于这些功能：
        - 通过姓名查找id
        - 注册新用户

## 测试

- 测试函数：
    ```c++
  // testStaff.cpp
    void test_name_process();  
    void test_name_find_ID();
    ```
- 期望的输出：

1. test_name_process()
```powershell
C:\Languages\C_Code\sis_versions\sis1125v2_myBranchZip\CSC3002_SIS-staff-1120\cmake-build-debug\CSC3002_SIS.exe
unformatted name: yImEnG tENG
formatted name: Yimeng TENG

Process finished with exit code 0
```
2. test_name_find_ID()
```powershell
C:\Languages\C_Code\sis_versions\sis1125v2_myBranchZip\CSC3002_SIS-staff-1120\cmake-build-debug\CSC3002_SIS.exe
illegal name test passed.
non-existing name test passed.
Student Name: yImeng tenG; Student ID: 1230002
Staff Name: Yangsheng XU; Staff ID: 9200001

Process finished with exit code 0
```
