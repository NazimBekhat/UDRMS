#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H
#include "User.h"

class University;

class SystemAdmin: public User{        
    public:
        SystemAdmin(const string& username, const string& password, const string& fullName);

        virtual string getRole() const override;
        void backup(University& uni, const string& filename);
        void loadData(University& uni, const string& filename);
};

#endif