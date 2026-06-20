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
        void removeUser(User* user);
        void addDormitory(Dormitory&& dormitory); //When you write addDormitory(Dormitory dormitory) — pass by value — the compiler needs to construct that local copy from whatever you pass in.
        //If you pass a temporary: university.addDormitory(Dormitory("Block A", 5)); Constructing that local copy = invoking the copy constructor. Copy is deleted → compile error.
        //Now you're binding directly to the temporary — no copy involved. Inside the function you do dormitories.push_back(std::move(dormitory)) to move it into the vector.
        User* findUserByID(int userID);
        User* findUserByUsername(const std::string& username);

        University(const University&)=delete;
        University& operator=(const University&)=delete;
};

#endif