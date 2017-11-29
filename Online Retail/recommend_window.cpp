#include "recommend_window.h"
#include <iostream>
#include <QString>

using namespace std;

recommendWindow::recommendWindow(vector<pair<string, double> > list) {
	list_ = list;
	setWindowTitle("Recommendation");
	overallLayout = new QVBoxLayout();
	recommend = new QLabel(QString::fromStdString("Recommendation"));
	close = new QPushButton("Done");
	list_product = new QListWidget();
	for (unsigned int i = 0; i < list_.size(); i++) {
		QString item;
		item += QString::number(list_[i].second, 'f', 2);
		item += QString::fromStdString("  ");
		item += QString::fromStdString(list_[i].first);
		list_product->addItem(item);
	}
	overallLayout->addWidget(recommend);
	overallLayout->addWidget(list_product);
	overallLayout->addWidget(close);
	connect(close, SIGNAL(clicked()), this, SLOT(close()));
	setLayout(overallLayout);
}

recommendWindow::~recommendWindow() 
{
	delete recommend;
	delete close;
	delete list_product;
	delete overallLayout;
}