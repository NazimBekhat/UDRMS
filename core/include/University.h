#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include <string>
using std::string;
#include <vector>
using std::vector;

class User;
#include "Dormitory.h"

class University{
    private:
        string name;
        vector<User*> users;
        vector<Dormitory> dormitories;

    public:
        University(const string& name);
        ~University();

        string getName() const;
        void setName(const string& name);

        const vector<User*>& getUsers() const;
        const vector<Dormitory>& getDormitories() const;
        vector<Dormitory>& getDormitories();

        void addUser(User* user);
        void clearAll();
        void removeUser(User* user);
        void addDormitory(Dormitory&& dormitory); 
        User* findUserByID(int userID);
        User* findUserByUsername(const std::string& username);

        University(const University&)=delete;
        University& operator=(const University&)=delete;
};

#endif