#include "main_window.h"
#include "msort.h"
#include "review.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFont>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "util.h"
#include <functional>
#include <cmath>
#include <iomanip>
#include "heap.h"
#include "recommend_window.h"


using namespace std;
struct AlphaStrComp {
	bool operator()(Product* lhs, Product* rhs)
	{
		return lhs->getName() < rhs->getName();
	}
};

struct RateComp {
	RateComp(MyDataStore* ds) : datastore(ds) {

	}

	bool operator()(Product* lhs, Product* rhs)
	{
		string left = lhs->getName();
		string right = rhs->getName();
		double left_average = 0.0;
		double right_average = 0.0;
		map<string, vector<Review*> > review_map = datastore->showReview();
		map<string, vector<Review*> >::iterator mit1 = review_map.find(left);
		map<string, vector<Review*> >::iterator mit2 = review_map.find(right);
		if (mit1 != review_map.end()) {
			for (vector<Review*>::iterator it = mit1->second.begin(); it != mit1->second.end(); ++it) {
				left_average += (double)(*it)->rating;
			}
			left_average = left_average/(unsigned)mit1->second.size();
		}
		if (mit2 != review_map.end()) {
			for (vector<Review*>::iterator it = mit2->second.begin(); it != mit2->second.end(); ++it) {
				right_average += (double)(*it)->rating;
			}
			right_average = right_average/(unsigned)mit2->second.size();
		}
		return left_average > right_average;
	}
	MyDataStore* datastore;
};

struct DateComp {
	bool operator()(Review* lhs, Review* rhs)
	{
		vector<string> left, right;
		string date1 = lhs->date;
		string date2 = rhs->date;
		string temp;
		stringstream ss1(date1);
		while(getline(ss1, temp, '-')) {
			left.push_back(temp);
		}
		stringstream ss2(date2);
		while(getline(ss2, temp, '-')) {
			right.push_back(temp);
		}
		if (left[0] != right[0])
			return left[0] > right[0];
		else if (left[1] != right[1])
			return left[1] > right[1];
		else
			return left[2] > right[2];
	}
};

struct dijkstraAlgorithm
{
	bool operator() (double left, double right) {
		return left < right;
	}
};

MainWindow::MainWindow(MyDataStore* ds, string username) {
	data = ds;
	user_name = username;
	setWindowTitle("Search Engine");
	// Initialize all components for this widget.
	initializeComponents();
	// Arrange the components into the layout, which is how they appear in the application.
	horizontalLayout->addLayout(layout);
	horizontalLayout->addWidget(list_product);
	horizontalLayout->addLayout(second_column);
	horizontalLayout->addWidget(reviews_);
	overallLayout->addLayout(horizontalLayout);
	overallLayout->addLayout(review_box);
	overallLayout->addWidget(recommendButton);
	setupLayout();
	setupOption();
	setupSort();
	setupUserList();
	setupDate();
	setLayout(overallLayout);
	connectEvents();
	window_opened = false;
	product_showed = false;
}

MainWindow::~MainWindow() {
	delete recommendButton;
	delete list_product;
	delete searchLabel;
	delete searchInput;
	delete search_option;
	delete viewCartButton;
	delete addCartButton;
	delete searchButton;
	delete sort_option;
	delete sort_box; 
	delete sort_alpha;
	delete sort_rate;
	delete searchButton;
	delete user_list;
	delete reviews_;
	delete saveButton;
	delete second_column;
	delete quitButton;
	delete userLabel;
	delete review_box;
	delete reviewLabel;
	delete write_review;
	delete year;
	delete month;
	delete day;
	delete rating;
	delete rate;
	delete summit_review;
	delete layout;
	delete horizontalLayout;
	delete overallLayout;
}

