#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>


/** Complete the setIntersection and setUnion functions below
 *  in this header file (since they are templates  
 */
template <typename T>
std::set<T> setIntersection(std::set<T>& s1, std::set<T>& s2)
{
	std::set<T> intersect;
	typename std::set<T>::iterator it1;
	typename std::set<T>::iterator it2;
	for (it1 = s1.begin(); it1 != s1.end(); ++it1) {
		for (it2 = s2.begin(); it2 != s2.end(); ++it2) {
			if (*it1 == *it2) {
				intersect.insert(*it1);
			}
		}
	}
	//std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(intersect));
	return intersect;
}
template <typename T>
std::set<T> setUnion(std::set<T>& s1, std::set<T>& s2)
{
	std::set<T> s;
	typename std::set<T>::iterator it1;
	typename std::set<T>::iterator it2;
	for (it1 = s1.begin(); it1 != s1.end(); ++it1) {
		s.insert(*it1);
	}
	for (it2 = s2.begin(); it2 != s2.end(); ++it2) {
		s.insert(*it2);
	}
	//std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(s));
	return s;
}

/***********************************************/
/* Prototypes of functions defined in util.cpp */
/***********************************************/

std::string convToLower(std::string src);

std::set<std::string> parseStringToWords(std::string line);

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) ;

// trim from end
std::string &rtrim(std::string &s) ;

// trim from both ends
std::string &trim(std::string &s) ;
#endif
