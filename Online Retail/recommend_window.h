#ifndef RECOMMEND_WINDOW_H
#define RECOMMEND_WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <vector>
#include <string>
#include "mydatastore.h"
#include "main_window.h"

class recommendWindow : public QWidget {
	Q_OBJECT
	public:
		recommendWindow(std::vector<std::pair<std::string, double> > list);
		~recommendWindow();
	private:
		QVBoxLayout* overallLayout;
		QLabel* recommend;
		QPushButton* close;
		QListWidget* list_product;
		std::vector<std::pair<std::string, double> > list_;
};

#endif