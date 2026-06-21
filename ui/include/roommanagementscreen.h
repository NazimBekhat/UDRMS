#ifndef ROOMMANAGEMENTSCREEN_H
#define ROOMMANAGEMENTSCREEN_H
#include <QWidget>

class University;
class User;
class Staff;
class Room;

namespace Ui {
class RoomManagementScreen;
}

class RoomManagementScreen : public QWidget
{
    Q_OBJECT

public:
    explicit RoomManagementScreen(University* university, QWidget *parent = nullptr);
    ~RoomManagementScreen();

    void refresh();
    void setUser(User* user);   
signals:
    void requestBack();

private slots:
    void onDormitoryChanged(int index);
    void onRoomSelected(int row);
    void onAssignClicked();
    void onRemoveClicked();
    void onSaveMenuClicked();
    void onAddDormitoryClicked();
    void onAddRoomClicked();

private:
    void populateDormitoryList();
    void populateRoomList();
    void updateMenuLabels();
    bool canEditMenu() const;  

    Ui::RoomManagementScreen *ui;
    University* university;
    User* currentUser;
    int currentDormitoryIndex;
    Room* currentRoom;
};
#endif