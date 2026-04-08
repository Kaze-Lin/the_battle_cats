#include "DatabaseManager.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility> // for std::move
#include <filesystem> // 加入 filesystem 以便印出當前工作目錄

using json = nlohmann::json;

bool DatabaseManager::LoadCatData(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "[DatabaseManager] error " << filepath << std::endl;
        std::cerr << "-> check the data" << std::filesystem::current_path() << std::endl;
        return false;
    }

    try {
        json j;
        file >> j; // 將檔案內容讀取進 json 物件

        // 【最核心的一行】
        // 直接要求 nlohmann/json 把 "Cat" 陣列轉換成 std::vector<UnitData>
        // 它會在背後自動呼叫我們寫好的 from_json 函式！
        std::vector<UnitData> catList = j.at("Cat").get<std::vector<UnitData>>();

        // 清空舊有的資料庫，確保若是重複呼叫此函式，不會有舊資料殘留
        m_catDatabase.clear();

        // 將解析好的 vector 內容，一筆一筆存入我們的 Hash Map 中以便快速查詢
        for (auto& cat : catList) {
            int id = cat.id; // 先備份 ID，因為 move 之後 cat 的內容可能被掏空
            m_catDatabase[id] = std::move(cat);
        }

        std::cout << "[DatabaseManager] loading success!" << m_catDatabase.size() << " 隻貓咪的資料！\n";
        return true;

    } catch (const json::exception& e) {
        std::cerr << "[DatabaseManager] JSON crash out：" << e.what() << std::endl;
        return false;
    }
}

const UnitData* DatabaseManager::GetCatData(int id) const {
    auto it = m_catDatabase.find(id);
    if (it != m_catDatabase.end()) {
        return &(it->second); // 回傳找到的資料的記憶體位址
    }

    // 如果傳入的 ID 不在資料庫裡，印出警告並回傳空指標
    std::cerr << "[DatabaseManager] warning" << id << " data！\n";
    return nullptr;
}

bool DatabaseManager::LoadEnemyData(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[DatabaseManager] 錯誤：無法開啟敵人檔案 " << filepath << std::endl;
        return false;
    }

    try {
        json j;
        file >> j;

        // 從 JSON 的 "Enemy" 陣列中抓取資料並自動轉換
        std::vector<EnemyData> enemyList = j.at("Enemy").get<std::vector<EnemyData>>();

        for (const auto& enemy : enemyList) {
            m_enemyDatabase[enemy.id] = enemy;
        }

        std::cout << "[DatabaseManager] 成功載入 " << m_enemyDatabase.size() << " 種敵人的資料！\n";
        return true;

    } catch (const json::exception& e) {
        std::cerr << "[DatabaseManager] 敵人 JSON 解析失敗：" << e.what() << std::endl;
        return false;
    }
}

const EnemyData* DatabaseManager::GetEnemyData(int id) const {
    auto it = m_enemyDatabase.find(id);
    if (it != m_enemyDatabase.end()) {
        return &(it->second);
    }
    std::cerr << "[DatabaseManager] 警告：找不到 ID 為 " << id << " 的敵人資料！\n";
    return nullptr;
}

bool DatabaseManager::LoadStageData(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    try {
        json j;
        file >> j;

        // 指定讀取 "Chapters" 這個陣列
        std::vector<ChapterData> chapterList = j.at("Chapters").get<std::vector<ChapterData>>();

        for (const auto& chapter : chapterList) {
            m_chapterDatabase[chapter.chapterId] = chapter;
        }
        return true;
    } catch (const json::exception& e) {
        std::cerr << "關卡解析失敗: " << e.what() << std::endl;
        return false;
    }
}