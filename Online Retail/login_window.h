#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "mydatastore.h"
#include "main_window.h"


class LoginWindow :  public QWidget {
	Q_OBJECT

	public:
		LoginWindow(MyDataStore* ds);
		~LoginWindow();

	private slots:
		void login();
		void create();

	private:
		void initializeComponents();
		void setupLayout();
		void connectEvents();
		unsigned long long hashFunction(int n);
		MainWindow* mainWindow;
		MyDataStore* data;
		QHBoxLayout* overallLayout;
		QVBoxLayout* layout;
		QHBoxLayout* usernameLayout;
		QHBoxLayout* passwordLayout;
		QLabel* signInLabel;
		QLabel* usernameLabel;
		QLineEdit* usernameInput;
		QLabel* passwordLabel;
		QLineEdit* passwordInput;
		QPushButton* loginButton;
		
		QVBoxLayout* createUserLayout;
		QHBoxLayout* newUserLayout;
		QHBoxLayout* newPasswordLayout;
		QLabel* createLabel;
		QLabel* newUserLabel;
		QLineEdit* newUserInput;
		QLabel* newPasswordLabel;
		QLineEdit* newPasswordInput;
		QPushButton* createButton;
		QPushButton* quitButton;

};

#endif