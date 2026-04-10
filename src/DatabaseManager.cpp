#include "DatabaseManager.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>
#include <filesystem>

using json = nlohmann::json;

namespace {
    template <typename DataType, typename MapType, typename GetIdFunc>
    bool LoadDataHelper(const std::string& filepath,
                        const std::string& jsonKey,
                        const std::string& logName,
                        MapType& database,
                        GetIdFunc getId) {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "[DatabaseManager] error " << filepath << std::endl;
            std::cerr << "-> check the data: " << std::filesystem::current_path() << std::endl;
            return false;
        }

        try {
            json j;
            file >> j;

            // 從指定的 JSON key 抓取資料並自動轉換
            std::vector<DataType> dataList = j.at(jsonKey).get<std::vector<DataType>>();

            // 統一清空舊資料，確保重新載入時不會有殘留
            database.clear();

            for (auto& item : dataList) {
                auto id = getId(item); // 透過 Lambda 動態取得 ID (id 或是 chapterId)
                database[id] = std::move(item);
            }

            std::cout << "[DatabaseManager] loading success! " << database.size() << " 筆" << logName << "的資料！\n";
            return true;

        } catch (const json::exception& e) {
            std::cerr << "[DatabaseManager] " << logName << " JSON crash out：" << e.what() << std::endl;
            return false;
        }
    }

    // === 泛用讀取資料 Helper ===
    template <typename DataType, typename MapType, typename KeyType>
    const DataType* GetDataHelper(const MapType& database, KeyType id, const std::string& logName) {
        auto it = database.find(id);
        if (it != database.end()) {
            return &(it->second);
        }

        std::cerr << "[DatabaseManager] warning " << id << " " << logName << " data！\n";
        return nullptr;
    }
}

// --- 貓咪資料 ---
bool DatabaseManager::LoadCatData(const std::string& filepath) {
    return LoadDataHelper<UnitData>(filepath, "Cat", "貓咪", m_catDatabase,
        [](const UnitData& d) { return d.id; });
}

const UnitData* DatabaseManager::GetCatData(int id) const {
    return GetDataHelper<UnitData>(m_catDatabase, id, "貓咪");
}


// --- 敵人資料 ---
bool DatabaseManager::LoadEnemyData(const std::string& filepath) {
    return LoadDataHelper<EnemyData>(filepath, "Enemy", "敵人", m_enemyDatabase,
        [](const EnemyData& d) { return d.id; });
}

const EnemyData* DatabaseManager::GetEnemyData(int id) const {
    return GetDataHelper<EnemyData>(m_enemyDatabase, id, "敵人");
}


// --- 關卡資料 ---
bool DatabaseManager::LoadStageData(const std::string& filepath) {
    // 這裡的 Lambda 自動處理了欄位名稱的不同 (chapterId)
    return LoadDataHelper<ChapterData>(filepath, "Chapters", "關卡", m_chapterDatabase,
        [](const ChapterData& d) { return d.chapterId; });
}

const ChapterData* DatabaseManager::GetChapterData(int chapterId) const {
    return GetDataHelper<ChapterData>(m_chapterDatabase, chapterId, "篇章");
}

const StageData* DatabaseManager::GetStageData(int chapterId, int stageId) const {
    // 1. 先拿到該篇章的資料
    const ChapterData* chapter = GetChapterData(chapterId);

    // 2. 如果篇章存在，就在裡面的 stages 陣列尋找對應的 stageId
    if (chapter != nullptr) {
        for (const auto& stage : chapter->stages) {
            if (stage.stageId == stageId) {
                return &stage; // 找到了！回傳這個關卡的指標
            }
        }
        std::cerr << "[DatabaseManager] 警告：在 Chapter " << chapterId << " 中找不到 Stage " << stageId << " 的資料！\n";
    }

    return nullptr;
}