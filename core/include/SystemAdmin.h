#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H
#include "User.h"

class SystemAdmin: public User{        
    public:
        SystemAdmin(const string& username, const string& password);

        virtual string getRole() const override;
        void backup();
        void loadData();
};

#endif