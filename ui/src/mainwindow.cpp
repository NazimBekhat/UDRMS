#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "loginscreen.h"
#include "University.h"
#include "User.h"

MainWindow::MainWindow(University* university, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , university(university)
{
    ui->setupUi(this);

    loginScreen = new LoginScreen(university, this);
    ui->stackedWidget->addWidget(loginScreen);
    ui->stackedWidget->setCurrentWidget(loginScreen);

    connect(loginScreen, &LoginScreen::loginSucceeded, this, &MainWindow::onLoginSucceeded);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onLoginSucceeded(User* user)
{
    QMessageBox::information(this, "Login Successful", QString::fromStdString(user->getFullName()));
    // TODO: switch to dashboard once it exists, passing `user` along
}