#include "Restaurant.h"
#include <iostream>
using namespace std;

Restaurant::Restaurant(const string& breakFastMenu, const string& lunchMenu, const string& dinnerMenu): breakFastMenu(breakFastMenu), lunchMenu(lunchMenu), dinnerMenu(dinnerMenu){}

void Restaurant::setBreakFastMenu(const string& breakFastMenu){
    this->breakFastMenu = breakFastMenu;
}

string Restaurant::getBreakFastMenu() const{
    return breakFastMenu;
}

void Restaurant::setLunchMenu(const string& lunchMenu){
    this->lunchMenu = lunchMenu;
}

string Restaurant::getLunchMenu() const{
    return lunchMenu;
}

void Restaurant::setDinnerMenu(const string& dinnerMenu){
    this->dinnerMenu = dinnerMenu;
}

string Restaurant::getDinnerMenu() const{
    return dinnerMenu;
}

void Restaurant::displayMenu() const{
    cout<<"The breakfast menu: "<<breakFastMenu<<endl;
    cout<<"The lunch menu: "<<lunchMenu<<endl;;
    cout<<"The dinner menu: "<<dinnerMenu<<endl;;
}