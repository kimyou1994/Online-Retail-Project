#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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
#include <QListWidgetItem>
#include <QTextEdit>
#include <string>
#include <vector>
#include "mydatastore.h"
#include "newWindow.h"

class MainWindow : public QWidget {
	Q_OBJECT

	//struct AlphaStrComp;
    //struct RateComp;
    //struct dateComp;
	public:
		MainWindow(MyDataStore * ds, std::string username);
		~MainWindow();

	private slots:
		void search();
		void addCart();
		void changeUser();
		void setupReview(QListWidgetItem* item);
		void openSecondWindow();
		void saveToFile();
		void quitWindow();
		void addReview();
		void recommendProducts();

	private:
		void initializeComponents();
		void setupLayout();
		void connectEvents();
		void setupOption();
		void setupSort();
		void setupUserList();
		void setupDate();
		void sort(std::vector<Product*>&, int n);
		NewWindow* newWindow_;
		QVBoxLayout* overallLayout;
		QHBoxLayout* horizontalLayout;
		QVBoxLayout* layout;
		QLabel* searchLabel;
		QLineEdit* searchInput;
		QGroupBox* search_option;
		QRadioButton* OR;
		QRadioButton* AND;
		QVBoxLayout* option_box;
		QListWidget* list_product;
		QPushButton* viewCartButton;
		QPushButton* addCartButton;
		QGroupBox* sort_option;
		QRadioButton* sort_alpha;
		QRadioButton* sort_rate;
		QVBoxLayout* sort_box;
		QPushButton* searchButton;
		QComboBox* user_list;
		QTextEdit* reviews_;
		QPushButton* saveButton;
		QVBoxLayout* second_column;
		QPushButton* quitButton;
		QLabel* userLabel;
		QVBoxLayout* review_box;
		QTextEdit* write_review;
		MyDataStore* data;
		QLabel* reviewLabel;
		QLabel* date;
		QLabel* rating;
		QComboBox* year;
		QComboBox* month;
		QComboBox* day;
		QPushButton* summit_review;
		QHBoxLayout* review_date;
		QComboBox* rate;
		QLabel* user;
		
		QPushButton* recommendButton;
		//std::vector<std::string> search_words;
		std::string user_name;
		std::vector<Product*> products;
		std::string product_name;
		bool window_opened;
		bool product_showed;
};

#endif