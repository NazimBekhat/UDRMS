#include "Administrator.h"

Administrator::Administrator(const string& username, const string& password, int adminID): User(username, password), adminID(adminID){}

int Administrator::getAdminID() const{
    return adminID;
}

void Administrator::setAdminID(int adminID){
    this->adminID = adminID;
}

string Administrator::getRole() const{
    return "Administrator";
}
