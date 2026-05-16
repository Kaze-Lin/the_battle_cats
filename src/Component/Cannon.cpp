#include "Component/Cannon.hpp"
#include <algorithm>

Cannon::Cannon() 
    : m_chargeTimer(0.0f), m_maxChargeTime(30.0f), 
      m_attackLevel(1), m_rangeLevel(1), m_chargeLevel(1),
      m_currentChargeState(0) {
}

void Cannon::Initialize(int attackLevel, int rangeLevel, int chargeLevel) {
    m_attackLevel = attackLevel;
    m_rangeLevel = rangeLevel;
    m_chargeLevel = chargeLevel;
    
    // 充電時間會隨著科技等級縮短
    m_maxChargeTime = std::max(10.0f, 40.0f - (m_chargeLevel * 1.5f));
    m_chargeTimer = 0.0f;
    m_currentChargeState = 0;
    
    NotifyChanged();
}

void Cannon::Update(float deltaTime) {
    if (m_currentChargeState >= 10) return; // 已經充滿
    
    m_chargeTimer += deltaTime;
    if (m_chargeTimer >= m_maxChargeTime) {
        m_chargeTimer = m_maxChargeTime;
    }
    
    int newState = static_cast<int>((m_chargeTimer / m_maxChargeTime) * 10.0f);
    if (newState > 10) newState = 10;
    
    if (newState != m_currentChargeState) {
        m_currentChargeState = newState;
        NotifyChanged();
    }
}

bool Cannon::Fire() {
    if (!IsFull()) {
        LOG_INFO("Cannon is not fully charged yet!");
        return false;
    }
    
    // 發射後重置冷卻
    m_chargeTimer = 0.0f;
    m_currentChargeState = 0;
    NotifyChanged();
    
    return true;
}

float Cannon::GetDamage() const {
    // 基礎傷害 + 等級加成
    return 100.0f + (m_attackLevel - 1) * 50.0f;
}

float Cannon::GetRange() const {
    // 砲擊範圍
    return 500.0f + (m_rangeLevel - 1) * 50.0f;
}

void Cannon::NotifyChanged() {
    if (m_onChargeChanged) {
        m_onChargeChanged(m_currentChargeState);
    }
}
