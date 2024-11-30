// cpp lib
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<memory>
#include<vector>
#include<tuple>
#include<algorithm>
#include<cmath>

// sis lib
#include "staff.hpp"
#include"course.hpp"


const std::string Staff::staff_path = ".\\sis_ws\\data_repo\\staff\\";


/**
* @brief Constructs a Staff object with the given input ID.
*
* This constructor initializes a Staff object by reading the staff profile
* from a file. If the file cannot be opened, the userID, userName, and passcode
* are set to "ERROR". But it's assumed that, before calling this constructor,
* the existence of this ID must have been verified.
*
* @param inputID The ID of the staff member. Guaranteed to exist.
*/

Staff::Staff(const std::string &inputID): Client(inputID) {
    profile_path = staff_path + inputID + ".txt";
    ifstream fileReader(profile_path);

    if (!fileReader.is_open()) {
        printf("Sorry, an error was encountered. Please try again.");
        userID = "ERROR";
        userName = "ERROR";
        passcode = "ERROR";
        // [todo] add a detecting mechanism where this is called, to check whether userID is "ERROR". However, I don't think it's required?
    } else {
        std::string psc;
        std::string name1;
        std::string name2;

        fileReader >> psc;
        fileReader >> name1;
        fileReader >> name2;

        passcode = psc;
        userName = name1 + " " + name2;

        int course_num = 0;
        fileReader >> course_num;


        // 1. Store to class and course

        for (int i = 0; i < course_num; i++) {
            std::string course_code;
            fileReader >> course_code;
            courses[course_code] = std::vector<short>();
            std::vector<short> *v_ptr = &(courses[course_code]);
            int class_num = 0;
            fileReader >> class_num;
            for (int j = 0; j < class_num; j++) {
                std::string cls_str;
                short class_code;
                fileReader >> class_code;
                v_ptr->push_back(class_code);
                classes.insert(class_code);
            }
            v_ptr = nullptr;
        }
    }
}


/**
 * @brief Finds the profile of a staff member by their ID.
 *
 * This function checks if a staff member with the given ID exists. If the ID exists,
 * it creates a new Staff object on the heap and returns a shared pointer to it.
 * If the ID does not exist, it returns a nullptr.
 *
 * @param inputID The ID of the staff member to find.
 * @return shared_ptr<Staff> A shared pointer to the Staff object if found, otherwise nullptr.
 */
shared_ptr<Staff> Staff::find_profile(const std::string &inputID) {
    std::string find_path = staff_path + inputID + ".txt";

    // check whether the id exists
    if (!Client::id_exist(inputID)) {
        // case 1, id doesn't exist
        return nullptr;
    }
    // case 2, id does exists. Then create a new object in HEAP and return its pointer.
    shared_ptr<Staff> staff_ptr = std::make_shared<Staff>(inputID);
    return staff_ptr;
}

// Feature 1: Create Course


// TODO: 流程。先验证输入，如果不对就提示用户重新输入；对就使用下面这个函数。
// TODO: year这里，用户界面做成“多选”的那种形式？
// TODO: finish comment on this function. return error code.

/**
 * @brief Create a course with the given information.
 *
 * First, validate expression. If valid, then, create the corresponding file.
 * Return code explanation:
 * 0 successfully created and written into file
 * 1 the input requisites expression is invalid -- should ask the user to re-enter.
 * 2 the file cannot be written into (unknown error; or a file with the same name exists)
 * 3 if there's error in updating todo.txt file
 * @param course_name The name of the course.
 * @param pre_req The expression of prerequisites of the course. Not assumed to be valid.
 * @param year The year of the course.
 * @param description The description of the course.
 * @return status code
 */
int Staff::create_course(const std::string &course_name, const std::string &pre_req, const std::string &year,
                         const std::string &description) {
    if (!is_valid_course_expr(pre_req)) return 1;
    std::string file_name = get_current_datetime() + ".txt";
    std::string file_path = course_claim_path_prefix + "registry\\" + file_name;
    std::string index_dir = ".\\sis_ws\\data_repo\\course_claim\\registry\\to_claim_list.txt";
    std::ofstream os(file_path, std::ios::out);

    //std::ios::in means a new file will not be created if a file with the same name exists.
    if (!os.is_open()) {
        return 2;
    }
    if (update_index_file(index_dir, file_name) != 0) {
        return 3;
    }

    os << course_name << std::endl;
    os << this->userID << std::endl;
    if (pre_req == "") {
        os << "None" << std::endl;
    } else {
        os << pre_req << std::endl;
    }
    os << year << std::endl;
    os << description << std::endl;
    return 0;
}

