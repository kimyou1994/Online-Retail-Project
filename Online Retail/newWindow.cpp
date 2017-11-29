#include "newWindow.h"
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>
#include <sstream>
#include <string>
#include <queue>

using namespace std;

NewWindow::NewWindow(MyDataStore* ds, string name)
{
	name_ = name;
	data = ds;
	setWindowTitle("User's Cart");
	initializeComponents();
	overallLayout->addWidget(list_review);
	layout->addWidget(buyItemButton);
	layout->addWidget(removeItemButton);
	overallLayout->addLayout(layout);
	setLayout(overallLayout);
	displayCart();
	connectEvents();
}

NewWindow::~NewWindow() {
	delete buyItemButton;
	delete removeItemButton;
	delete layout;
	delete overallLayout;
}

void NewWindow::initializeComponents()
{
	list_review = new QListWidget();
	overallLayout = new QVBoxLayout();
	layout = new QHBoxLayout();
	buyItemButton = new QPushButton("Buy this Product");
	removeItemButton = new QPushButton("Remove this Product");
}

void NewWindow::displayCart()
{
	queue<Product*> reviews = data->showCart(name_);
	for (unsigned int i = 0; i < reviews.size(); i++) {	//view through the cart and print each of product
		QString item;
		Product* temp = reviews.front();
		reviews.pop();
		item += QString::fromStdString(temp->getName());
		item += QString::fromStdString("\n");
		item += QString::number(temp->getPrice());
		list_review->addItem(item);
		reviews.push(temp);
	}	
}

void NewWindow::connectEvents()
{
	connect(buyItemButton, SIGNAL(clicked()), this, SLOT(buyItem()));
	connect(removeItemButton, SIGNAL(clicked()), this, SLOT(removeItem()));
}

void NewWindow::buyItem()
{
	data->buyCart(name_);
	list_review->clear();
	displayCart();
}

void NewWindow::removeItem()
{
	int index = list_review->currentRow() ;
	data->removeItem(name_, index);
	list_review->clear();
	displayCart();
	QMessageBox::information(this, "Item removed", "Removed this Item");
}