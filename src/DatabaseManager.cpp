#include "DatabaseManager.hpp"
#include <fstream>
#include <filesystem>
#include <Util/Logger.hpp>

using json = nlohmann::json;

namespace {
    // === 泛用讀取資料 Helper ===
    template <typename DataType, typename MapType, typename GetIdFunc>
    bool LoadDataHelper(const std::string& filepath,
                        const std::string& jsonKey,
                        const std::string& logName,
                        MapType& database,
                        GetIdFunc getId) {
        std::ifstream file(filepath);

        if (!file.is_open()) {
            LOG_ERROR("[DatabaseManager] cannot open the file: %s", filepath.c_str());
            LOG_ERROR("-> please check the path: %s", std::filesystem::current_path().string().c_str());
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
                auto id = getId(item); // 透過 Lambda 動態取得 ID
                database[id] = std::move(item);
            }

            LOG_INFO("[DatabaseManager] loading " + std::to_string(database.size()) + " " + logName + " successfully");
            return true;

        } catch (const json::exception& e) {
            LOG_ERROR("[DatabaseManager] loading " + logName + " failed: " + std::string(e.what()));
            return false;
        }
    }

    // === 泛用查詢資料 Helper ===
    template <typename DataType, typename MapType, typename KeyType>
    const DataType* GetDataHelper(const MapType& database, KeyType id, const std::string& logName) {
        auto it = database.find(id);
        if (it != database.end()) {
            return &(it->second);
        }
        LOG_WARN("[DatabaseManager] cannot find " + logName + " with id " + std::to_string((int)id));
        return nullptr;
    }
}

// ==========================================
// 貓咪資料 (Cat Data)
// ==========================================
bool DatabaseManager::LoadCatData(const std::string& filepath) {
    return LoadDataHelper<UnitData>(filepath, "Cat", "貓咪", m_catDatabase,
        [](const UnitData& d) { return d.id; });
}

const UnitData* DatabaseManager::GetCatData(int id) const {
    return GetDataHelper<UnitData>(m_catDatabase, id, "貓咪");
}

// ==========================================
// 敵人資料 (Enemy Data)
// ==========================================
bool DatabaseManager::LoadEnemyData(const std::string& filepath) {
    return LoadDataHelper<EnemyData>(filepath, "Enemy", "敵人", m_enemyDatabase,
        [](const EnemyData& d) { return d.id; });
}

const EnemyData* DatabaseManager::GetEnemyData(int id) const {
    return GetDataHelper<EnemyData>(m_enemyDatabase, id, "敵人");
}

// ==========================================
// 關卡資料 (Stage Data)
// ==========================================
bool DatabaseManager::LoadStageData(const std::string& filepath) {
    return LoadDataHelper<ChapterData>(filepath, "Chapters", "關卡篇章", m_chapterDatabase,
        [](const ChapterData& d) { return d.chapterId; });
}

const ChapterData* DatabaseManager::GetChapterData(int chapterId) const {
    return GetDataHelper<ChapterData>(m_chapterDatabase, chapterId, "篇章");
}

const StageData* DatabaseManager::GetStageData(int chapterId, int stageId) const {
    const ChapterData* chapter = GetChapterData(chapterId);

    if (chapter != nullptr) {
        for (const auto& stage : chapter->stages) {
            if (stage.stageId == stageId) {
                return &stage;
            }
        }
        LOG_WARN("[DatabaseManager] cannot find stage with id " + std::to_string(stageId) + " in chapter " + std::to_string(chapterId));
    }
    return nullptr;
}