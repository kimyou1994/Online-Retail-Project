#include "SkipList.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
int main() {

    // 1.Initialize an empty Skip_list object
    std::less<int> less_than;
    SkipList<int, string, less<int> >  s(less_than);

    // 2. insert()
    for (int i = 0; i < 10; ++i) {
        std::stringstream ss;
        ss << i;
        pair<int, string> newPair(i, ss.str());
        s.insert(newPair);
    }
    // 3. find()     
    for (int i = 0; i < 10; ++i) {
        std::pair<const int, string>* f = s.find(i);
        if (f) std::cout << "Node found!\nvalue: " << f << '\n';
        else std::cout << "Node NOT found!\n";
    }  

    // 4. insert() - reassign
    s.insert(pair<int, string>(5, "TEST"));

    // 5. erase()
    s.erase(8);
    for (int i = 0; i < 10; ++i) {
        std::pair<const int, string>* f = s.find(i);
        if (f) std::cout << "Node found!\nvalue: " << f << '\n';
        else std::cout << "Node NOT found!\n";
    }  
    s.erase(4);
    //s.erase(1);

    for (int i = 0; i < 10; ++i) {
        cout << "i is " << i << endl;
        std::pair<const int, string>* f = s.find(i);
        if (f) std::cout << "Node found!\nvalue: " << f << '\n';
        else std::cout << "Node NOT found!\n";
    }  
    //6. empty()
    if (!s.empty())
        cout  <<  "not empty" << endl;

    //7. size = 9
    cout << s.size() << endl;

    cout << "\nDone!\n";
    return 0;
}