// TODO: integer return code indicates normal or error.

/**
 * @brief Computes the final grade for a class.
 *
 * This function reads the scores of students from a file, calculates their average scores,
 * sorts them, and assigns letter grades based on their ranking.
 *
 * Return code explanation:
 * 0 - The final grades were computed and written successfully.
 * 1 - The class code does not exist (file cannot be opened).
 * 2 - The output file cannot be written into (unknown error).
 * 3 - The number of students in the file does not match the expected number.
 *
 * @param class_code The code of the class for which to compute final grades.
 * @return int 0 if successful, 1 if the class code does not exist, 2 if the output file cannot be written, 3 if the number of students does not match.
 */

int Staff::compute_final_grade(short class_code) {

    int SCORE_NUM = 3;
    string str_class_code = std::to_string(class_code);
    std::string input_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\" + str_class_code + ".txt";
    std::ifstream fileReader = std::ifstream(input_dir);
    if (!fileReader.is_open()) {
        return 1; // indicates the class code doesn't exist
    }

    // TODO: when the class profile is re-structure, staff shall read this value from the file.
    // here for testing I just made up a number.
    int enrolled_num = 4;
    if (class_code == 2) {
        enrolled_num = 9;
    }
    // don't need enrolled stu ID?
    // testing part ends

    // student ID, average grade of the student
    vector<std::tuple<int, double> > id_avg; // student ID, average grade
    std::string line;
    int arr_ind = 0;
    while (std::getline(fileReader, line)) {
        std::istringstream iss(line);
        int student_id;
        double score1, score2, score3;
        iss >> student_id >> score1 >> score2 >> score3;
        score1 = max(0.0,score1);
        score2 = max(0.0,score2);
        score3 = max(0.0,score3);
        double average = (score1 + score2 + score3) / 3.0;
        id_avg.push_back(std::make_tuple(student_id, average));
        arr_ind++;
    }
    fileReader.close();

    if (arr_ind != enrolled_num) {
        return 3; // indicates the number of students in the file doesn't match the number of students in the class.
        // should check our data repo. not supposed to happen because of client's operations, but bc of the error in the code.
    }

    std::sort(id_avg.begin(), id_avg.end(), [](const std::tuple<int, double> &a, const std::tuple<int, double> &b) {
        return std::get<1>(a) > std::get<1>(b);
    });


    // TODO: change this to ...
    // grade: ABCDE, 5 even partitioning
    // test
    int PARTITION_NUM = 5;
    char LETTER_GRADE[PARTITION_NUM] = {'a', 'b', 'd', 'g', 'l'};

    std::string output_dir = ".\\sis_ws\\data_repo\\student_grade\\" + str_class_code + ".txt";

    ofstream fileWriter = ofstream(output_dir);
    if (!fileWriter.is_open()) {
        return 2; // indicates the file cannot be written into. Unkown error.
    }

    for (int i = 0; i < enrolled_num; ++i) {
        std::string id_str = std::to_string(std::get<0>(id_avg[i]));
        int grade_ind = std::floor(PARTITION_NUM * i / enrolled_num);
        char grade = LETTER_GRADE[grade_ind];
        std::string grade_str(1, grade);
        fileWriter << id_str << grade_str << std::endl;
    }
    fileWriter.close();
    return 0;
}

/**
 * @brief Creates a class with the given course code, class code, lecture times, and tutorial times.
 *
 * This function writes the class information to a file in the class claim registry.
 *
 * Return code explanation:
 * 0 - The class was created and written to the file successfully.
 * 1 - The file could not be opened (unknown error).
 * 2 - Error in handling to_claim_list.txt
 * 3 - Class already exists
 *
 * What this function doesn't do: ensure there's no time conflict with the professor's current schedule.
 * This can be done by only allowing professor to select the available time for themselves.
 *
 * @param course_code
 * @param class_code
 * @param input_lec A vector of integers representing lecture time slots, refer to the Communication Notes.
 * @param input_tut A vector of integers representing tutorial time slots, with the same rule.
 * @return status code.
 */
