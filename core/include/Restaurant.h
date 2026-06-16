#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
using std::string;

class Restaurant{
    private:
        string breakFastMenu;
        string lunchMenu;
        string dinnerMenu;
    public:
        Restaurant(const string& breakFastMenu="", const string& lunchMenu="", const string& dinnerMenu="");

        void setBreakFastMenu(const string& breakFastMenu);
        string getBreakFastMenu() const;
        void setLunchMenu(const string& lunchMenu);
        string getLunchMenu() const;
        void setDinnerMenu(const string& dinnerMenu);
        string getDinnerMenu() const;

        void displayMenu() const;
};

#endif