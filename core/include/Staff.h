#ifndef STAFF_H
#define STAFF_H

#include "User.h"
#include <string>
using std::string; 

class Staff: public User{
    private:
        int staffID;
        string departement;

    public:
        Staff(const string& username, const string& password,int staffID, const string& departement, const string& fullName);

        virtual string getRole() const override;

        void setStaffID(int staffID);
        int getStaffID() const;

        void setDepartement(const string& departement);
        string getDepartement() const;
};

#endif