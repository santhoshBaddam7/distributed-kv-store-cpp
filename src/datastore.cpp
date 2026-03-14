#include "datastore.h"
#include <fstream>
#include <chrono>

long long currentTime()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void DataStore::set(std::string key, std::string value)
{
    store[key] = value;
    saveToDisk();
}

std::string DataStore::get(std::string key)
{
    if(expiry.find(key) != expiry.end())
    {
        if(currentTime() > expiry[key])
        {
            store.erase(key);
            expiry.erase(key);
            return "NULL";
        }
    }

    if(store.find(key) == store.end())
        return "NULL";

    return store[key];
}

void DataStore::del(std::string key)
{
    store.erase(key);
    expiry.erase(key);
    saveToDisk();
}

bool DataStore::exists(std::string key)
{
    return store.find(key) != store.end();
}

std::string DataStore::listKeys()
{
    std::string result;

    for(auto &p : store)
        result += p.first + "\n";

    return result;
}

void DataStore::clear()
{
    store.clear();
    expiry.clear();
    saveToDisk();
}

void DataStore::expire(std::string key, int seconds)
{
    expiry[key] = currentTime() + seconds;
}

void DataStore::cleanupExpired()
{
    long long now = currentTime();

    for(auto it = expiry.begin(); it != expiry.end(); )
    {
        if(now > it->second)
        {
            store.erase(it->first);
            it = expiry.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void DataStore::saveToDisk()
{
    std::ofstream file("database.txt");

    for(auto &p : store)
        file << p.first << " " << p.second << "\n";

    file.close();
}

void DataStore::loadFromDisk()
{
    std::ifstream file("database.txt");

    std::string key,value;

    while(file >> key >> value)
        store[key] = value;

    file.close();
}

int DataStore::size()
{
    return store.size();
}