void MainWindow::initializeComponents()
{
	horizontalLayout = new QHBoxLayout();
	layout = new QVBoxLayout();
	list_product = new QListWidget();
	searchLabel = new QLabel(QString::fromStdString("Search Words: "));
	searchInput = new QLineEdit();
	search_option = new QGroupBox(tr("Search with OR or AND"));
	OR = new QRadioButton(tr("OR"));
	AND = new QRadioButton(tr("AND"));
	option_box = new QVBoxLayout();
	addCartButton = new QPushButton("Add to Cart");
	viewCartButton = new QPushButton("View Cart");
	sort_box = new QVBoxLayout();
	sort_option = new QGroupBox(tr("Sort by"));
	sort_alpha = new QRadioButton(tr("Alphabetical"));
	sort_rate = new QRadioButton(tr("Rating"));
	searchButton = new QPushButton("Search");
	user_list = new QComboBox();
	reviews_ = new QTextEdit();
	saveButton = new QPushButton("Save");
	second_column = new QVBoxLayout();
	quitButton = new QPushButton("Quit");
	userLabel = new QLabel(QString::fromStdString("Users: "));
	review_box = new QVBoxLayout();
	reviewLabel = new QLabel(QString::fromStdString("Write a Review: "));
	write_review = new QTextEdit();
	overallLayout = new QVBoxLayout();
	review_date = new QHBoxLayout();
	year = new QComboBox();
	month = new QComboBox();
	day = new QComboBox();
	date = new QLabel(QString::fromStdString("Date: "));
	rating = new QLabel(QString::fromStdString("Rate: "));
	rate = new QComboBox();
	summit_review = new QPushButton("Summit");
	recommendButton = new QPushButton("Product Recommendations");
}

void MainWindow::setupLayout()
{	
	QFont f( "Arial", 16, QFont::Bold);
	searchLabel->setFont(f);
	userLabel->setFont(f);
	layout->addWidget(searchLabel);
	layout->addWidget(searchInput);
	layout->addWidget(search_option);
	layout->addWidget(sort_option);
	layout->addWidget(searchButton);
	layout->addWidget(addCartButton);
	layout->addWidget(viewCartButton);
	second_column->addWidget(userLabel);
	second_column->addWidget(user_list);
	second_column->addStretch(1);
	second_column->addWidget(saveButton);
	second_column->addWidget(quitButton);
	review_box->addWidget(reviewLabel);
	review_box->addWidget(write_review);
	review_box->addLayout(review_date);
	review_date->addWidget(date);
	review_date->addWidget(year);
	review_date->addWidget(month);
	review_date->addWidget(day);
	review_date->addStretch(1);
	review_date->addWidget(rating);
	review_date->addWidget(rate);
	review_date->addStretch(1);
	review_date->addWidget(summit_review);

}

void MainWindow::setupOption()
{
	OR->setChecked(true);
	option_box->addWidget(OR);
	option_box->addWidget(AND);
	option_box->addStretch(1);
	search_option->setLayout(option_box);
}

void MainWindow::setupSort()
{
	sort_alpha->setChecked(true);
	sort_box->addWidget(sort_alpha);
	sort_box->addWidget(sort_rate);
	sort_box->addStretch(1);
	sort_option->setLayout(sort_box);
}
void MainWindow::connectEvents()
{
	connect(addCartButton, SIGNAL(clicked()), this, SLOT(addCart()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
	connect(user_list, SIGNAL(currentIndexChanged(int)), this, SLOT(changeUser()));
	connect(viewCartButton, SIGNAL(clicked()), this, SLOT(openSecondWindow()));
	connect(list_product, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setupReview(QListWidgetItem*)));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quitWindow()));
	connect(summit_review, SIGNAL(clicked()), this, SLOT(addReview()));
	connect(recommendButton, SIGNAL(clicked()), this, SLOT(recommendProducts()));
}

void MainWindow::search() 
{
	list_product->clear();

	if (searchInput->text().isEmpty()) 
		return;
	vector<Product*> list;
	vector<string> search_words;
	string line = searchInput->text().toStdString();
	istringstream iss(line);
	string term;
	while(iss >> term) {
		search_words.push_back(term);
	}

	if (OR->isChecked()) 
		list = data->search(search_words, 1);
	else if (AND->isChecked()) 
		list = data->search(search_words, 0);
	if (sort_alpha->isChecked())
	{
		sort(list, 1);
	}
	else if (sort_rate->isChecked())
	{
		sort(list, 0);
	}
	for (vector<Product*>::iterator it = list.begin(); it != list.end(); ++it) {
		QString item;
		item += QString::fromStdString((*it)->getName());
		item += QString::fromStdString("\n");
		item += QString::number((*it)->getPrice());
		list_product->addItem(item);
	}
	list_product->setCurrentRow(0);
	product_showed = true;
	products = list;
}

void MainWindow::setupUserList() 
{
	user_list->addItem(QString::fromStdString(user_name));
	/*map<string, User*> list = data->showUsers();
	for (map<string, User*>::iterator it = list.begin(); it != list.end(); ++it) {
		string name = it->second->getName();
		user_list->addItem(QString::fromStdString(name));
	}
	user_list->addItem("Type Username");*/
}

