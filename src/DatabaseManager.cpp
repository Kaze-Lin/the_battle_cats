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