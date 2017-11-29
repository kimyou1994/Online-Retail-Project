#include <iostream>
#include "mydatastore.h"
#include "util.h"
#include "msort.h"
#include <functional>
#include <string>
#include <cmath>
#include "heap.h"
#include <vector>

using namespace std;
struct dijkstraAlgorithm
{
	bool operator() (double left, double right) {
		return left < right;
	}
};

struct RatingComp {
	bool operator()(pair<string, double> lhs, pair<string, double> rhs)
	{
		if (lhs.second == rhs.second) 
			return lhs.first.length() > rhs.first.length();
		return lhs.second > rhs.second;
	}
};

MyDataStore::MyDataStore(): DataStore() {

}

//Delete the product information and user information
MyDataStore::~MyDataStore() {
	for (map<string, Product*>::iterator mit = store_products.begin(); mit!=  store_products.end(); ++mit) {
		delete mit->second;
	}
	for (map<string, User*>::iterator it = store_users.begin(); it!= store_users.end(); ++it) {
		delete it->second;
	}
}

//Add the product in a map of product to keep track of list of product and include product 
//into any keywords that is relevant
void MyDataStore::addProduct(Product* p) {
	string name = p->getName();
	set<string> keywords = p->keywords();
	store_products.insert(make_pair(name, p));
	for (set<string>::iterator it = keywords.begin(); it != keywords.end(); ++it) {
		string lower = convToLower(*it);
		map<string, set<Product*> >::iterator mit = keyword_.find(lower);
		if (mit != keyword_.end()) {
			mit->second.insert(p);
		} else {
			set<Product*> create;
			create.insert(p);
			keyword_.insert(make_pair(lower, create));
		}
	}
}

//Add users to map of users sorted by name of user
void MyDataStore::addUser(User* u) {
	store_users.insert(make_pair(u->getName(), u));
	queue<Product*> cart;
	cart_.insert(make_pair(u->getName(), cart));
}

void MyDataStore::addReview(Review* r) {
	string name = r->prodName;
	string owner = r->username;
	map<string, vector<Review*> >::iterator mit = review_.find(name);
	map<string, vector<string> >::iterator mit2 = reviewOwner.find(owner);
	if (mit2 != reviewOwner.end()) {
		mit2->second.push_back(name);
	}else {
		vector<string> list;
		list.push_back(name);
		reviewOwner.insert(make_pair(owner, list));
	}
	if (mit != review_.end()) {
		mit->second.push_back(r);
	}else {
		vector<Review*> create;
		create.push_back(r);
		review_.insert(make_pair(name, create));
	}
}

//Search the product with given terms and type
vector<Product*> MyDataStore::search(vector<string>& terms, int type) {
	vector<Product*> list;
	set<Product*> total;
	vector<string>::iterator it = terms.begin();
	unsigned int count = 0;
	if (terms.size() == 1) {
		string item = convToLower(*it);
		map<string, set<Product*> >::iterator mit = keyword_.find(item);
		if (mit != keyword_.end()) {
			total = mit->second;
		}
	}
	else {
		for (it = terms.begin(); it != terms.end(); ++it) {
			string item = convToLower(*it);
			map<string, set<Product*> >::iterator mit = keyword_.find(item);
			if (type == 1 && mit != keyword_.end()) {
				total = setUnion(total, mit->second );
			} 
			if (type == 0 && mit != keyword_.end()) {
				count++;
				if (total.size() == 0 && count == 1) {
					total = mit->second;
				}else {
					total = setIntersection(total, mit->second);
				}
			} 
		}
	}
	if (count != terms.size() && type == 0) //if interesection is not satisfied
		return vector<Product*>();	
	if (total.size() != 0)
		list.assign(total.begin(), total.end());
	return list;
}

//ostream into output files after all the calculation is done
void MyDataStore::dump(ostream& ofile) {
	ofile << "<products>" << "\n";
	for (map<string, Product*>::iterator it = store_products.begin(); it != store_products.end(); ++it) {
		it->second->dump(ofile);
	}
	ofile << "</products>" << "\n" << "<users>" << "\n";
	for (map<string, User*>::iterator itu = store_users.begin(); itu != store_users.end(); ++itu) {
		itu->second->dump(ofile);
	}
	ofile << "</users>" << "\n" << "<reviews>" << "\n";
	for (map<string, vector<Review*> >::iterator itrs = review_.begin(); itrs != review_.end(); ++itrs) {
		vector<Review*> list = itrs->second;
		for (vector<Review*>::iterator itr = list.begin(); itr != list.end(); ++itr) {
			ofile << (*itr)->prodName << "\n" << (*itr)->rating << " " << (*itr)->username << " "
				<< (*itr)->date << " " << (*itr)->reviewText << "\n";
		}
	}
	ofile << "</reviews>";

}

