#include "DataBase/UnitData.hpp"

#include "Util/Logger.hpp"

using json = nlohmann::json;

void from_json(const json& j, AnimationData& a) {
    j.at("precast").get_to(a.precast);
    j.at("postcast").get_to(a.postcast);
    LOG_INFO("loading cat AnimationData successfully");
}

void from_json(const json& j, FormData& f) {
    j.at("form_index").get_to(f.formIndex);
    j.at("name").get_to(f.name);
    j.at("cost").get_to(f.cost);
    j.at("base_hp").get_to(f.baseHp);
    j.at("hp_growth").get_to(f.hpGrowth);
    j.at("base_attack").get_to(f.baseAttack);
    j.at("attack_growth").get_to(f.attackGrowth);
    j.at("range").get_to(f.range);
    j.at("is_area_attack").get_to(f.isAreaAttack);
    j.at("animation").get_to(f.animation);
    j.at("time_between_attacks").get_to(f.timeBetweenAttacks);
    j.at("move_speed").get_to(f.moveSpeed);
    j.at("Knockback").get_to(f.knockback);
    j.at("ability").get_to(f.ability);
    j.at("talent").get_to(f.talent);
    j.at("Recharge_Time").get_to(f.rechargeTime);
    LOG_INFO("loading cat FormData successfully");
}

void from_json(const json& j, UnitData& u) {
    j.at("id").get_to(u.id);
    j.at("name_internal").get_to(u.nameInternal);
    j.at("max_level").get_to(u.maxLevel);
    j.at("cat_path").get_to(u.catPath);
    j.at("generate_cat_button").get_to(u.catGenButton);

    j.at("forms").get_to(u.forms);
    j.at("upgrade_costs").get_to(u.upgradeCosts);
    LOG_INFO("loading cat UnitData successfully");
}