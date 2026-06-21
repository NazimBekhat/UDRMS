#ifndef CREATEUSERSCREEN_H
#define CREATEUSERSCREEN_H
#include <QWidget>

class University;
class User;

namespace Ui {
class CreateUserScreen;
}

class CreateUserScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CreateUserScreen(University* university, QWidget *parent = nullptr);
    ~CreateUserScreen();

    void setUser(User* user); 

signals:
    void requestBack();

private slots:
    void onRoleChanged(int index);
    void onCreateClicked();

private:
    Ui::CreateUserScreen *ui;
    University* university;
    User* currentUser;
};
#endif