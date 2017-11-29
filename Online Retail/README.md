Name: Young Jin Kim

USC ID: 8977310189

Email: kim687@usc.edu

For compiler: call qmake -project, then you need to include  
CONFIG += DEBUG
QMAKE_CXXFLAGS += -std=c++11 in .pro file to compile.
Next, do qmake, then make
then, run the program by calling ./hw8 <filename>

For extra credit I did Skiplist which is SkipList.h.
I made the test case to see if all the function works properly
To compile do g++ -g -Wall --std=c++11 skiplist_test.cpp -o test
and run ./test
When I run it it seems all function works except the erase function. it somehow mess up the list and delete not only one elements but
sometimes it delete multiple. Just turning in for partial.

If there is skiplist_test.cpp is in same folder as homework 8 materials qmake will be messed up you need to get rid of the skiplist_test.cpp 
to run hw8.