#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

struct Resources {
    int xp = 0;
    int catFood = 0;
    int energy = 0;
    int maxEnergy = 0;
};
inline void to_json(nlohmann::json& j, const Resources& r) {
    j = nlohmann::json{{"xp", r.xp}, {"cat_food", r.catFood}, {"energy", r.energy}, {"max_energy", r.maxEnergy}};
}
inline void from_json(const nlohmann::json& j, Resources& r) {
    r.xp = j.value("xp", 0);
    r.catFood = j.value("cat_food", 0);
    r.energy = j.value("energy", 100);
    r.maxEnergy = j.value("max_energy", 100);
}

struct Progress {
    int clearedChapters = 0;
    std::array<int, 2> currentStage = {1, 1}; // {chapter, stage}
    int highestStageCleared = 0;
    int userRank = 1;
};
inline void to_json(nlohmann::json& j, const Progress& p) {
    j = nlohmann::json{{"cleared_chapters", p.clearedChapters}, {"current_stage", p.currentStage}, {"highest_stage_cleared", p.highestStageCleared}, {"user_rank", p.userRank}};
}
inline void from_json(const nlohmann::json& j, Progress& p) {
    p.clearedChapters = j.value("cleared_chapters", 0);
    p.currentStage = j.value("current_stage", std::array<int, 2>{1, 1});
    p.highestStageCleared = j.value("highest_stage_cleared", 0);
    p.userRank = j.value("user_rank", 0);
}

struct BaseUpgrades {
    int workerCatEfficiency = 1;
    int walletCapacity = 1;
    int baseHp = 1;
    int cannonAttack = 1;
    int cannonRange = 1;
    int cannonCharge = 1;
    int accounting = 1;
    int study = 1;
};
inline void to_json(nlohmann::json& j, const BaseUpgrades& b) {
    j = nlohmann::json{
        {"worker_cat_efficiency", b.workerCatEfficiency}, {"wallet_capacity", b.walletCapacity},
        {"base_hp", b.baseHp}, {"cannon_attack", b.cannonAttack}, {"cannon_range", b.cannonRange},
        {"cannon_charge", b.cannonCharge}, {"accounting", b.accounting}, {"study", b.study}
    };
}
inline void from_json(const nlohmann::json& j, BaseUpgrades& b) {
    b.workerCatEfficiency = j.value("worker_cat_efficiency", 1);
    b.walletCapacity = j.value("wallet_capacity", 1);
    b.baseHp = j.value("base_hp", 1);
    b.cannonAttack = j.value("cannon_attack", 1);
    b.cannonRange = j.value("cannon_range", 1);
    b.cannonCharge = j.value("cannon_charge", 1);
    b.accounting = j.value("accounting", 1);
    b.study = j.value("study", 1);
}

struct CatSaveData {
    int catId = 0;
    int level = 1;
    int currentForm = 1;
};
inline void to_json(nlohmann::json& j, const CatSaveData& c) {
    j = nlohmann::json{{"cat_id", c.catId}, {"level", c.level}, {"current_form", c.currentForm}};
}
inline void from_json(const nlohmann::json& j, CatSaveData& c) {
    c.catId = j.value("cat_id", 0);
    c.level = j.value("level", 1);
    c.currentForm = j.value("current_form", 1);
}

// --- 玩家總檔案 ---
struct UserProfile {
    std::string username;
    std::string password;
    Resources resources;
    Progress progress;
    BaseUpgrades baseUpgrades;
    std::vector<CatSaveData> unlockedCats;
    std::unordered_map<std::string, std::vector<int>> treasures;
};

inline void to_json(nlohmann::json& j, const UserProfile& p) {
    j = nlohmann::json{
        {"username", p.username},
        {"password", p.password},
        {"resources", p.resources},
        {"progress", p.progress},
        {"base_upgrades", p.baseUpgrades},
        {"unlocked_cats", p.unlockedCats},
        {"treasures", p.treasures}
    };
}

inline void from_json(const nlohmann::json& j, UserProfile& p) {
    j.at("username").get_to(p.username);
    p.password = j.value("password", "");

    if(j.contains("resources")) j.at("resources").get_to(p.resources);
    if(j.contains("progress")) j.at("progress").get_to(p.progress);
    if(j.contains("base_upgrades")) j.at("base_upgrades").get_to(p.baseUpgrades);
    if(j.contains("unlocked_cats")) j.at("unlocked_cats").get_to(p.unlockedCats);
    if(j.contains("treasures")) j.at("treasures").get_to(p.treasures);
}

#endif