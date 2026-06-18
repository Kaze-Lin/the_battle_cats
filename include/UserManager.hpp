#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <memory>
#include "DataBase/UserData.hpp"
#include "Util/Logger.hpp"

class UserManager {
private:
    std::unordered_map<std::string, std::shared_ptr<UserProfile>> m_database;
    std::shared_ptr<UserProfile> m_currentUser = nullptr;

    const std::string SAVE_FILE_PATH = RESOURCE_DIR "/assets/Database/UserData.json";

    bool m_isLoaded = false;

    UserManager() {
        LoadDatabase();
    }
    ~UserManager() = default;

public:
    static UserManager& GetInstance() {
        static UserManager instance;
        return instance;
    }

    void LoadDatabase() {
        std::ifstream file(SAVE_FILE_PATH);
        if (!file.is_open()) {
            LOG_WARN("No existing user database found. Will create a new one upon saving.");
            m_isLoaded = true;
            return;
        }

        try {
            nlohmann::json j;
            file >> j;
            m_database.clear();

            for (const auto& item : j["users"]) {
                auto profile = std::make_shared<UserProfile>(item.get<UserProfile>());
                m_database[profile->username] = profile;
            }
            m_isLoaded = true;
            LOG_INFO("User database loaded safely. Total users: %zu", m_database.size());

        } catch (const std::exception& e) {
            LOG_ERROR("JSON parsing failed! Corrupted file? %s", e.what());
        }
    }

    void SaveDatabase() {
        if (!m_isLoaded) {
            LOG_ERROR("Refusing to save: Database was not properly loaded! Risk of data loss.");
            return;
        }

        nlohmann::json j;

        std::vector<UserProfile> users;
        for (const auto& pair : m_database) {
            users.push_back(*(pair.second));
        }
        j["users"] = users;


        std::string tempPath = SAVE_FILE_PATH + ".tmp";
        std::ofstream file(tempPath);
        if (!file.is_open()) {
            LOG_ERROR("Failed to create temporary save file.");
            return;
        }

        file << j.dump(4);
        file.close();

        std::remove(SAVE_FILE_PATH.c_str());
        if (std::rename(tempPath.c_str(), SAVE_FILE_PATH.c_str()) == 0) {
            LOG_INFO("User database saved successfully without losing old data.");
        } else {
            LOG_ERROR("Failed to replace the old save file!");
        }
    }

    bool Register(const std::string& username, const std::string& password) {
        if (m_database.find(username) != m_database.end()) {
            LOG_WARN("Registration failed: Username %s already exists.", username.c_str());
            return false;
        }

        auto newUser = std::make_shared<UserProfile>();
        newUser->username = username;
        newUser->password = password;

        newUser->resources.catFood = 100;
        newUser->progress.currentStage = {1, 1};
        newUser->baseUpgrades.walletCapacity = 1;

        m_database[username] = newUser;
        std::string temp = username.c_str();
        temp = "User " + temp + " registered successfully.";
        LOG_INFO(temp);

        SaveDatabase();
        return true;
    }

    bool Login(const std::string& username, const std::string& password) {
        auto it = m_database.find(username);

        if (it == m_database.end()) {
            LOG_WARN("Login failed: User %s not found.", username.c_str());
            return false;
        }

        if (it->second->password == password) {
            m_currentUser = it->second;
            LOG_INFO("User '%s' logged in successfully.", username.c_str());
            return true;
        } else {
            LOG_WARN("Login failed: Incorrect password for '%s'.", username.c_str());
            return false;
        }
    }

    void Logout() {
        if (m_currentUser) {
            LOG_INFO("User %s logged out.", m_currentUser->username.c_str());
            m_currentUser = nullptr;
            SaveDatabase();
        }
    }

    std::shared_ptr<UserProfile> GetCurrentUser() { return m_currentUser; }

    // --- Resource Modification API ---
    int GetXP() const {
        return m_currentUser ? m_currentUser->resources.xp : 0;
    }

    void AddXP(int amount) {
        if (m_currentUser && amount > 0) {
            m_currentUser->resources.xp += amount;
            SaveDatabase();
        }
    }

    bool SpendXP(int amount) {
        if (m_currentUser && amount > 0 && m_currentUser->resources.xp >= amount) {
            m_currentUser->resources.xp -= amount;
            SaveDatabase();
            return true;
        }
        return false;
    }

    int GetCatFood() const {
        return m_currentUser ? m_currentUser->resources.catFood : 0;
    }

    void AddCatFood(int amount) {
        if (m_currentUser && amount > 0) {
            m_currentUser->resources.catFood += amount;
            SaveDatabase();
        }
    }

    bool SpendCatFood(int amount) {
        if (m_currentUser && amount > 0 && m_currentUser->resources.catFood >= amount) {
            m_currentUser->resources.catFood -= amount;
            SaveDatabase();
            return true;
        }
        return false;
    }

    // --- Encapsulated API for Cat Data Access & Modification ---

    std::vector<CatSaveData> GetUnlockedCats() const {
        if (m_currentUser) {
            return m_currentUser->unlockedCats;
        }
        return {};
    }

    // Returns true if upgrade was successful. Outputs the updated cat data.
    bool TryUpgradeCat(int catId, int cost, CatSaveData& outUpdatedCat) {
        if (!m_currentUser) return false;

        for (auto& cat : m_currentUser->unlockedCats) {
            if (cat.catId == catId) {
                if (cat.level < 10 && m_currentUser->resources.xp >= cost) {
                    m_currentUser->resources.xp -= cost;
                    cat.level += 1;
                    if (cat.level == 10) {
                        cat.currentForm = 2; // Unlock next form at level 10
                    }
                    outUpdatedCat = cat;
                    SaveDatabase(); // Ensure the upgrade is immediately persisted to disk
                    return true;
                }
                break; // Cat found but already max level or not enough XP
            }
        }
        return false;
    }
};

#endif