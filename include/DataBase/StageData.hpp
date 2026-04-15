#ifndef STAGEDATA_HPP
#define STAGEDATA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <Util/Logger.hpp>

// 單一出怪規則
struct SpawnRule {
    int enemyId;
    float startTime;
    float interval;
    int maxCount;
    int magnification;
    int baseHpTrigger;
    bool isBoss;

    int currentCount = 0;
    float timer = 0.0f;
    bool isUnlocked = false;
};

struct StageData {
    int stageId;
    int enemyLimit;
    int backgroundLength;
    std::string stageName;
    std::string basePath;
    int enemyBaseHp;
    int experience;
    std::vector<SpawnRule> spawnRules;
};

struct ChapterData {
    int chapterId;
    std::string chapterName;
    std::unordered_map<std::string, int> chapterLevel;
    std::vector<StageData> stages;
};

void from_json(const nlohmann::json& j, SpawnRule& r);
void from_json(const nlohmann::json& j, StageData& s);
void from_json(const nlohmann::json& j, ChapterData& c);

#endif