int Staff::claim_class(const std::string &course_code, short class_code, vector<int> input_lec, vector<int> input_tut) {

    std::string file_name = get_current_datetime() + ".txt";
    std::string work_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\" + file_name;
    std::string index_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\to_claim_list.txt";
    std::ofstream os(work_dir, std::ios::out);
    if (!os.is_open()) {
        return 1;
    }
    if (update_index_file(index_dir, file_name) != 0) {
        return 2;
    }
    os << course_code << std::endl;
    os << this->userID << std::endl;
    os << class_code << std::endl;
    os << input_lec.size() << std::endl;
    for (int i = 0; i < input_lec.size(); i++) {
        os << input_lec[i] << std::endl;
    }
    os << input_tut.size() << std::endl;
    for (int i = 0; i < input_tut.size(); i++) {
        os << input_tut[i] << std::endl;
    }

    // validation 1: class doesn't exists yet
    // case 1: class already exists
    std::string testDir = ".\\sis_ws\\data_repo\\class_claim\\registry\\" + class_code;
    std::ifstream testOpen(testDir);
    if (testOpen.is_open()) return 3;

    // case 2: doesn't. can claim.
    return 0;
}

/**
 * @brief Handles the steps to be taken after successfully claiming a class.
 *
 * This function updates the staff profile and object by adding the claimed class code
 * to the staff's list of classes. It also creates a new temporary grade file for the class.
 *
 * @param class_code The code of the class that was successfully claimed.
 */
void Staff::claim_class_succ(const short class_code) {

    // update profile and object
    profile_add_class(class_code);

    // create student_temp_grade
    std::string new_temp_grade = "./sis_ws/data_repo/student_temp_grade/"+std::to_string(class_code)+".txt";
    ofstream fileWriter(new_temp_grade);
    if (!fileWriter.is_open()) printf("error: cannot create new file");// TODO: test
    fileWriter << "0" <<std::endl;
}
/**
 * @brief Updates the staff profile and object after successfully claiming a class.
 *
 * This function updates the staff profile and object by adding the claimed class code
 * to the staff's list of classes. It ensures that the class code is added to the
 * appropriate course in the staff's profile.
 * This is one step among the steps after a class_claim is approved.
 *
 * @param class_code The code of the class that was successfully claimed.
 */
void Staff::profile_add_class( short class_code) {
    // update object
    std::vector<std::string> lines;
    std::string line;
    Course new_class = Course(class_code);
    std::string new_course_code = new_class.courseCode;

    auto it = courses.find(new_course_code);
    bool has_course = (it!=courses.end());
    classes.insert(class_code);
    if (!has_course) courses[new_course_code] = vector<short>();
    auto it2 = courses.find(new_course_code);
    if (it2!=courses.end()) {
        it2->second.push_back(class_code);
    }
    // update file

    // save to temporary vector; do modifications
    
    std::fstream file(profile_path, std::ios::in | std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Error: the file could not be opened." << std::endl;
        return;
    }

    // case 1: doesn't have this course's classes before

    if (!has_course) {
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        lines.at(3)=std::to_string(courses.size());
        lines.push_back(new_course_code);
        lines.push_back("1");
        lines.push_back(std::to_string(class_code));
    }else {
        // case 2: has another course of this class before
        while (std::getline(file, line)) {
            lines.push_back(line);
            if (line==new_course_code) {
                std::getline(file, line);//discard
                lines.push_back(std::to_string(it->second.size()));
                lines.push_back(std::to_string(class_code));
            }
        }
    }

    // from temporary vector to file
    file.clear();
    file.seekp(0, std::ios::beg);
    for (const auto &l : lines) {
        file << l << std::endl;
    }
}
struct node1 {
    string filename;
    node1 *nxt;
    node1 *prv;
    explicit node1(string s);
};

node1::node1(string s): filename(std::move(s)), nxt(nullptr), prv(nullptr) {}

void insert_at_tail(node1 *head, string filename) {
    node1 *new_node = new node1(std::move(filename));
    node1 *p = head;
    while (p->nxt != head) p = p->nxt;
    new_node->nxt = p->nxt;
    new_node->prv = p;
    head->prv = new_node;
    p->nxt = new_node;
}
void print_list(node1 *head) {
    if (head == nullptr) return;
    node1 *p = head;
    bool flag = true;
    while(p != head or flag) {
        printf("%s\n", p->filename.c_str());
        p = p->nxt;
        flag = false;
    }
}
void print_list_rev(node1 *head) {
    if (head == nullptr) return;
    node1 *p = head->prv;
    bool flag = true;
    while(p != head->prv or flag) {
        printf("%s\n", p->filename.c_str());
        p = p->prv;
        flag = false;
    }
}
void list_delete(node1 *&head, node1 *p) {
    if(p == head) head = head->nxt;
    p->nxt->prv = p->prv;
    p->prv->nxt = p->nxt;
    delete p;
}

