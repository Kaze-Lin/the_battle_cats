#include "Unit.hpp"
#include "EntityManager.hpp"
#include <iostream>

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"

Unit::Unit(Faction faction, const UnitStats& stats, float startX)
    :Util::GameObject(), m_faction(faction), m_stats(stats), m_positionX(startX) {
    
    m_currentHp = m_stats.maxHp;
    m_currentState = UnitState::Walk;
    m_stateTimer = 0.0f;
    m_knockbackCount = 0;

    m_direction = (m_faction == Faction::Player) ? -1 : 1;

    m_knockbackThreshold = m_stats.maxHp / m_stats.knockbackLimit;
    if (m_knockbackThreshold <= 0) m_knockbackThreshold = 1;
}

void Unit::ChangeState(UnitState newState) {
    m_currentState = newState;
    m_stateTimer = 0.0f;
}

void Unit::Update(float deltaTime) {
    m_stateTimer += deltaTime;

    switch (m_currentState) {
        case UnitState::Walk:
            m_positionX += m_stats.moveSpeed * m_direction * deltaTime;
            
            // TODO: 之後要在這裡呼叫 EntityManager，判斷前方有沒有敵人，有的話切換到 Precast
            break;

        case UnitState::Precast:
            if (m_stateTimer >= m_stats.precastTime) {
                // TODO: 攻擊判定！呼叫 EntityManager 找出範圍內的敵人並扣血
                std::cout << m_stats.name << " 造成了 " << m_stats.attackPower << " 點傷害！\n";
                ChangeState(UnitState::Postcast);
            }
            break;

        case UnitState::Postcast:
            if (m_stateTimer >= m_stats.postcastTime) {
                ChangeState(UnitState::Cooldown);
            }
            break;

        case UnitState::Cooldown:
            if (m_stateTimer >= m_stats.cooldownTime) {
                ChangeState(UnitState::Walk);
            }
            break;

        case UnitState::Knockback:
            // 擊退時會快速往後退
            m_positionX -= (m_stats.moveSpeed * 2.0f) * m_direction * deltaTime;
            
            // 假設擊退硬直時間為 0.5 秒
            if (m_stateTimer >= 0.5f) {
                if (IsDead()) {
                    ChangeState(UnitState::Dead);
                } else {
                    ChangeState(UnitState::Walk);
                }
            }
            break;

        case UnitState::Dead:
            // 等待死亡動畫播完 (假設 1 秒)
            break;
    }
}

void Unit::TakeDamage(int damage) {
    if (IsDead() || m_currentState == UnitState::Knockback) return;

    m_currentHp -= damage;
    
    if (m_currentHp <= 0) {
        m_currentHp = 0;
        ChangeState(UnitState::Knockback); // 死亡前強制擊退一次
        return;
    }

    // KB (Knockback) 判定邏輯
    int lostHp = m_stats.maxHp - m_currentHp;
    int expectedKnockbacks = lostHp / m_knockbackThreshold;
    
    if (expectedKnockbacks > m_knockbackCount && m_knockbackCount < m_stats.knockbackLimit - 1) {
        m_knockbackCount = expectedKnockbacks;
        ForceKnockback();
    }
}

void Unit::ForceKnockback() {
    if (!IsDead()) {
         LOG_INFO(m_stats.name + " 被擊退了！\n");
        ChangeState(UnitState::Knockback);
    }
}

bool Unit::IsDeadAndAnimationFinished() const {
    return (m_currentState == UnitState::Dead && m_stateTimer >= 1.0f);
}

void Unit::Render() const {

}