#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;

Movie::Movie(const std::string category, const std::string name, double price, int qty, 
	const std::string Genre, const std::string Rating) : Product(category, name, price, qty) 
{	
	genre_ = Genre;
	rating_ = Rating;
}

Movie::~Movie()
{

}

set<string> Movie::keywords() const 
{
	std::set<std::string> name = parseStringToWords(name_);
	std::set<std::string> genre = parseStringToWords(genre_);
	std::set<std::string> keyword = setUnion(name, genre);
	keyword.insert(rating_);
	return keyword;
}

double Movie::getPrice() const
{
    return price_;
}

std::string Movie::getName() const
{
    return name_;
}

void Movie::subtractQty(int num)
{
    qty_ -= num;
}

int Movie::getQty() const
{
    return qty_;
}


string Movie::displayString() const
{
	stringstream info;
	info << name_ << "\n" << "Genre: " << genre_ << " Rating: " << rating_ << "\n" <<
		price_ << " " << qty_ << " left." << endl;
	return info.str();
}

void Movie::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << 
    	"\n" << genre_ << "\n" << rating_ << endl;
}