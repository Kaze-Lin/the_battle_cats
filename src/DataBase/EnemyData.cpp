#include "DataBase/EnemyData.hpp"

using json = nlohmann::json;

void from_json(const json& j, EnemyAnimationData& a) {
    j.at("precast").get_to(a.precast);
    j.at("postcast").get_to(a.postcast);
    LOG_INFO("loading enemy AnimationData successfully");
}

void from_json(const json& j, EnemyData& e) {
    j.at("id").get_to(e.id);
    j.at("name").get_to(e.name);
    j.at("photo_path").get_to(e.photoPath);
    j.at("base_hp").get_to(e.baseHp);
    j.at("base_attack").get_to(e.baseAttack);
    j.at("range").get_to(e.range);
    j.at("is_area_attack").get_to(e.isAreaAttack);
    j.at("animation").get_to(e.animation);
    j.at("time_between_attacks").get_to(e.timeBetweenAttacks);
    j.at("move_speed").get_to(e.moveSpeed);
    j.at("Knockback").get_to(e.knockback);   // 注意：JSON 裡是寫大寫的 K
    j.at("money_drop").get_to(e.moneyDrop);
    j.at("traits").get_to(e.traits);
    LOG_INFO("loading enemy EnemyData successfully");
}