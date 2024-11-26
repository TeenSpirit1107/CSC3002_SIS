# CPP File Input Output

## Basics

- Date: 2024/11/13
- Motivation: CSC3002 Project
  - Learn some basic operations about file io.
- Notation
  - ?? : to-do list

## Reference

1. 打开文件、读取文件：[cpp打开文件方式一览表，C语言中文网](https://c.biancheng.net/view/294.html)
    - 表格里有很多有用更多东西。
2. 写入文件，尤其是在特定的某一行进行：删除、插入、更改，等操作。
    - ?? 我还没开始学这个。

## My Code 1: ifstream

- a test, covering task 1-4.

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include "stdio.h"
#include <filesystem>

int main() {
    std::ifstream file;

    // Version 1:
    file.open("..\\test_data_repo\\text.txt");
    // Version 2, with slash:
    // file.open("../test_data_repo/text.txt");
    
    // Version 1:
    if (!file.is_open()) {
    // Version 2: (but I don't prefer this)s
    // if(flie){}

        printf("Sorry, the file doesn't exist.\n");
    }else {
        printf("Printing the file:");
        std::string line;
        int count = 0;
        while (std::getline(file, line)) {
            count++;
            printf("%d %s\n",count,line.c_str());
        }
        file.close();
    }
}

```

### Finding Path

- for printing the working directory: `std::filesystem::current_path()`
  - 这一点可能是比较重要的，因为我print了之后才发现原来我的working directory并不是该cpp文件所在的directory, 而是：`"C:\\Users\\Lisa\\CLionProjects\\testProj\\cmake-build-debug"`
  - 如果需要回到上一级目录，可以用`..`
- slash and backslash: both works; 但是斜杠需要转义字符。也就是说有以下两种写法：
  - /
  - \\
  - Example:

    ```cpp
    // Version 1:
    file.open("..\\test_data_repo\\text.txt");
    // Version 2, with slash:
    // file.open("../test_data_repo/text.txt");
    ```

- Output:
  
    ```powershell
    C:\Users\Lisa\CLionProjects\testProj\cmake-build-debug\testProj.exe
    Printing the file:1 good
    2 work
    File ends.
    Process finished with exit code 0
    ```

### Check File Open

- 2 methods:
  1. `file.is_open()` . I prefer this. This function is designed to do so.
  2. `if(file)`. Same result but I think it's a bit more dangerous to use.

- Example:
  
    ```cpp
    // Version 1:
    if (!file.is_open()) {
    // Version 2: (but I don't prefer this)s
    // if(flie){}
    ```

### Close

- If there's no `file.close()`, the file will still be automatically closed. Nevertheless it's a good habit to write `file.close()`.

### Whether automatically create file, if not existing

- here, we use `ifstream`, which corresponds to `ios::in`, when the file doesn't exist, it doesn't create the file.
- if we use `ofstream`, `ios::out`, it will create the file if it doesn't exist yet. Refer to the next testing code.

## My Code 2: fstream

- 我觉得这个最好用，因为可以写入也可以写出。

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include "stdio.h"
#include <filesystem>

int main() {

    std::fstream file;

    // mode 1: in|out.
    file.open("..\\test_data_repo\\test.txt",std::ios::in|std::ios::out);
    // mode 2: out

    if (!file.is_open()) {
        printf("Sorry, the file doesn't exist.\n");
    }else {
        printf("Printing the file:\n");
        std::string line;
        int count = 0;
        while (std::getline(file, line)) {
            count++;
            printf("%d %s\n",count,line.c_str());
        }
        file.close();
    }
}
```

### fstream适用的mode

- 以下总结我觉得常用的几个：
  1. `ios::in|ios::out`。 当找不到文件的时候，**不会**创造新文件（对应到应用中，是用户输错了id的时候）；当找到文件的时候，也不会重写文件。
  2. `ios::out`。可以用来创建新文件。

### 更改文件的两种做法

- 没有函数可以直接更改文件。所以有以下俩种做法：
1. 存在vector<string>里，例如staff.profile_add_class(short class_code)中的写法
2. 用一个temp文件，把原文件的内容读入，然后更改，最后再写入原文件。代码和更改前后文件如下。
例子：
```c++
void test_rewrite_file() {
std::string work_dir = "sis_ws/src/testFile.txt";
std::ifstream fileReader(work_dir);
std::ofstream tempWriter("temp.txt");
std::string line;

    while (std::getline(fileReader, line)) {
        if (line == "change") {
            tempWriter << "hello1" << std::endl;
        } else {
            tempWriter << line << std::endl;
            if (line == "hi") {
                tempWriter << "hello2" << std::endl;
            }
        }
    }

    fileReader.close();
    tempWriter.close();
    std::remove(work_dir.c_str());
    std::rename("temp.txt", work_dir.c_str());
}
```

- testFile.txt before
```text
empty
change
hi
world
```

- testFile.txt after
```text
empty
hello1
hi
hello2
world

```