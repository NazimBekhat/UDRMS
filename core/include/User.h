#ifndef USER_H
#define USER_H

#include <string>
using std::string;

class User{
    private:
        string username;
        string password;
        string fullName;
    public:
        User(const string& username, const string& password,const string& fullName);
        virtual ~User()=default; 

        virtual bool login(const string& inputPassword);
        virtual string getRole()const =0;

        void setUsername(const string& username);
        string getUsername() const;
        void setPassword(const string& password);
        void setFullName(const string& fullName);
        string getFullName() const;
};

#endif