#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <string>
#include <vector>
#include "mydatastore.h"


class NewWindow : public QWidget {
	Q_OBJECT

	public:
		NewWindow(MyDataStore * ds, std::string name);
		~NewWindow();

	private slots:
		void buyItem();
		void removeItem();
	private:
		void initializeComponents();
		void displayCart();
		void connectEvents();
		QListWidget* list_review;
		QVBoxLayout* overallLayout;
		QHBoxLayout* layout;
		QPushButton* buyItemButton;
		QPushButton* removeItemButton;
		MyDataStore* data;
		std::string name_;
		
};

#endif