//Check if the user tpyed the valid name
bool MyDataStore::valid(string name) {
	map<string, User*>::iterator match = store_users.find(name);
	if (match == store_users.end()) {
		return false;
	}
	return true;
}

//Add product with given index to user's item 
void MyDataStore::addCart(string name, int index, vector<Product*> hits) {
	Product* item = hits[index - 1];
	map<string, queue<Product*> >::iterator mit = cart_.find(name);
	mit->second.push(item);
}

//View the user's cart by searching the name of user
void MyDataStore::viewCart(string name) {
	map<string, queue<Product*> >::iterator mit = cart_.find(name);
	queue<Product*> it = mit->second;
	for (unsigned int i = 0; i < it.size(); i++) {	//view through the cart and print each of product
		cout << "Item " << i + 1 << endl;
		Product* temp = it.front();
		it.pop();
		cout << temp->displayString() << endl;
		it.push(temp);
	}
}

//Buycart allows users to buy product if they have money and takes out product out of cart
void MyDataStore::buyCart(string name) {
	map<string, queue<Product*> >::iterator mit = cart_.find(name);
	map<string, User*>::iterator info = store_users.find(name);
	queue<Product*> it = mit->second;
	int size = it.size();
	double balance = info->second->getBalance();	//user's balance
	for (int i = 0; i < size; i++) {	//check if user can buy product with thier balance
		Product* temp = it.front();
		double cost = temp->getPrice();
		it.pop();
		if (balance >= cost) {	//available
			info->second->deductAmount(cost);
			balance -= cost;
			temp->subtractQty(1);
		} else {	//can't buy this product
			it.push(temp);
		}
	}
	if (mit != cart_.end()) {	//erase the product that users bought and insert new result cart
		cart_.erase(name);
		cart_.insert(make_pair(name, it));
	}
}

void MyDataStore::removeItem(string name, int index) {
	map<string, queue<Product*> >::iterator mit = cart_.find(name);
	queue<Product*> it = mit->second;
	for (unsigned int i = 0; i < it.size(); i++) {	//view through the cart and print each of product
		Product* temp = it.front();
		it.pop();
		if (i != (unsigned)index) {
			it.push(temp);
		}
	}
	cart_[name] = it;
}

map<string, User*> MyDataStore::showUsers() {
	return store_users;
}

map<string, vector<Review*> >MyDataStore::showReview() {
	return review_;
}

queue<Product*> MyDataStore::showCart(string name) {
	map<string, queue<Product*> >::iterator mit = cart_.find(name);
	return mit->second;
}

map<string, Product*> MyDataStore::showProduct() {
	return store_products;
}

double MyDataStore::basicSimilarities(User* one, User* two) {	
	double similar = 1.0;
	string nameOne = one->getName();
	string nameTwo = two->getName();
	set<Product*> userOne, userTwo;
	for (map<string, vector<Review*> >::iterator mit = review_.begin(); mit != review_.end(); ++mit) {
		vector<Review*> review = mit->second;
		for (vector<Review*>::iterator it = review.begin(); it != review.end(); ++it) {
			string user = (*it)->username;
			string productName = mit->first;
			map<string, Product*>::iterator mit2 = store_products.find(mit->first);
			if (nameOne.compare(user) == 0) {
				userOne.insert(mit2->second);
			} else if (nameTwo.compare(user) == 0) {
				userTwo.insert(mit2->second);
			}
		}
	}
	set<Product*> intersection = setIntersection(userOne, userTwo);
	if (intersection.size() == 0) {
		return similar;
	} else {
		for (set<Product*>::iterator it = intersection.begin(); it != intersection.end(); ++it) {
			double a = 0.0, b = 0.0;
			string name = (*it)->getName();
			map<string, vector<Review*> >::iterator it2 = review_.find(name);
			for (vector<Review*>::iterator rev = (it2->second).begin(); rev != (it2->second).end(); ++rev) {
				if ((*rev)->username == nameOne)
					a = (*rev)->rating;
				else if ((*rev)->username == nameTwo)
					b = (*rev)->rating;
			}
			similar = (double)abs(a - b)/4.0;
		}
	}	

	return similar;
}

