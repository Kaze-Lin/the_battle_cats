#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>

#include "Util/GameObject.hpp"

enum class Faction {
    Player,
    Enemy
};
// 定義單位的狀態
enum class UnitState {
    Walk,
    Precast,   // 攻擊前搖
    Postcast,  // 攻擊後搖
    Cooldown,  // 攻擊冷卻中
    Knockback, // 被擊退中
    Dead       // 死亡
};

struct UnitStats {
    std::string name;
    int maxHp;
    int attackPower;
    float range;
    bool isAreaAttack;
    float moveSpeed;
    int knockbackLimit;

    float precastTime;
    float postcastTime;
    float cooldownTime;

    std::string imagePath;
};

class Unit: public Util::GameObject {
private:
    Faction m_faction;
    UnitStats m_stats;

    int m_currentHp;
    float m_positionX;
    int m_direction;

    int m_knockbackThreshold; // 每扣多少血會被擊退一次
    int m_knockbackCount;     // 已經被擊退了幾次

    UnitState m_currentState;
    float m_stateTimer;

    void ChangeState(UnitState newState);

public:
    Unit(Faction faction, const UnitStats& stats, float startX);
    virtual ~Unit() = default;

    void Update(float deltaTime);
    void Render() const;

    // 戰鬥互動
    void TakeDamage(int damage);
    void ForceKnockback();

    // 取得資訊
    bool IsDeadAndAnimationFinished() const;
    Faction GetFaction() const { return m_faction; }
    float GetPositionX() const { return m_positionX; }
    float GetRange() const { return m_stats.range; }
    bool IsDead() const { return m_currentHp <= 0; }
};

#endif