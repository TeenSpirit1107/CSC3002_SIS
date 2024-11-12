#include <iostream>
#include "registry.hpp"

using namespace std;

int main(){
    Registry reg{};
    reg.output_basic_info();
    /*while(1) {
        char op;
        scanf("%c", &op);
        if(op == 'a') {*/
            // claim courses.
            //reg.claim_course();
        //}
        //else if(op == 'b') {
            // claim classes.
            reg.claim_class();
        //}
   // }
    return 0;
}