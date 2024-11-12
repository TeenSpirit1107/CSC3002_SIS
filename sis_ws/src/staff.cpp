#include"staff.hpp"

Staff::Staff() {

}
Staff::Staff(string prof_code) {
    school = "SDS";
    userID = prof_code;
    userName = "Rui HUANG";
    userType = 'b';
}
Staff::~Staff() {

}
string Staff::getName() {
    return userName;
}
