#include "SystemAdmin.h"

SystemAdmin::SystemAdmin(const string& username, const string& password): User(username, password){}
string SystemAdmin::getRole() const{
    return "SystemAdmin";
};
