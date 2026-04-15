#include "Unit.hpp"
#include "EntityManager.hpp"
#include <iostream>

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"

Unit::Unit(Faction faction, const UnitStats& stats, float startX)
    : Util::GameObject(), m_faction(faction), m_stats(stats), m_positionX(startX) {

    m_currentHp = m_stats.maxHp;
    m_currentState = UnitState::Walk;
    m_stateTimer = 0.0f;
    m_knockbackCount = 0;
    m_direction = (m_faction == Faction::Player) ? -1 : 1;

    m_knockbackThreshold = m_stats.maxHp / m_stats.knockbackLimit;
    if (m_knockbackThreshold <= 0) m_knockbackThreshold = 1;

    if (!m_stats.photoPath.empty()) {
        m_Drawable = std::make_shared<Util::Image>(m_stats.photoPath);
    }

    m_Transform.translation = glm::vec2(m_positionX, -50.0f);

    // m_Transform.scale = glm::vec2(1.0f, 1.0f);

    if (m_stats.name == "Cat_Base" || m_stats.name == "Enemy_Base") {
        this->SetZIndex(5.0f);
    } else {
        this->m_Transform.translation = glm::vec2(m_positionX, -145.0f);
        this->SetZIndex(10.0f);
    }
}

void Unit::ChangeState(UnitState newState) {
    m_currentState = newState;
    m_stateTimer = 0.0f;
}

void Unit::Update(float deltaTime) {
    m_stateTimer += deltaTime;

    Faction targetFaction = (m_faction == Faction::Player) ? Faction::Enemy : Faction::Player;

    switch (m_currentState) {
        case UnitState::Walk: {
            float detectEdge = m_positionX + (m_stats.range * m_direction);
            auto targets = EntityManager::GetInstance().GetEntitiesInRange(targetFaction, m_positionX, detectEdge);

            if (!targets.empty()) {
                ChangeState(UnitState::Precast);
            } else {
                m_positionX += m_stats.moveSpeed * m_direction * deltaTime;
            }
            break;
        }

        case UnitState::Precast: {
            if (m_stateTimer >= m_stats.precastTime) {
                float attackEdge = m_positionX + (m_stats.range * m_direction);
                auto hitTargets = EntityManager::GetInstance().GetEntitiesInRange(targetFaction, m_positionX, attackEdge);

                if (!hitTargets.empty()) {
                    if (m_stats.isAreaAttack) {
                        for (auto* target : hitTargets) {
                            target->TakeDamage(m_stats.attackPower);
                        }
                    } else {
                        Unit* closestTarget = hitTargets[0];
                        float minDistance = std::abs(closestTarget->GetPositionX() - m_positionX);

                        for (auto* target : hitTargets) {
                            float dist = std::abs(target->GetPositionX() - m_positionX);
                            if (dist < minDistance) {
                                minDistance = dist;
                                closestTarget = target;
                            }
                        }
                        closestTarget->TakeDamage(m_stats.attackPower);
                    }
                }
                ChangeState(UnitState::Postcast);
            }
            break;
        }

        case UnitState::Postcast: {
            if (m_stateTimer >= m_stats.postcastTime) {
                ChangeState(UnitState::Cooldown);
            }
            break;
        }

        case UnitState::Cooldown: {
            if (m_stateTimer >= m_stats.cooldownTime) {
                ChangeState(UnitState::Walk);
            }
            break;
        }

        case UnitState::Knockback: {
            m_positionX -= (m_stats.moveSpeed * 2.0f) * m_direction * deltaTime;

            if (m_stateTimer >= 0.5f) {
                if (IsDead()) {
                    ChangeState(UnitState::Dead);
                } else {
                    ChangeState(UnitState::Walk);
                }
            }
            break;
        }

        case UnitState::Dead: {
            break;
        }
    }

    m_Transform.translation.x = m_positionX;
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
         LOG_INFO(m_stats.name + " knockback!\n");
        ChangeState(UnitState::Knockback);
    }
}

bool Unit::IsDeadAndAnimationFinished() const {
    return (m_currentState == UnitState::Dead && m_stateTimer >= 0.0f);
}

void Unit::Render() const {

}