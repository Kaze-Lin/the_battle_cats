#ifndef ENEMYDATA_HPP
#define ENEMYDATA_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Util/Logger.hpp>

// 敵人的動畫資料 (可以跟貓咪共用同一個結構，這裡以防萬一再寫一次)
struct EnemyAnimationData {
    int precast;
    int postcast;
};

// 敵人的本體資料
struct EnemyData {
    int id;
    std::string name;
    std::string photoPath;
    int baseHp;
    int baseAttack;
    int range;
    bool isAreaAttack;
    EnemyAnimationData animation;
    int timeBetweenAttacks;
    float moveSpeed;
    int knockback;
    int moneyDrop; // 專屬：掉落金錢
    std::vector<std::string> traits; // 專屬：擁有的屬性(陣列)
};

void from_json(const nlohmann::json& j, EnemyAnimationData& a);
void from_json(const nlohmann::json& j, EnemyData& e);

#endif