double MyDataStore::refineSimilarityEstimates(User* one, User* two) {
	dijkstraAlgorithm comp;
	equal_to<string> equal;
	hash<string> hash_;
	Heap<string, equal_to<string>, dijkstraAlgorithm, hash<string> > heap(2, comp, hash_, equal);
	map<string, double> other;
	map<string, double> check;
	string root = one->getName();  
	heap.push(0.0, root);
	check.insert(make_pair(root, 0.0));
	string min;
	double refine = 1.0;
	for (map<string, User*>::iterator mit = store_users.begin(); mit != store_users.end(); ++mit) {
		if (mit->first != root){
			heap.push(1.0, mit->first);
			other.insert(make_pair(mit->first, 1.0));
		}
	}
	while(!heap.empty()) {
		string top = heap.top();
		min = top;
		map<string, double>::iterator user = other.find(top);
		double min_edge = user->second;
		check.insert(make_pair(min, min_edge));
		heap.pop();
		map<string, User*>::iterator min_user = store_users.find(min);
		for (map<string, double>::iterator it = other.begin(); it != other.end(); ++it) {
			string current = it->first;
			map<string, User*>::iterator cur_user = store_users.find(it->first);
			map<string, double>::iterator mit = check.find(current);
			if (mit == check.end()) {
				double basic = basicSimilarities(min_user->second, cur_user->second);
				map<string, double>::iterator other_user = other.find(current);
				double cur_edge = other_user->second;
				if (cur_edge > min_edge + basic) {
					it->second = min_edge + basic;
				}
				heap.decreaseKey(min_edge + basic, current);
			}
		}
	}
	for (map<string, double>::iterator mit = other.begin(); mit != other.end(); ++mit) {
		if (mit->first == two->getName()) {
			refine = mit->second;
		}
	}
	return refine;
}

vector<pair<string, double> > MyDataStore::makeSuggestion(string currentUser) {
	vector<pair<string, double> > value;
	vector<Product*> notReviewed;
	set<string> otherUser;
	User* current = store_users.find(currentUser)->second;
	map<string, vector<string> >::iterator mit = reviewOwner.find(currentUser);
	if (mit != reviewOwner.end()) {
		vector<string> list = mit->second;
		if (list.size() == store_products.size()) 
			return value;
		for (map<string, Product* >::iterator mit2 = store_products.begin(); mit2 != store_products.end(); ++mit2)  {
			vector<string>::iterator it = find(list.begin(), list.end(), mit2->first);
			if (it == list.end()) {
				Product* product = mit2->second;
				notReviewed.push_back(product);
			}
		}
	} else {
		for (map<string, Product* >::iterator mit2 = store_products.begin(); mit2 != store_products.end(); ++mit2)  {
			Product* product = mit2->second;
			notReviewed.push_back(product);
		}
	}
	for (unsigned int i = 0; i < notReviewed.size(); i++) {
		Product* product = notReviewed[i];
		string product_name = product->getName();
		map<string, vector<Review*> >::iterator rev = review_.find(product_name);
		if (rev != review_.end()) {
			vector<Review*> listofReviews = rev->second;
			double w = 0.0, rp = 0.0;
			for (vector<Review*>::iterator it = listofReviews.begin(); it != listofReviews.end(); ++it) {
				if ((*it)->username != currentUser) 
					otherUser.insert((*it)->username);
			}
			for (set<string>::iterator it = otherUser.begin(); it != otherUser.end(); ++it) {
				User* other = store_users.find(*it)->second;
				double similarity = refineSimilarityEstimates(current, other);
				map<string, vector<string> >::iterator reviewed = reviewOwner.find((*it));
				vector<string> reviewed_ = reviewed->second;
				int rate = 0;
				for (vector<Review*>::iterator it = listofReviews.begin(); it != listofReviews.end(); ++it) {
					if ((*it)->username == other->getName()) 
						rate = (*it)->rating; 
				}
				w += 1.0 - similarity;
				rp += (1.0 - similarity) * rate;
			}
			if (w == 0.0) {
				pair<string, double> connect (product_name, 0.0);
				value.push_back(connect);
			}
			else  {
				double recommend = rp/w;
				pair<string, double> connect (product_name, recommend);
				value.push_back(connect);
			}
		}
	}
	RatingComp comp;
	mergeSort(value, comp);
	return value;
}