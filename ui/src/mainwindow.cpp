#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "University.h"
#include "User.h"
#include "loginscreen.h"
#include "dashboardscreen.h"
#include "roommanagementscreen.h"

MainWindow::MainWindow(University* university, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , university(university)
{
    ui->setupUi(this);

    loginScreen = new LoginScreen(university, this);
    dashboardScreen = new DashboardScreen(university, this);
    roomManagementScreen = new RoomManagementScreen(university, this);

    ui->stackedWidget->addWidget(loginScreen);
    ui->stackedWidget->addWidget(dashboardScreen);
    ui->stackedWidget->addWidget(roomManagementScreen);

    ui->stackedWidget->setCurrentWidget(loginScreen);

    connect(loginScreen, &LoginScreen::loginSucceeded, this, &MainWindow::onLoginSucceeded);
    connect(dashboardScreen, &DashboardScreen::requestRoomManagement, this, &MainWindow::onRequestRoomManagement);
    connect(dashboardScreen, &DashboardScreen::requestRestaurantMenu, this, &MainWindow::onRequestRoomManagement);
    connect(roomManagementScreen, &RoomManagementScreen::requestBack, this, &MainWindow::onRequestBackToDashboard);
    connect(dashboardScreen, &DashboardScreen::requestLogout, this, &MainWindow::onLogout);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onLoginSucceeded(User* user)
{
    dashboardScreen->setUser(user);
    roomManagementScreen->setUser(user);
    ui->stackedWidget->setCurrentWidget(dashboardScreen);
}

void MainWindow::onRequestRoomManagement()
{
    roomManagementScreen->refresh(); 
    ui->stackedWidget->setCurrentWidget(roomManagementScreen);
}

void MainWindow::onRequestBackToDashboard()
{
    ui->stackedWidget->setCurrentWidget(dashboardScreen);
}

void MainWindow::onLogout()
{
    loginScreen->clearFields();
    ui->stackedWidget->setCurrentWidget(loginScreen);
}