#include <iostream>
#include <cstdio>
#include <conio.h>
#include <course.hpp>
#include <fstream>
#include <utility>
#include <windows.h>
#include <iomanip>
#include <map>
#include <set>
#include <vector>

#include <student.hpp>
#include "registry.hpp"
#include "client.hpp"

using namespace std;

Registry::Registry(): Client("0000000") {
    this->userID = "0000000";
    this->userName = "Registry";
    // this->userType = 'a';
    this->passcode = "1234567890";
    printf("Registry Connected\n");
}

Registry::~Registry() {
    printf("disconnecting from registry\n");
}

void course_display(string course_name, string prof_id, string precourse, bool lmt[], string comment) {
    cout << "course_display: course_name: " << course_name << endl;
    cout << "course_display: prof_id: " << prof_id << endl;
    // [todo] get prof's name by id, cooperate with tym
    cout << "course_display: precourse: " << precourse<< endl;
    cout << "course_display: lmt: ";
    for (int i = 0; i < 7; i++) {
        printf("%d ", lmt[i]);
    }
    cout << endl;
    cout << "course_display: comment: " << comment << endl;
}
void pass_process(string course_name, string prof_code, string precourse, int num_lim, bool lmt[], string comment) {
    // [todo] raise an announcement
    string course_code;
    cin >> course_code;
    string work_dir = ".\\sis_ws\\data_repo\\course_claim\\staff\\"+prof_code+"_succ.txt";
    FILE *file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%s\n", course_code.c_str());
    fprintf(file, "%s\n", course_name.c_str());
    fclose(file);
    work_dir = ".\\sis_ws\\data_repo\\course\\";
    file = fopen((work_dir+"Course Number.txt").c_str(), "r");
    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    file = fopen((work_dir+"Course Number.txt").c_str(), "w");
    fprintf(file, "%d\n", n+1);
    fclose(file);

    file = fopen((work_dir+"Course List.txt").c_str(), "a");
    fprintf(file, "%s\n", course_code.c_str());
    fclose(file);

    file = fopen((work_dir+course_code+".txt").c_str(), "w");
    fprintf(file, "%s\n", course_name.c_str());
    fprintf(file, "%s\n", prof_code.c_str());
    fprintf(file, "%s\n", precourse.c_str());
    fprintf(file, "%d ", num_lim);
    for (int i = 0; i < 7; i++) if (lmt[i]) fprintf(file, "%d ", i+1); fprintf(file, "\n");
    fprintf(file, "%s\n", comment.c_str());
    fclose(file);

    file = fopen((work_dir+course_code+"_class_arrange.txt").c_str(), "w");
    fprintf(file, "0\n");
    fclose(file);
}
void fail_process(string course_name, string prof_code) {
    // [todo] raise an announcement
    string reason;
    cin >> reason;
    string work_dir = ".\\sis_ws\\data_repo\\course_claim\\staff\\"+prof_code+"_fail.txt";
    FILE *file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%s\n", course_name.c_str());
    fprintf(file, "%s\n", reason.c_str());
    fclose(file);
}
struct node {
    string filename;
    node *nxt;
    node *prv;
    explicit node(string s);
};

node::node(string s): filename(std::move(s)), nxt(nullptr), prv(nullptr) {}

void insert_at_tail(node *head, string filename) {
    node *new_node = new node(std::move(filename));
    node *p = head;
    while (p->nxt != head) p = p->nxt;
    new_node->nxt = p->nxt;
    new_node->prv = p;
    head->prv = new_node;
    p->nxt = new_node;
}

