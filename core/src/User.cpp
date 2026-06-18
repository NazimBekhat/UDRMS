#include "User.h"
#include <iostream>
using namespace std;

User::User(const string& username, const string& password,const string& fullName): username(username), password(password), fullName(fullName){}

bool User::login(const string& inputPassword){
    return inputPassword == password;
}

void User::setUsername(const string& username){
    this->username = username;
}

string User::getUsername() const{
    return username;
}

void User::setPassword(const string& password){
    this->password = password;
}

// string User::getPassword() const{
//     return password;
// };

void User::setFullName(const string& fullName){
    this->fullName = fullName;
}

string User::getFullName() const{
    return fullName;
}

