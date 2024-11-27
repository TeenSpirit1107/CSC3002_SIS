# 1126 学生、教师存课的方式

| 成员变量声明                                                        | Staff                            | Student                                            |
|---------------------------------------------------------------|----------------------------------|----------------------------------------------------|
| ~~`shared_ptr<Course> schedule[49]`~~                         | ~~学生教授都有；学生可以看朋友的时间表~~ 已取消，详情见下文 |                                                    |
| `unordered_set<short> classess;`                              | 学生教授都有；与schedule相比，能反映课程的真实数目。   |                                                    |
| `std::unordered_map<std::string,std::vector<short>> courses;` | 学生没有                             | 只有教授有，因为只有教授会出现一个course多个class的情况。另外，能反映course的数目。 |

- `shared_ptr<Course> schedule[49]`改为用一个函数随时计算schedule, 而不是在构造器中，否则会有循环定义问题(需要include Course 造成的)，而且即使是forward declaration也解决不了。