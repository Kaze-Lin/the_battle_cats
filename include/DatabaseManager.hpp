#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <string>
#include <unordered_map>
#include "DataBase/UnitData.hpp"
#include "DataBase/EnemyData.hpp"
#include "DataBase/StageData.hpp"

class DatabaseManager {
private:
    std::unordered_map<int, UnitData> m_catDatabase;
    std::unordered_map<int, EnemyData> m_enemyDatabase;
    std::unordered_map<int, ChapterData> m_chapterDatabase;

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

    bool LoadEnemyData(const std::string& filepath);
    const EnemyData* GetEnemyData(int id) const;

    bool LoadStageData(const std::string& filepath);
    const ChapterData* GetChapterData(int chapterId) const;
    const StageData* GetStageData(int chapterId, int stageId) const;
};

#endif