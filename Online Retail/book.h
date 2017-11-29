#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "product.h"
#include "util.h"

class Book : public Product {
public:
	Book(const std::string category_, const std::string name_, double price_, int qty,
		const std::string ISBN, const std::string Author);

	~Book();
	/**
     * Returns the appropriate keywords that this product should be associated with
     */
    std::set<std::string> keywords() const ;


    /**
     * Returns a string to display the product info for hits of the search
     */
    std::string displayString() const;

    /**
     * Outputs the product info in the database format
     */
    
    void dump(std::ostream& os) const;

    double getPrice() const;
    std::string getName() const;
    int getQty() const;
    void subtractQty(int num);
private:
	std::string isbn_;
	std::string author_;
};
#endif