void print_list(node *head) {
    if (head == nullptr) return;
    node *p = head;
    bool flag = true;
    while(p != head or flag) {
        printf("%s\n", p->filename.c_str());
        p = p->nxt;
        flag = false;
    }
}
void print_list_rev(node *head) {
    if (head == nullptr) return;
    node *p = head->prv;
    bool flag = true;
    while(p != head->prv or flag) {
        printf("%s\n", p->filename.c_str());
        p = p->prv;
        flag = false;
    }
}
void list_delete(node *&head, node *p) {
    if(p == head) head = head->nxt;
    p->nxt->prv = p->prv;
    p->prv->nxt = p->nxt;
    delete p;
}
void Registry::claim_course() {
    cout << "claiming course" << endl;
    string work_dir = ".\\sis_ws\\data_repo\\course_claim\\registry\\"; // [todo] set work dir
    string index_dir = work_dir + "to_claim_list.txt";
    FILE *file = fopen(index_dir.c_str(), "r");
    if (!file) {
        cout << "could not open file " << index_dir << endl;
    } else {
        int n;
        fscanf(file, "%d", &n);
        printf("%d to be claim\n", n);
        char fn[20];
        fscanf(file, "%s", fn);
        node *head = new node(fn);
        head->nxt = head; head->prv = head;
        for (int i = 1; i < n; i++) {
            char fn[20];
            fscanf(file, "%s", fn);
            insert_at_tail(head, fn);
        }
        printf("claiming course list\n");
        print_list(head);

        node *p = head;
        while(n) {
            string fn = p->filename;
            printf("claiming course %s\n", fn.c_str());
            string course_path = work_dir + fn;
            ifstream course_file(course_path);
            if (!course_file.is_open()) {
                cout << "could not open file " << course_path << endl;
                continue;
            }
            string course_name;
            getline(course_file, course_name);
            string prof_code;
            getline(course_file, prof_code);
            string precourse_code;
            getline(course_file, precourse_code);
            int num_limit;
            bool year_lmt[7] = {false, false, false, false, false, false, false};
            course_file >> num_limit;
            for (int j = 0; j < num_limit; j++) {
                int t;
                course_file >> t;
                year_lmt[t-1] = true;
            }
            string comment;
            getline(course_file, comment);
            getline(course_file, comment);
            course_file.close();
            course_display(course_name, prof_code, precourse_code, year_lmt, comment);
            printf("judgement: (P/F/S/B)\n"); // pass fail skip break
            char c;
            scanf("%c", &c);
            while (c != 'P' && c != 'F' && c != 'S' && c != 'B') scanf("%c", &c);
            if (c == 'P') pass_process(course_name, prof_code, precourse_code, num_limit, year_lmt, comment);
            else if (c == 'F') fail_process(course_name, prof_code);
            else if (c == 'S') {
                p = p->nxt;
                continue;
            }
            else if(c == 'B') break;
            bool deleted = remove(course_path.c_str());
            if (!deleted) {
                p = p->nxt;
                list_delete(head, p->prv);
                n--;
            } else {
                printf("could not delete file\n");
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
string checkCourseCode(string courseCode) {
    cout << "checking course code: " << courseCode << endl;
    string work_dir = ".\\sis_ws\\data_repo\\course\\";
    string index_dir = work_dir + courseCode + ".txt";
    ifstream course_file(index_dir);
    if (!course_file.is_open()) {
        cout << "could not open file " << courseCode << endl;
        return "DNE";
    }
    string course_name;
    getline(course_file, course_name);
    course_file.close();
    return course_name;
}
short getClassNum() {
    cout << "checking class code" << endl;
    string work_dir = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    short n;
    fscanf(file, "%hd", &n);
    return n;
}
void class_pass_process(string prof_code, string courseCode, short classNum, int N, int lec[], int M, int tut[], int quota = 140) {
    // [todo] raise an announcement
    string work_dir = ".\\sis_ws\\data_repo\\class_claim\\staff\\"+prof_code+"_succ.txt";
    FILE *file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%s\n", courseCode.c_str());
    fprintf(file, "%04hd\n", classNum);
    fclose(file);

    work_dir = ".\\sis_ws\\data_repo\\staff\\"+prof_code+".txt"; // [todo] negotiate with tym
    file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%hd\n", classNum);
    fclose(file);

    work_dir = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    file = fopen(work_dir.c_str(), "r");
    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    file = fopen(work_dir.c_str(), "w");
    fprintf(file, "%d\n", n+1);
    fclose(file);

    work_dir = ".\\sis_ws\\data_repo\\class\\"+to_string(classNum)+".txt";
    file = fopen(work_dir.c_str(), "w");
    fprintf(file, "%s\n", prof_code.c_str());
    fprintf(file, "%s\n", courseCode.c_str());
    fprintf(file, "%d\n", quota);
    fprintf(file, "%d\n", N);
    for (int i = 0; i < N; i++) fprintf(file, "%d\n", lec[i]);
    fprintf(file, "%d\n", M);
    for (int i = 0; i < M; i++) fprintf(file, "%d\n", tut[i]);
    for (int i = 0; i < N; i++) fprintf(file, "TBA");
    for (int i = 0; i < M; i++) fprintf(file, "TBA");
    fclose(file);

    work_dir = ".\\sis_ws\\data_repo\\course\\"+courseCode+"_class_arrange.txt";
    file = fopen(work_dir.c_str(), "r");
    fscanf(file, "%d", &n);
    string classCode[n];
    for (int i = 0; i < n; i++) fscanf(file, "%s", classCode[i].c_str());
    fclose(file);

    work_dir = ".\\sis_ws\\data_repo\\course\\"+courseCode+"_class_arrange.txt";
    file = fopen(work_dir.c_str(), "w");
    fprintf(file, "%d\n", n+1);
    for (int i = 0; i < n; i++) fprintf(file, "%s\n", classCode[i].c_str());
    fprintf(file, "%d\n", classNum+1);
    fclose(file);

}
void class_fail_process(string prof_code, string courseCode) {
    // [todo] raise an announcement
    string reason;
    cin >> reason;
    string work_dir = ".\\sis_ws\\data_repo\\class_claim\\staff\\"+prof_code+"_fail.txt";
    FILE *file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%s\n", courseCode.c_str());
    fprintf(file, "%s\n", reason.c_str());
    fclose(file);
}
void Registry::claim_class() {
    cout << "claiming class" << endl;
    string work_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\";
    string index_dir = work_dir + "to_claim_list.txt";
    FILE *file = fopen(index_dir.c_str(), "r");
    if (!file) {
        cout << "could not open file " << index_dir << endl;
    } else {
        int n;
        fscanf(file, "%d", &n);
        printf("%d to be claim\n", n);
        char fn[20];
        fscanf(file, "%s", fn);
        node *head = new node(fn);
        head->prv = head; head->nxt = head;
        for (int i = 1; i < n; i++) {
            fscanf(file, "%s", fn);
            insert_at_tail(head, fn);
        }
        printf("class claim list\n");
        print_list(head);
        node *p = head;
        while(n) {
            string fn = p->filename;
            printf("class %s\n", fn.c_str());
            string class_path = work_dir + fn;
            ifstream class_file(class_path);
            if (!class_file.is_open()) {
                cout << "could not open file " << class_path << endl;
            } else {
                string courseName, courseCode;
                getline(class_file, courseCode);
                courseName = checkCourseCode(courseCode);
                if(courseName == "DNE") {
                    printf("could not find course %s\n", courseCode.c_str());
                    class_file.close();
                    continue;
                }
                cout << "Course Name: " << courseName << endl;
                string prof_code;
                getline(class_file, prof_code);
                int quota;
                class_file >> quota;
                int N, M;
                int lec[28];
                int tut[28];
                class_file >> N;
                for (int i = 0; i < N; i++) class_file >> lec[i];
                class_file >> M;
                for (int i = 0; i < M; i++) class_file >> tut[i];
                class_file.close();
                short classCode = getClassNum()+short(1);
                Course course(courseCode, courseName, prof_code, 3, classCode, N, M, lec, tut, quota);
                course.printCourse();
                char c;
                printf("judgement: (P/F/S/B)\n"); // pass fail skip break
                scanf("%c", &c);
                while (c != 'P' && c != 'F' && c != 'S' && c != 'B') scanf("%c", &c);
                if (c == 'P') class_pass_process(prof_code, courseCode, classCode, N, lec, M, tut, quota);
                else if (c == 'F') class_fail_process(prof_code, courseCode);
                else if (c == 'S') {
                    p = p->nxt;
                    continue;
                }
                else if(c == 'B') break;
                bool deleted = remove(class_path.c_str());
                if (!deleted) {
                    printf("%s claimed successfully\n", p->filename.c_str());
                    p = p->nxt;
                    list_delete(head, p->prv);
                    n--;
                } else {
                    printf("could not delete file\n");
                }
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
typedef struct classroom{
    int capacity;
    int cnt;
    string location;
    bool usable;
    bool lec_timeslot[28], tut_timeslot[21];
    classroom *nxt;
    explicit classroom(int capacity, string location, bool usable, int cnt=0, classroom *nxt = nullptr) {
        this->capacity = capacity;
        this->location = std::move(location);
        this->usable = usable;
        this->nxt = nxt;
        this->cnt = cnt;
        for (int i = 0; i < 28; i++) this->lec_timeslot[i] = true;
        for (int i = 0; i < 21; i++) this->tut_timeslot[i] = true;
    }
}CR;

void output_usable_classroom(CR *head[], int kinds_cr) {
    for (int i = 0; i < kinds_cr; i++) {
        printf("capacity: %d\n", head[i]->capacity);
        for (CR *j = head[i]->nxt; j != nullptr; j = j->nxt) printf("location: %s usable: %d  ", j->location.c_str(), j->usable);
        printf("\n");
    }
}
map<int, int> mp;
int load_classroom(CR *head[]) {
    int cnt = 0;
    string index_path = ".\\sis_ws\\data_repo\\class\\classroom.txt";
    FILE *file = fopen(index_path.c_str(), "r");
    int kinds_cr;
    fscanf(file, "%d", &kinds_cr);
    for(int i = 0; i < kinds_cr; i++) {
        int capacity;
        fscanf(file, "%d", &capacity);
        head[i]->capacity = capacity;
        mp[capacity] = i;
        int num_classroom;
        fscanf(file, "%d", &num_classroom);
        CR *p = head[i];
        for(int j = 0; j < num_classroom; j++) {
            string location;
            int usable;
            char fn[20];
            fscanf(file, "%s", fn);
            location = string(fn);
            fscanf(file, "%d", &usable);
            CR *new_node = new CR(capacity, location, bool(usable), cnt++);
            p->nxt = new_node;
            p = p->nxt;
        }
    }
    fclose(file);
    return kinds_cr;
}

void Registry::classroom_arrangement() {
    cout << "classroom_arrangement" << endl;
    string work_dir = ".\\sis_ws\\data_repo\\class_claim\\registry\\to_claim_list.txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    if(n != 0) printf("Warning: some classes haven't been approved\n");

    work_dir = ".\\sis_ws\\data_repo\\class\\";
    string index_path = work_dir + "Class Number.txt";
    file = fopen(index_path.c_str(), "r");
    fscanf(file, "%d", &n);
    fclose(file);

    // load classroom
    CR *heads[50];
    for (int i = 0; i < 50; i++) heads[i] = new CR(0, "TBA", false);
    int kinds_cr = load_classroom(heads);
    output_usable_classroom(heads, kinds_cr);

    Course *course[n];
    for(short i = 1; i <= n; i++) {
        course[i] = new Course(i);
        int cap = course[i]->capacity;
        int num_lec = course[i]->num_lec, num_tut = course[i]->num_tut;
        for (int j = 0; j < num_lec; j++) {
            CR *head = heads[mp[cap]], *p = head->nxt;
            while (p != nullptr && (!p->usable || !p->lec_timeslot[course[i]->lec[j]-1])) {
                printf("%s %d %d %d\n", p->location.c_str(), p->usable, p->lec_timeslot[course[i]->lec[j]-1], course[i]->lec[j]);
                p = p->nxt;
            }
            if (p == nullptr) printf("Warning: class %hd lec %d can't be arranged\n", course[i]->classCode, course[i]->lec[j]);
            else {
                p->lec_timeslot[course[i]->lec[j]-1] = false;
                printf("%s ->lec_timeslot[%d] = false, %d occupied\n", p->location.c_str(), course[i]->lec[j], i);
                course[i]->lec_classroom[j] = p->location;
            }
        }
        for (int j = 0; j < num_tut; j++) {
            CR *head = heads[mp[cap]], *p = head;
            while (p != nullptr && (!p->usable || !p->lec_timeslot[course[i]->tut[j]-1])) p = p->nxt;
            if (p == nullptr) printf("warning: class %hd tut %d cannot be arranged\n", course[i]->classCode, course[i]->tut[j]);
            else {
                p->tut_timeslot[course[i]->tut[j]-1] = false;
                course[i]->tut_classroom[j] = p->location;
            }
        }
    }

    for (short i = 1; i <= n; i++) {
        course[i]->print2File();
        printf("%d\n", course[i]->capacity);
        printf("lec:\n");
        for(int j = 0; j < course[i]->num_lec; j++) {
            printf("%d %d %s\n", course[i]->classCode, course[i]->lec[j], course[i]->lec_classroom[j].c_str());
        }
        printf("tut:\n");
        for (int j = 0; j < course[i]->num_tut; j++) {
            printf("%d %d %s\n", course[i]->classCode, course[i]->tut[j], course[i]->tut_classroom[j].c_str());
        }
    }
}

void read_transcript(string stuID, map<string, multimap<string, double> > &stu_lst) {
    string work_dir = ".\\sis_ws\\data_repo\\student\\transcript\\"+stuID + ".txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    if(file == nullptr) {
        stu_lst[stuID] = multimap<string, double>();
        return;
    }
    multimap<string, double> this_grade;
    int n;
    fscanf(file, "%d", &n);
    for(int i = 0; i < n; i++) {
        char courseCode[10]; double GPA;
        fscanf(file, "%s %lf", courseCode, &GPA);
        this_grade.insert(pair<string, double>(courseCode, GPA));
    }
    fclose(file);
    stu_lst.insert(make_pair(stuID, this_grade));
}
typedef struct stu_final_gpa {
    string stuID;
    double gpa;
    explicit stu_final_gpa(string inputstuID, double inputgpa) {
        stuID = inputstuID; gpa = inputgpa;
    }
    bool operator<(const stu_final_gpa& other) const {
        return this->gpa > other.gpa;
    }
}sfg;
void print_transcript(map<string, multimap<string, double> > stu_lst, map<string, int> mp_unit) {
    string work_dir = ".\\sis_ws\\data_repo\\student\\transcript\\";
    map<int, set<sfg> > mp_stu_grade;
    for(auto it = stu_lst.begin(); it != stu_lst.end(); it++) {
        double grade_point = 0, tot_unit = 0;
        multimap<string, double> this_grade = it->second;
        printf("%s\n", it->first.c_str());
        string stu_dir = work_dir + it->first + ".txt";
        FILE *fp = fopen(stu_dir.c_str(), "w");
        printf("%llu\n", this_grade.size());
        fprintf(fp, "%llu\n", this_grade.size());
        for(auto it1 = this_grade.begin(); it1 != this_grade.end(); it1++) {
            printf("%s %d %lf\n", it1->first.c_str(), mp_unit[it1->first], it1->second);
            fprintf(fp, "%s %lf\n", it1->first.c_str(), it1->second);
            grade_point += it1->second;
            tot_unit += mp_unit[it1->first];
        }
        fprintf(fp, "%lf %lf %lf\n", tot_unit, grade_point, grade_point/tot_unit);
        fclose(fp);
        string stuID = it->first;
        stu_final_gpa node(stuID, grade_point/tot_unit);
        mp_stu_grade[atoi(stuID.substr(0,3).c_str())].insert(node);
    }
    work_dir = ".\\sis_ws\\data_repo\\registry\\";
    for(auto it = mp_stu_grade.begin(); it != mp_stu_grade.end(); it++) {
        printf("%d\n", it->first);
        string curr_dir = work_dir + to_string(it->first) + ".txt";
        FILE *file = fopen(curr_dir.c_str(), "w");
        fprintf(file, "%llu\n", it->second.size());
        for(const auto& it1 : it->second) {
            printf("%s %lf\n", it1.stuID.c_str(), it1.gpa);
            fprintf(file, "%s %lf\n", it1.stuID.c_str(), it1.gpa);
            if(it1.gpa > 3.5) {
                // [todo] raise announcement to stuID
                // [todo] announcement could be raised to inform students' their GPA and rank, also Dean's list if they get in
            }
        }
        fclose(file);
    }
}
void Registry::stu_final_grade() {
    printf("stu_final_grade\n");
    map<string, multimap<string, double> > stu_lst;
    map<char, double> mp_grd;
    map<string, int> mp_unit;
    mp_grd['a'] = 4.0; mp_grd['b'] = 3.7; mp_grd['c'] = 3.3; mp_grd['d'] = 3.0; mp_grd['e'] = 2.7; mp_grd['f'] = 2.3; mp_grd['g'] = 2.0; mp_grd['h'] = 1.7; mp_grd['i'] = 1.3; mp_grd['j'] = 1.0; mp_grd['k'] = 0.7; mp_grd['l'] = 0;
    ifstream infile;
    string work_dir = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    printf("%d classes in total\n", n);
    work_dir = ".\\sis_ws\\data_repo\\student_grade\\";
    for(int i = 1; i <= n; i++) {
        infile.open(work_dir + "\\"+to_string(i)+".txt");
        if(!infile.is_open()) {
            printf("%d not found\n", i);
            continue;
        }
        Course C = Course(i);
        string courseCode = C.courseCode;
        int unit = C.unit;
        mp_unit[courseCode] = unit;
        printf("%s\n", courseCode.c_str());
        string line;
        while(getline(infile, line)) {
            string stu_ID = line.substr(0, 7);
            double grade = mp_grd[line[7]];
            printf("%s %lf\n", stu_ID.c_str(), grade);
            map<string, multimap<string, double> >::iterator it = stu_lst.find(stu_ID);
            if(it == stu_lst.end()) {
                read_transcript(stu_ID, stu_lst);
            }
            stu_lst[stu_ID].insert(pair<string, double>(courseCode, grade));
        }
        infile.close();
        printf("%d is over\n", i);
    }
    print_transcript(stu_lst, mp_unit);
}

void Registry::fetchOCTE() {
    string work_dir = ".\\sis_ws\\data_repo\\class\\Class Number.txt";
    FILE *file = fopen(work_dir.c_str(), "r");
    int class_number;
    fscanf(file, "%d", &class_number);
    fclose(file);
    for(int i = 1; i <= class_number; i++) {
        int tot_sc[5] = {}, tot_num = 0;
        work_dir = ".\\sis_ws\\data_repo\\student_temp_grade\\"+to_string(i)+".txt";
        ifstream infile(work_dir);
        string studentCode;
        while(getline(infile, studentCode)) {
            studentCode = studentCode.substr(0, 7);
            work_dir = ".\\sis_ws\\data_repo\\octe\\"+to_string(i)+"\\"+studentCode+".txt";
            file = fopen(work_dir.c_str(), "r");
            if (!file) {
                cout << studentCode << "didn't finish OCTE" << endl;
            } else {
                int sc[5];
                for(int j = 0; j < 5; j++) fscanf(file, "%d", &sc[j]), tot_sc[j] += sc[j];
                fclose(file);
                tot_num++;
            }
        }
        infile.close();
        work_dir = ".\\sis_ws\\data_repo\\octe\\"+to_string(i)+"\\summary.txt";
        file = fopen(work_dir.c_str(), "w");
        fprintf(file, "%d ", tot_num);
        for(int j = 0; j < 5; j++) fprintf(file, "%d ", tot_sc[j]);
        fclose(file);
    }
}