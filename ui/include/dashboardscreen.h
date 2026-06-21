#ifndef DASHBOARDSCREEN_H
#define DASHBOARDSCREEN_H
#include <QWidget>

class University;
class User;


namespace Ui {
class DashboardScreen;
}

class DashboardScreen : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardScreen(University* university,QWidget *parent = nullptr);
    ~DashboardScreen();

    void setUser(User* user);

signals:
    void requestRoomManagement();
    void requestRestaurantMenu();

private slots:
    void onBackupClicked();
    void onLoadClicked();

private:
    Ui::DashboardScreen *ui;
    University* university;
    User* currentUser;
};

#endif // DASHBOARDSCREEN_H
