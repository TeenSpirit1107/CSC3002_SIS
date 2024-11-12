#include "registry.hpp"
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <course.hpp>
#include <fstream>
#include <utility>
using namespace std;

Registry::Registry() {
    this->userID = "0000000";
    this->userName = "Registry";
    this->userType = 'a';
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
void class_pass_process(string prof_code, string courseCode, short classNum, int N, int lec[], int M, int tut[]) {
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
    file = fopen(work_dir.c_str(), "a+");
    fprintf(file, "%s\n", prof_code.c_str());
    fprintf(file, "%s\n", courseCode.c_str());
    fprintf(file, "%d\n", N);
    for (int i = 0; i < N; i++) fprintf(file, "%d\n", lec[i]);
    fprintf(file, "%d\n", M);
    for (int i = 0; i < M; i++) fprintf(file, "%d\n", tut[i]);
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
                int N, M;
                int lec[28];
                int tut[28];
                class_file >> N;
                for (int i = 0; i < N; i++) class_file >> lec[i];
                class_file >> M;
                for (int i = 0; i < M; i++) class_file >> tut[i];
                class_file.close();
                short classCode = getClassNum()+short(1);
                Staff staff(prof_code);
                Course course(courseCode, courseName, staff, classCode, N, M, lec, tut);
                course.printCourse();
                char c;
                printf("judgement: (P/F/S/B)\n"); // pass fail skip break
                scanf("%c", &c);
                while (c != 'P' && c != 'F' && c != 'S' && c != 'B') scanf("%c", &c);
                if (c == 'P') class_pass_process(prof_code, courseCode, classCode, N, lec, M, tut);
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
