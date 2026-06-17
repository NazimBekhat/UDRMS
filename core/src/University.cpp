#include "University.h"
#include "Student.h"
#include <algorithm>
using std::remove;


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
//std::move doesn't actually move anything. It just casts dormitory to an rvalue reference — telling the compiler "treat this as a temporary, you can steal from it."
//Without std::move: dormitories.push_back(dormitory); // dormitory has a name → lvalue → compiler looks for copy constructor → deleted → error
//With std::move: dormitories.push_back(std::move(dormitory)); // std::move casts it to rvalue → compiler looks for move constructor → found → ok
// The actual "stealing" happens inside Dormitory's move constructor — it takes the rooms vector's internal buffer and leaves the source empty.
// std::move = permission slip. Move constructor = the one that does the work.

User* University::findUserByID(int userID){
    for (User* user: users){
        if (dynamic_cast<Student*>(user) == nullptr) continue;
        if (dynamic_cast<Student*>(user)->getStudentID() == userID) return dynamic_cast<Student*>(user);
    }
    return nullptr;
}