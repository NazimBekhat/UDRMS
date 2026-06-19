#include "Staff.h"
#include <string>
using namespace std;

Staff::Staff(const string& username, const string& password,int staffID, const string& departement, const string& fullName): User(username,password,fullName), staffID(staffID), departement(departement){};

string Staff::getRole() const{
    return "Staff";
};

void Staff::setStaffID(int staffID){
    this->staffID = staffID;
};

int Staff::getStaffID() const{
    return staffID;
};

void Staff::setDepartement(const string& departement){
    this->departement = departement;
};

string Staff::getDepartement() const{
    return departement;
};