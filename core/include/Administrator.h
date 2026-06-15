#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H
#include "User.h"

class Administrator: public User{
    private:
        int adminID;
    public:
        Administrator(const string& username, const string& password, int adminID);

        int getAdminID() const;
        void setAdminID(int adminID);

        virtual string getRole() const override;

};

#endif