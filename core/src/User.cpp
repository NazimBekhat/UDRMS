#include "User.h"
#include <iostream>
using namespace std;

User::User(const string& username, const string& password): username(username), password(password){}

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

