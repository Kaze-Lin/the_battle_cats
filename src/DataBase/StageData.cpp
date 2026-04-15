#include "DataBase/StageData.hpp"

using json = nlohmann::json;

void from_json(const json& j, SpawnRule& r) {
    j.at("enemy_id").get_to(r.enemyId);
    j.at("start_time").get_to(r.startTime);
    j.at("interval").get_to(r.interval);
    j.at("max_count").get_to(r.maxCount);
    j.at("magnification").get_to(r.magnification);
    j.at("base_hp_trigger").get_to(r.baseHpTrigger);
    j.at("is_boss").get_to(r.isBoss);
    LOG_INFO("loading stage SpawnRule successfully");
}

void from_json(const json& j, StageData& s) {
    j.at("stage_id").get_to(s.stageId);
    j.at("enemy_limit").get_to(s.enemyLimit);
    j.at("background_length").get_to(s.backgroundLength);
    j.at("stage_name").get_to(s.stageName);
    j.at("base_path").get_to(s.basePath);
    j.at("enemy_base_hp").get_to(s.enemyBaseHp);
    j.at("experience").get_to(s.experience);
    j.at("spawn_rules").get_to(s.spawnRules);
    LOG_INFO("loading stage StageData successfully");
}

void from_json(const json& j, ChapterData& c) {
    j.at("chapter_id").get_to(c.chapterId);
    j.at("chapter_name").get_to(c.chapterName);
    j.at("chapter_level").get_to(c.chapterLevel);
    j.at("stages").get_to(c.stages);
    LOG_INFO("loading stage ChapterData successfully");
}