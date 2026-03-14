#ifndef DATASTORE_H
#define DATASTORE_H

#include <unordered_map>
#include <string>

class DataStore {

private:
    std::unordered_map<std::string, std::string> store;
    std::unordered_map<std::string, long long> expiry;

public:
    void set(std::string key, std::string value);
    std::string get(std::string key);
    void del(std::string key);

    bool exists(std::string key);
    std::string listKeys();
    void clear();

    void expire(std::string key, int seconds);

    void cleanupExpired();

    void saveToDisk();
    void loadFromDisk();

    int size();
};

#endif