void normal_addition_next_process_prof(string fn, string stuCode, short classCode, string description, string subpath, bool passed) {
    printf("normal addition next process\n");
    string reason = "";
    if(!passed) {
        cin >> reason;
    }
    string work_dir = ".\\sis_ws\\data_repo\\"+subpath+fn;
    FILE *file = fopen(work_dir.c_str(), "w");
    if(!file) {
        printf("File not found\n");
    } else {
        fprintf(file, "%s\n", stuCode.c_str());
        fprintf(file, "%04d\n", classCode);
        fprintf(file, "%s\n", description.c_str());
        fprintf(file, "%d\n", passed);
        if(!passed) fprintf(file, "%s\n", reason.c_str());
        fclose(file);
    }
    work_dir = ".\\sis_ws\\data_repo\\"+subpath+"\\staff2reg.txt";
    ifstream infile(work_dir);
    int n;
    string filenames[1000];
    if(infile.is_open()) {
        infile >> n;
        string tmp;
        getline(infile, tmp);
        for(int i = 0; i < n; i++) getline(infile, filenames[i]);
        infile.close();
    } else {
        n = 0;
    }
    file = fopen(work_dir.c_str(), "w");
    fprintf(file, "%d\n", n+1);
    for(int i = 0; i < n; i++) {
        fprintf(file, "%s\n", filenames[i].c_str());
    }
    fprintf(file, "%s\n", fn.c_str());
    fclose(file);
}
void Staff::Add_and_Drop(bool addition)
{
    string subpath = "";
    if(addition) subpath = "course_add\\";
    else subpath = "course_drop\\";
    cout << "Dealing Student Enrollments" << endl;
    string work_dir = ".\\sis_ws\\data_repo\\"+subpath;
    string index_dir = work_dir + "stu2staff.txt";
    FILE *file = fopen(index_dir.c_str(), "r");
    if (!file) {
        cout << "could not open file " << index_dir << endl;
    } else {
        int n;
        fscanf(file, "%d", &n);
        printf("%d to be claim\n", n);
        char fn[20];
        fscanf(file, "%s", fn);
        node1 *head = new node1(fn);
        head->prv = head; head->nxt = head;
        for (int i = 1; i < n; i++) {
            fscanf(file, "%s", fn);
            insert_at_tail(head, fn);
        }
        printf("normal addition list\n");
        print_list(head);
        node1 *p = head;
        while(n) {
            string fn = p->filename;
            printf("student %s\n", fn.c_str());
            string stu_path = work_dir + fn;
            ifstream class_file(stu_path);
            if (!class_file.is_open()) {
                cout << "could not open file " << stu_path << endl;
            } else {
                string stuCode, description;
                getline(class_file, stuCode);
                string tmp;
                getline(class_file, tmp);
                short classCode = static_cast<short>(stoi(tmp));
                getline(class_file, description);
                class_file.close();
                printf("student code: %s\n", stuCode.c_str());
                printf("student description: %s\n", description.c_str());
                // get transcript
                Course C = Course(classCode);
                printf("%s: %s\n", C.courseCode.c_str(), C.courseName.c_str());
                printf("Instructor: %s\n", C.instructor.c_str());
                printf("Capacity: %d\n", C.capacity);
                printf("Unit: %d\n", C.unit);
                char c;
                printf("judgement: (P/F/S/B)\n"); // pass fail skip break
                scanf("%c", &c);
                while (c != 'P' && c != 'F' && c != 'S' && c != 'B') scanf("%c", &c);
                if (c == 'P') normal_addition_next_process_prof(fn, stuCode, classCode, description, subpath, true);
                else if (c == 'F') normal_addition_next_process_prof(fn, stuCode, classCode, description, subpath, false);
                else if (c == 'S') {
                    p = p->nxt;
                    continue;
                }
                else if(c == 'B') break;
                printf("%s claimed successfully\n", p->filename.c_str());
                p = p->nxt;
                list_delete(head, p->prv);
                n--;
            }
        }
        fclose(file);
        FILE *file = fopen(index_dir.c_str(), "w");
        if (!file) {
            cout << "could not open file " << index_dir << endl;
        } else {
            fprintf(file, "%d\n", n);
            p = head;
            for (int i = 0; i < n; i++, p = p->nxt) {
                fprintf(file, "%s\n", p->filename.c_str());
            }
            fclose(file);
        }
        printf("all claims are done\n");
    }
}

Staff::~Staff() {
    //[todo] finish destructor
}
