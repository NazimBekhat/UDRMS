#ifndef USER_H
#define USER_H

#include <string>
using std::string; //we needed jsut string so no need to impotr the whole library

class User{
    private:
        string username;
        string password;
    public:
        User(const string& username, const string& password);
        virtual ~User()=default; //so when we delete a list of pointers of type user it uses the correct destructor of the object

        virtual bool login(const string& inputPassword); //virtual
        virtual string getRole()const =0; //pure virtual, const sicne it should be called by non const objects

        void setUsername(const string& username);
        string getUsername() const;
        void setPassword(const string& password);
        //string getPassword() const;
};

#endif