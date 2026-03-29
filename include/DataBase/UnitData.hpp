#ifndef UNITDATA_HPP
#define UNITDATA_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct AnimationData {
    int precast; // 前搖
    int postcast; // 後搖
};

struct FormData {
    int formIndex;
    std::string name;
    int cost;
    int baseHp;
    int hpGrowth;
    int baseAttack;
    int attackGrowth;
    int range;
    bool isAreaAttack;
    AnimationData animation;
    int timeBetweenAttacks;
    float moveSpeed;
    int knockback;
    std::string ability;
    std::string talent;
    int rechargeTime;
};

// 整隻貓咪的總容器
struct UnitData {
    int id;
    std::string nameInternal;
    int maxLevel;
    std::vector<FormData> forms;
    std::vector<int> upgradeCosts;
};

void from_json(const nlohmann::json& j, AnimationData& a);
void from_json(const nlohmann::json& j, FormData& f);
void from_json(const nlohmann::json& j, UnitData& u);

#endif