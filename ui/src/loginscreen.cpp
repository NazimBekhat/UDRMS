#include "loginscreen.h"
#include "ui_loginscreen.h"
#include "University.h"
#include "User.h"
#include "uihelpers.h"
#include <QMessageBox>

LoginScreen::LoginScreen(University* university,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginScreen), university(university)
{
    ui->setupUi(this);
    applyShadow(ui->usernameLineEdit);
    applyShadow(ui->passwordLineEdit);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginScreen::onLoginClicked);
}

void LoginScreen::clearFields()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

void LoginScreen::onLoginClicked()
{
    std::string username = ui->usernameLineEdit->text().toStdString();
    std::string password = ui->passwordLineEdit->text().toStdString();

    User* user = university->findUserByUsername(username);

    if (user == nullptr || !user->login(password)) {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        return;
    }

    emit loginSucceeded(user);
}