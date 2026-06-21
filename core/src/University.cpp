#include "University.h"
#include "Student.h"
#include "Administrator.h"
#include "Staff.h"
#include <algorithm>
using std::remove;

#include "Dormitory.h"

University::University(const string& name): name(name){}

University::~University(){
    for (User* user: users){
        delete user;
    }
}

string University::getName() const{
    return name;
}

void University::setName(const string& name){
    this->name = name;
}

const vector<User*>& University::getUsers() const {
    return users;
}

const vector<Dormitory>& University::getDormitories() const {
    return dormitories;
}

vector<Dormitory>& University::getDormitories() {
    return dormitories;
}

void University::addUser(User* user){
    if (user == nullptr) return;
    users.push_back(user);
}

void University::removeUser(User* user){
    if (user == nullptr) return;
    users.erase(remove(users.begin(), users.end(), user), users.end());
    delete user;
}

void University::addDormitory(Dormitory&& dormitory){
    dormitories.push_back(std::move(dormitory));
}


User* University::findUserByID(int userID){
    for (User* user: users){
        if (Student* s = dynamic_cast<Student*>(user)){
            if (s->getStudentID() == userID) return s;
        } else if (Staff* st = dynamic_cast<Staff*>(user)){
            if (st->getStaffID() == userID) return st;
        } else if (Administrator* a = dynamic_cast<Administrator*>(user)){
            if (a->getAdminID() == userID) return a;
        }
    }
    return nullptr;
}

User* University::findUserByUsername(const string& username){
    for (User* user : users){
        if (user->getUsername() == username) return user;
    }
    return nullptr;
}

void University::clearAll(){
    for (User* u : users) {
        delete u;
    }
    users.clear();
    dormitories.clear();
}