void MainWindow::setupDate()
{
	for (int i = 0; i < 15; i++) {
		stringstream ss;
		ss << (2016 + i);
		string number = ss.str();
		year->addItem(QString::fromStdString(number));
	}
	for (int i = 0; i < 12; i++) {
		stringstream ss;
		ss << (1 + i);
		string number = ss.str();
		month->addItem(QString::fromStdString(number));
	}
	for (int i = 0; i < 31; i++) {
		stringstream ss;
		ss << (1 + i);
		string number = ss.str();
		day->addItem(QString::fromStdString(number));
	}
	rate->addItem("1");
	rate->addItem("2");
	rate->addItem("3");
	rate->addItem("4");
	rate->addItem("5");
}

void MainWindow::setupReview(QListWidgetItem* item)
{
	reviews_->clear();
	QString info = item->text();
	QStringList split = info.split(QRegExp("\n|\r\n|\r"));
	QStringList::iterator qit = split.begin();
	QString name = (*qit);
	product_name = name.toStdString();
	map<string, vector<Review*> > review_map = data->showReview();
	map<string, vector<Review*> >::iterator mit = review_map.find(name.toStdString());
	reviews_->setText("");
	if (mit != review_map.end()) {
		vector<Review*> list = mit->second;
		if (list.size() > 0 ){
			DateComp comp;
			mergeSort(list, comp);
			for (vector<Review*>::iterator it = list.begin(); it != list.end(); ++it) {
				stringstream review;
				review << (*it)->username << "\n" << "Rating: " << (*it)->rating << "   " << (*it)->date 
							<< "\n" << (*it)->reviewText << "\n";
				string result = review.str();
				reviews_->append(QString::fromStdString(result));
			}
		}
	}
	reviews_->setReadOnly(true);
}

void MainWindow::changeUser()
{
	if (user_list->currentText() == "Type Username")
		user_list->setEditable(true);
	else 
		user_list->setEditable(false);
}

void MainWindow::addCart()
{
	if (product_showed) {
		string name = user_name;// user_list->currentText().toStdString();
		int index = list_product->currentRow() + 1;
		data->addCart(name, index, products);
		queue<Product*> cart = data->showCart(name);
		QMessageBox::information(this, "Add Cart", "Added to Cart");
	}
}
void MainWindow::openSecondWindow()
{
	if (window_opened) 	//first time opened
		delete newWindow_;
	string name = user_name;
	newWindow_ = new NewWindow(data, name);
	newWindow_->show();
	window_opened = true;
}

void MainWindow::sort(vector<Product*>& list, int n) 
{
	if (n == 1) {
		AlphaStrComp comp;
		mergeSort(list, comp);
	}
	else {
		RateComp comp(data);
		mergeSort(list, comp);
	}
}

void MainWindow::saveToFile() 
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Information"),
						 "", tr("Plain Text (*.txt);;All File (*)"));
	if (filename.isEmpty()) 
		QMessageBox::information(this, "Error", "Insert filename");
	else {
		QFile file(filename);
		string name = filename.toStdString();
		ofstream ofile(name.c_str());
		data->dump(ofile);
		ofile.close();
	}
}

void MainWindow::addReview()
{
	QString date = year->currentText() + "-" + month->currentText() + "-" + day->currentText();
	string date_string = date.toStdString();
	string review_str = write_review->toPlainText().toStdString();
	string rate_ = (rate->currentText()).toStdString();
	//user_name = user_list->currentText().toStdString();
	int rating;
	if (rate_.compare("1") == 0) rating = 1;
	else if (rate_.compare("2") == 0)  rating = 2;
	else if (rate_.compare("3") == 0)  rating  = 3;
	else if (rate_.compare("4") == 0)  rating = 4;
	else rating = 5;
	Review* new_review = new Review(product_name, rating, user_name, date_string, review_str);
	data->addReview(new_review);
	QMessageBox::information(this, "Add Review", "Review Added");
	setupReview(list_product->currentItem());
}

void MainWindow::recommendProducts()
{	
	vector<pair<string, double> > products = data->makeSuggestion(user_name);
	string name = "rec.txt";
	ofstream ofile(name.c_str());
	if (products.size() == 0) {
		QMessageBox mymsg(this);
		mymsg.setText("No recommendation available");
		mymsg.exec();
		ofile << "No recommendation available";
	} else {
		recommendWindow* suggest = new recommendWindow(products);
		suggest->show();
		ofile << user_name << "\n";
		for (unsigned i = 0; i < products.size(); i++) {
			double rate = products[i].second;
			ofile << setprecision(2) << fixed << rate << "  " << products[i].first << "\n";
		}
	}
	ofile.close();
}

void MainWindow::quitWindow() 
{
	close();
}
