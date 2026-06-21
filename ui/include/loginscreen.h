#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>

class User;
class University;

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(University* university,QWidget *parent = nullptr);
    void clearFields();
    ~LoginScreen();

signals:
    void loginSucceeded(User* user);

private slots:
    void onLoginClicked();

private:
    Ui::LoginScreen *ui;
    University* university;
};

#endif // LOGINSCREEN_H
