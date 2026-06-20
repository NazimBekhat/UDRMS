#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class University;
class LoginScreen;
class User;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(University* university, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoginSucceeded(User* user);

private:
    Ui::MainWindow *ui;
    University* university;
    LoginScreen* loginScreen;
};
#endif // MAINWINDOW_H