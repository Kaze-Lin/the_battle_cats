#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <string>
#include <unordered_map>
#include "UnitData.hpp"

class DatabaseManager {
private:
    std::unordered_map<int, UnitData> m_catDatabase;

    DatabaseManager() = default;
    ~DatabaseManager() = default;

public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager& GetInstance() {
        static DatabaseManager instance;
        return instance;
    }

    bool LoadCatData(const std::string& filepath);

    const UnitData* GetCatData(int id) const;
};

#endif