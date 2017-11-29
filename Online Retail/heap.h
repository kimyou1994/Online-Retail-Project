#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

private:
    /// Add whatever helper functions you need below

    void trickleUp(int loc);
    void trickleDown(int loc);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    std::pair<double, T> newItem(priority, item);
    store_.push_back(newItem);
    T item_ = store_[store_.size()-1].second;
    std::pair<T, int> newItem_ (item_, store_.size()-1);
    keyToLocation_.insert(newItem_);
    trickleUp(store_.size() - 1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(int loc)
{
    std::cout << "location " <<  loc << std::endl;
    if (loc == 0) {
        return;
    }

    std::cout << "--------------------" << std::endl;
    int parent = (loc - 1)/2;
    std::cout << "parent " << parent << std::endl;
    if (c_(store_[loc].first, store_[parent].first)) {
        std::pair<double, T> temp = store_[parent];
        store_[parent] = store_[loc];
        store_[loc] = temp;
        auto current = keyToLocation_.find(store_[loc].second);
        auto parent_index = keyToLocation_.find(store_[parent].second);
        current->second = parent;
        parent_index->second = loc;
        trickleUp(parent);
        return;
    }

    /*int parent = (loc - 1)/2;
    while (parent >= 0 && c_(store_[loc].first, store_[parent].first)) {
        std::pair<double, T> temp = store_[parent];
        store_[parent] = store_[loc];
        store_[loc] = temp;
        loc = parent;
        parent = (loc - 1)/2;
    }   
    T item = store_[loc].second;
    std::pair<T, int> newItem (item, loc);
    keyToLocation_.insert(newItem);*/
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    std::cout << "dcecrease key" << std::endl;
    typename std::unordered_map<T, size_t, Hasher, TComparator>::iterator value = keyToLocation_.find(item);
    int index = value->second;
    store_[index].first = priority;
    trickleUp(index);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    } else {
        std::pair<double, T> temp = store_[0];
        T& value = temp.second;
        return value;
    }

}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    } else {
        T item = store_[0].second;
        keyToLocation_.erase(item);
        store_[0] = store_[store_.size() - 1];
        store_.pop_back();
        trickleDown(0);
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleDown(int loc)
{
    if (store_.size() > 1) {
        if (loc * m_ > store_.size() - 1) {
            return;
        }
        int smallerChild = loc * m_ + 1;
        if (smallerChild < store_.size()) {
            int rChild = smallerChild + 1;
            if (c_(store_[rChild].first, store_[smallerChild].first)) 
                smallerChild = rChild;
        }
        if (!c_(store_[loc].first, store_[smallerChild].first)) {
            std::pair<double, T> temp = store_[smallerChild];
            store_[smallerChild] = store_[loc];
            store_[loc] = temp;
            auto current = keyToLocation_.find(store_[loc].second);
            auto child_index = keyToLocation_.find(store_[smallerChild].second);
            current->second = smallerChild;
            child_index->second = loc;
            trickleDown(smallerChild);
        }
    }
}


/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif
