# 1126 功能：教务处通过class_claim, 教授档案和对象更新

## 功能描述

- 教务处通过class_claim的时候，用这个函数更新教授档案中的class；当前的教授对象对应的成员变量也更新；[TODO] 课程作业文件夹、课程临时成绩文件夹被创建。

test_profile_add_class()


## 测试 staffTest.cpp

- 函数：`void test_profile_add_class();`
- 使用前后记得更改教授的档案文件。
- 期望输出如下：

1. 情况1：教授教其他的这个course的班
   - 举例：xys教授，档案原先是：
    ```text
    CUHKsz
    Yangsheng
    XU
    3
    STA2001
    1
    5
    CSC3002
    1
    1
    MAT1001
    1
    6
    ```
   - 现在加入课程代号为4的STA2001班 `xys.add_class(4)`
   - 更改后，档案为：
   ```text
    CUHKsz
    Yangsheng
    XU
    3
    STA2001
    2
    4
    5
    CSC3002
    1
    1
    MAT1001
    1
    6
    ```

2. 情况2：教授不教其他的这个course的班
    - 举例：xys教授，档案原先是：
    ```text
    CUHKsz
    Yangsheng
    XU
    3
    STA2001
    2
    4
    5
    CSC3002
    1
    1
    ```
    - 现在加入课程代号为6的MAT1001班， `xys.add_class(6)`
    - 更改后，档案为：
   ```text
    CUHKsz
    Yangsheng
    XU
    3
    STA2001
    2
    4
    5
    CSC3002
    1
    1
    MAT1001
    1
    6
    ```