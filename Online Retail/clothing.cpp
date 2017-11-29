#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;

Clothing::Clothing(const std::string category, const std::string name, double price, int qty, 
	const std::string size, const std::string Brand) : Product(category, name, price, qty) 
{	
	size_ = size;
	brand_ = Brand;
}

Clothing::~Clothing()
{

}

set<string> Clothing::keywords() const 
{
	std::set<std::string> name = parseStringToWords(name_);
	std::set<std::string> brand = parseStringToWords(brand_);
	std::set<std::string> keyword = setUnion(name, brand);
	keyword.insert(size_);
	return keyword;
}

double Clothing::getPrice() const
{
    return price_;
}

std::string Clothing::getName() const
{
    return name_;
}

void Clothing::subtractQty(int num)
{
    qty_ -= num;
}

int Clothing::getQty() const
{
    return qty_;
}

string Clothing::displayString() const
{
	stringstream info;
	info << name_ << "\n" << "Size: " << size_ << " Brand: " << brand_ << "\n"
		<< price_ << " " << qty_ << " left." << endl;
	return info.str();
}

void Clothing::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << 
    	"\n" << size_ << "\n" << brand_ << endl;
}