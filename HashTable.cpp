//
//  HashTable.cpp
//  Program 6
//
//  Created by Jody Spikes on 3/17/26.
//

#include <stdio.h>
#include "HashTable.h"

//------
//Insert
//------
template <typename Key, typename Value>
bool HashTable<Key, Value>::insert(const Key& key, const Value& value) {
    if(loadFactor() >= maxLoadFactor_){
        rehash();
    }
    
    size_t idx = bucketIndex(key);
    
    size_t pos = findEntryPosition(idx, key);
    
    if(pos != buckets_[idx].size()){
        buckets_[idx][pos].value = value;
        return false; //no key update
    }
    
    if(!buckets_[idx].empty()){
        ++collisionCount_;
    }
    
    buckets_[idx].emplace_back(key, value);
    ++elementCount_;
    return true;
    
}

//--------
//Contains
//--------
    template <typename Key, typename Value>
    bool HashTable<Key, Value>::contains(const Key& key) const {
        size_t idx = bucketIndex(key);
        return findEntryPosition(idx, key) != buckets_[idx].size();
    }
//
//    bool remove(const Key& key) {}

//----------------
//Find (not const)
//----------------
template <typename Key, typename Value>
Value* HashTable<Key, Value>::find(const Key& key) {
    size_t idx = bucketIndex(key);
    size_t pos = findEntryPosition(idx, key);
    if(pos == buckets_[idx].size()) {
        return nullptr;
    }
    return &buckets_[idx][pos].value;
}
//
//    const Value* find(const Key& key) const {}
//
//    Value& at(const Key& key) {}
//
//    const Value& at(const Key& key) const {}
//
//    void rehash() {}
//
//    size_t size() const {}
//
//    size_t tableSize() const {}
//
//    double loadFactor() const {}
//
//    size_t collisionCount() const {}
//
//    size_t rehashCount() const {}
//
//    vector<Key> keys() const {}
//
//private:
//    vector<vector<Entry> > buckets_;
//    size_t elementCount_;
//    size_t collisionCount_;
//    size_t rehashCount_;
//    double maxLoadFactor_;

//------------
//Bucket Index
//------------
    template <typename Key, typename Value>
    size_t HashTable<Key, Value>::bucketIndex(const Key& key) const {
        return hash<Key>{}(key) % buckets_.size();
    }

//-------------------
//Find Entry Position
//-------------------
template <typename Key, typename Value>
size_t HashTable<Key, Value>::findEntryPosition
(size_t bucket, const Key& key) const {
    const auto& chain = buckets_[bucket];
    for(size_t i = 0; i < chain.size(); i++){
        if(chain[i].key == key){
            return i;
        }
    }
    return chain.size();
}
//
//    void insertWithoutStats(const Key& key, const Value& value) {}
//
    static bool isPrime(size_t n) {
          if(n < 2)      return false;
          if(n == 2)     return true;
          if(n % 2 == 0) return false;
          
          for(size_t i = 3; i * i <= n; i += 2){
              if(n % i == 0) return false;
          }
          
          return true;
      }

    static size_t nextPrime(size_t n) {
        if(n < 2) return 2;
        
        while(!isPrime(n)){
            ++n;
        }
        return n;
    }
