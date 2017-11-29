#include "login_window.h"
#include <QMessageBox>
#include <iostream>
#include <map>
#include <math.h>


using namespace std;

LoginWindow::LoginWindow(MyDataStore* ds) {
	data = ds;
	setWindowTitle("Sign In");
	initializeComponents();
	setupLayout();
	connectEvents();
	overallLayout->addLayout(layout);
	overallLayout->addLayout(createUserLayout);
	overallLayout->addWidget(quitButton);
	setLayout(overallLayout);
}

LoginWindow::~LoginWindow() {
	delete signInLabel;
	delete usernameLabel;
	delete usernameInput;
	delete passwordLabel;
	delete passwordInput;
	delete loginButton;
	delete createLabel;
	delete newUserLabel;
	delete newUserInput;
	delete newPasswordLabel;
	delete newPasswordInput;
	delete createButton;
	delete quitButton;
	delete layout;
	delete newUserLayout;
	delete newPasswordLayout;
	delete createUserLayout;
	//delete usernameLayout;
	//delete passwordLayout;
	//delete overallLayout;
}

void LoginWindow::initializeComponents() {
	overallLayout = new QHBoxLayout();
	layout = new QVBoxLayout();
	usernameLayout = new QHBoxLayout();
	passwordLayout = new QHBoxLayout();
	signInLabel = new QLabel(QString::fromStdString("Sign In"));
	usernameLabel = new QLabel(QString::fromStdString("Username: "));
	usernameInput = new QLineEdit();
	passwordLabel = new QLabel(QString::fromStdString("Password: "));
	passwordInput = new QLineEdit();
	loginButton = new QPushButton("Log In");
	createUserLayout = new QVBoxLayout();
	newUserLayout = new QHBoxLayout();
	newPasswordLayout = new QHBoxLayout();
	createLabel = new QLabel(QString::fromStdString("Create Account"));
	newUserLabel = new QLabel(QString::fromStdString("Username: "));
	newUserInput = new QLineEdit();
	newPasswordLabel = new QLabel(QString::fromStdString("Password: "));
	newPasswordInput = new QLineEdit();
	createButton = new QPushButton("Create Account");
	quitButton = new QPushButton("Quit");
}

void LoginWindow::setupLayout() {
	layout->addWidget(signInLabel);
	usernameLayout->addWidget(usernameLabel);
	usernameLayout->addWidget(usernameInput);
	layout->addLayout(usernameLayout);
	passwordLayout->addWidget(passwordLabel);
	passwordLayout->addWidget(passwordInput);
	layout->addLayout(passwordLayout);
	layout->addWidget(loginButton);
	createUserLayout->addWidget(createLabel);
	newUserLayout->addWidget(newUserLabel);
	newUserLayout->addWidget(newUserInput);
	createUserLayout->addLayout(newUserLayout);
	newPasswordLayout->addWidget(newPasswordLabel);
	newPasswordLayout->addWidget(newPasswordInput);
	createUserLayout->addLayout(newPasswordLayout);
	createUserLayout->addWidget(createButton);
}
void LoginWindow::connectEvents() {
	connect(loginButton, SIGNAL(clicked()), this, SLOT(login()));
	connect(createButton, SIGNAL(clicked()), this, SLOT(create()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

unsigned long long LoginWindow::hashFunction(int n) {
	string password;
	if (n == 0)
		password = passwordInput->text().toStdString();
	else if (n == 1)
		password = newPasswordInput->text().toStdString();
	int size = password.length();
	unsigned long long digits = 0;
	for (int i = 0; i < size; i++) {
		digits += (unsigned long long)(pow(128, i) * password[size - i - 1]);
	}
	unsigned int digit[4];
	for (int i = 3; i >= 0; i--) {
		digit[i] = digits % 65521;
		digits /= (unsigned long long)65521;
	}
	unsigned long long encrypt = (45912 * digit[0] + 35511 * digit[1] + 65169 * digit[2] + 4625 * digit[3]) % 65521;
	return encrypt;
}
void LoginWindow::login() {
	if (usernameInput->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Username is empty");
		mymsg.exec();
	} else if (passwordInput->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Password is empty");
		mymsg.exec();
	} else if (usernameInput->text().isEmpty() && passwordInput->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Username and password are empty");
		mymsg.exec();
	} else {
		string username = usernameInput->text().toStdString();
		map<string, User*> users = data->showUsers();
		map<string, User*>::iterator it = users.find(username);
		if (it == users.end()) {
			QMessageBox mymsg(this);
			mymsg.setText("Invalid Username");
			mymsg.exec();
			usernameInput->setText("");
			passwordInput->setText("");
		} else {
			unsigned long long encript = hashFunction(0);
			User* valid = it->second;
			unsigned long long correctPassword = valid->getPassword();
			if (encript == correctPassword) {
				MainWindow* main = new MainWindow(data, username);
				main->show();
				this->close();
			} else {
				QMessageBox mymsg(this);
				mymsg.setText("Incorrect Password");
				mymsg.exec();
				usernameInput->setText("");
				passwordInput->setText("");
			}
		}
	}
}

void LoginWindow::create() {
	if (newUserInput->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Username is empty");
		mymsg.exec();
	} else if (newPasswordInput->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Password is empty");
		mymsg.exec();
	} else if (newUserInput->text().isEmpty() && newPasswordInput ->text().isEmpty()) {
		QMessageBox mymsg(this);
		mymsg.setText("Username and password are empty");
		mymsg.exec();
	} else {
		string username = newUserInput->text().toStdString();
		for (unsigned int i = 0; i < username.length(); i++) {
			if (username[i] == ' ') {
				QMessageBox mymsg(this);
				mymsg.setText("Username can't have space in between");
				mymsg.exec();
				newUserInput->setText("");
				newPasswordInput->setText("");
			}
		}
		map<string, User*> users = data->showUsers();
		map<string, User*>::iterator it = users.find(username);
		if (it != users.end()) {
			QMessageBox mymsg(this);
			mymsg.setText("Username already exists");
			mymsg.exec();
			newUserInput->setText("");
			newPasswordInput->setText("");
		} else {
			string newpassword = newPasswordInput->text().toStdString();
			for (unsigned int i = 0; i < newpassword.length(); i++) {
				if (newpassword[i] == ' ') {
					QMessageBox mymsg(this);
					mymsg.setText("Password can't have space in between");
					mymsg.exec();
					newUserInput->setText("");
					newPasswordInput->setText("");
				}
			}
			if (newpassword.length() > 8) {
				QMessageBox mymsg(this);
				mymsg.setText("Pasword can't be more than 8 characters");
				mymsg.exec();
				newUserInput->setText("");
				newPasswordInput->setText("");
			}else {
				unsigned long long encript = hashFunction(1);
				User* newUser = new User(username, 100.0, 0, encript);
				data->addUser(newUser);
				MainWindow* main = new MainWindow(data, username);
				main->show();
				this->close();
			}
		}
	}

}