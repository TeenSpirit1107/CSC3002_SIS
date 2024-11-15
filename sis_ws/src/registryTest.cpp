#include <iostream>
#include "registry.hpp"

using namespace std;

int main(){
    Registry reg{};
    reg.output_basic_info();
    // Announcement test;
    // test.setContent("Class application approved", "You application of adding CSC3100 at Wednesday has been approved!\nYou can check weekly schedule now.");
    // test.setPromoter("Rui HUANG");
    // test.setTarget("123090001");
    // test.send();
    while(1) {
        char op;
        scanf("%c", &op);
        if(op == 'a') {
            // claim courses.
            reg.claim_course();
        }
        else if(op == 'b') {
            // claim classes.
            reg.claim_class();
        }
        else if(op == 'c') {
            reg.classroom_arrangement();
        }
        else if(op == 'e') {
            break;
        }
    }
    return 0;
}