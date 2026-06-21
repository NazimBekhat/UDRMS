#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class University;
class LoginScreen;
class DashboardScreen;
class RoomManagementScreen;
class User;
class CreateUserScreen; 

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
    void onRequestRoomManagement();
    void onRequestBackToDashboard();
    void onLogout(); 
    void onRequestCreateUser();  
    void onDataLoaded(QString username); 

private:
    Ui::MainWindow *ui;
    University* university;
    LoginScreen* loginScreen;
    DashboardScreen* dashboardScreen;
    RoomManagementScreen* roomManagementScreen;
    CreateUserScreen* createUserScreen;  
    User* loggedInUser; 
};
#endif // MAINWINDOW_H