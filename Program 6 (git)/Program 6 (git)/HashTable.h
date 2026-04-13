#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <cstddef>

using namespace std;

// Generic hash table using separate chaining.

template <typename Key, typename Value>
class HashTable {
public:
    struct Entry {
        Key key;
        Value value;

        Entry(const Key& k, const Value& v) : key(k), value(v) {}
    };

    explicit HashTable(size_t initialSize = 101, double maxLoadFactor = 0.75):
        buckets_(nextPrime(initialSize)),
        elementCount_(0),
        collisionCount_(0),
        rehashCount_(0),
        maxLoadFactor_(maxLoadFactor) {}
        

    bool insert(const Key& key, const Value& value);
        

    bool contains(const Key& key) const;

    bool remove(const Key& key);

    Value* find(const Key& key);

    const Value* find(const Key& key) const;

    Value& at(const Key& key);

    const Value& at(const Key& key) const;

    void rehash();

    size_t size() const;

    size_t tableSize() const;

    double loadFactor() const;

    size_t collisionCount() const;

    size_t rehashCount() const;

    vector<Key> keys() const;

private:
    vector<vector<Entry> > buckets_;
    size_t elementCount_;
    size_t collisionCount_;
    size_t rehashCount_;
    double maxLoadFactor_;

    size_t bucketIndex(const Key& key) const;

    size_t findEntryPosition(size_t bucket, const Key& key) const;

    void insertWithoutStats(const Key& key, const Value& value);

    static bool isPrime(size_t n);

    static size_t nextPrime(size_t n);
};

#endif
