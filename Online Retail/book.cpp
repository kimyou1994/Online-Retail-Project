#include <sstream>
#include <iomanip>
#include "book.h"

using namespace std;

Book::Book(const std::string category, const std::string name, double price, int qty, 
	const std::string ISBN, const std::string Author): Product(category, name, price, qty) 
{
	isbn_ = ISBN;
	author_ = Author;
}

Book::~Book()
{
}

set<string> Book::keywords() const 
{
	std::set<std::string> name = parseStringToWords(name_);
	std::set<std::string> author = parseStringToWords(author_);
	std::set<std::string> keyword = setUnion(name, author);
	keyword.insert(isbn_);
	return keyword;
}

double Book::getPrice() const
{
    return price_;
}

std::string Book::getName() const
{
    return name_;
}

void Book::subtractQty(int num)
{
    qty_ -= num;
}

int Book::getQty() const
{
    return qty_;
}

string Book::displayString() const
{
	stringstream info;
	info << name_ << "\n" <<  "Author: " << author_ << " ISBN: " << isbn_ << "\n" <<
		price_ << " " << qty_ << " left." << endl;
	return info.str();
}

void Book::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << 
    	"\n" << isbn_ << "\n" << author_ << endl;
}