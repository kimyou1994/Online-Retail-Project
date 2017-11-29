#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include "product.h"
#include "user.h"
#include "datastore.h"

class MyDataStore : public DataStore {

public:
    MyDataStore();
    ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

    //check if the name given is valid
    bool valid(std::string name);

    //add the product into user's cart
    void addCart(std::string name, int index, std::vector<Product*> hits);

    //view cart of specific user by searching name
    void viewCart(std::string name);

    //buy cart and take out the items that users bought
    void buyCart(std::string name);

    void addReview(Review* r);

    void removeItem(std::string name, int index);
    std::map<std::string, User*> showUsers();

    std::map<std::string, std::vector<Review*> >showReview();
    std::map<std::string, Product*> showProduct();

//    std::vector<Product*> sort(std::vector<Product*>&, int n);
    std::queue<Product*> showCart(std::string name);
    double basicSimilarities(User* one, User* two);
    double refineSimilarityEstimates(User* one, User* two);
    std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
private:
    std::map<std::string, Product*> store_products;  //list of products
    std::map<std::string, User*> store_users;   //list of users
    std::map<std::string, std::set<Product*> > keyword_;    //keyword users put in
    std::map<std::string, std::queue<Product*> > cart_; //list of product in cart with user as a key
    std::map<std::string, std::vector<Review*> > review_;  //list of reviews in a product
    std::map<std::string, std::vector<std::string> > reviewOwner; //user reviewed these products
};

#endif
