#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <limits>

using namespace std;

template <typename K, typename V, typename Comp = std::less<K> >
class SkipList // or SplayTree or LSM
{
public:
    SkipList(const Comp& c = Comp());
    ~SkipList();
    // If the key is already present, update the value
    void insert(const std::pair<const K,V>& item); 
    // Return NULL if the key doesn't exist in the map
    pair<const K,V>* find(const K& key); 
    // Do nothing if the key does not exist
    void erase(const K& key); 
    size_t size() const;
    bool empty() const;

private:
    struct SkipNode{
        K key;
        V value;
        SkipNode** forward; //array of ptrs
        SkipNode(K& k, V& v, int level){
            key = k; value = v;
            forward = new SkipNode*[level + 1];
        } 
    };
    int randomLevel();
    SkipNode* head;
    SkipNode* end;

    double probability;
    int maxLevel;
    int size_;
    Comp c_;

};

template <typename K, typename V, typename Comp>
SkipList<K, V, Comp>::SkipList(const Comp& c) :probability(0.5), maxLevel(20), size_(0), c_(c)
{
    K min_key = numeric_limits<K>::min();
    V min_value = numeric_limits<V>::min();
    head = new SkipNode(min_key, min_value, maxLevel);
    K max_key = numeric_limits<K>::max();
    V max_value = numeric_limits<V>::max();
    end = new SkipNode(max_key, max_value, maxLevel);
    for (int i = 0; i <= maxLevel; i++) {
        head->forward[i] = end;
    }

}

template <typename K, typename V, typename Comp>
SkipList<K, V, Comp>::~SkipList() 
{
    SkipNode* node = head;
    while (node->forward[0]) {
        SkipNode* temp = node;
        node = node->forward[0];
        delete temp;
    }
    delete node;
}

template <typename K, typename V, typename Comp>
int SkipList<K, V, Comp>::randomLevel()
{
    int height = 1;
    while (((double)std::rand() / RAND_MAX) < probability && height < maxLevel) {
        height++;
    }
    return height;

}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::insert(const std::pair<const K,V>& item)
{
    K key_ = item.first;
    V value_ = item.second;
    SkipNode* current = head;
    vector<SkipNode*> update(maxLevel + 1);
    for (int i = maxLevel; i >= 0; i--) {
        while( c_(current->forward[i]->key, key_)) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    SkipNode* next = current->forward[0];
    if (next->key == key_ && next != end) {
        next->value = value_;
        return;
    } else {
        int height =  randomLevel();
        SkipNode* newNode = new SkipNode(key_, value_, height);
        for (int i = 0; i < height; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
    size_++;
}

template <typename K, typename V, typename Comp>
pair<const K, V>* SkipList<K, V, Comp>::find(const K& key)
{
    SkipNode* current = head;
    for (int i = maxLevel; i >= 0; i--) {
        while (c_(current->forward[i]->key, key)) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current->key == key) {
        pair<const K, V> point =  make_pair(key, current->value);
        pair<const K, V> *newPair = &point;
        return newPair;
    } else {
        pair<const K, V>* newPair{};
        return newPair;
    }
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::erase(const K& key)
{
    SkipNode* current = head;
    vector<SkipNode*> update(maxLevel + 1);
    for (int i = maxLevel; i >= 0; i--) {
        while(current->forward[0] && c_(current->forward[i]->key, key)) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    SkipNode* look = current->forward[0];
    if (look->key != key) {
        return;
    }
    for (int i  = 0; i <= maxLevel; i++) {
        update[i] = look->forward[i];

    }
    size_--;
    delete look;
}

template <typename K, typename V, typename Comp>
size_t SkipList<K, V, Comp>::size() const 
{
    return size_;
}

template <typename K, typename V, typename Comp>
bool SkipList<K, V, Comp>::empty() const
{
    return size_ == 0;
}

#endif