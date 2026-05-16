#include "Component/Wallet.hpp"

Wallet::Wallet() 
    : m_currentMoney(0.0f), m_maxCapacity(0), m_currentLevel(1),
      m_workerCatEfficiencyLevel(1), m_walletCapacityLevel(1) {
}

void Wallet::Initialize(int workerCatEfficiencyLevel, int walletCapacityLevel) {
    m_workerCatEfficiencyLevel = workerCatEfficiencyLevel;
    m_walletCapacityLevel = walletCapacityLevel;
    
    m_currentLevel = 1;
    m_currentMoney = 0.0f;
    m_maxCapacity = GetMaxCapacityForLevel(m_currentLevel);
    
    NotifyChanged();
}

void Wallet::Update(float deltaTime) {
    if (m_currentMoney < m_maxCapacity) {
        m_currentMoney += GetMoneyGenerationRate() * deltaTime;
        if (m_currentMoney > m_maxCapacity) {
            m_currentMoney = static_cast<float>(m_maxCapacity);
        }
        NotifyChanged();
    }
}

void Wallet::AddMoney(float amount) {
    m_currentMoney += amount;
    if (m_currentMoney > m_maxCapacity) {
        m_currentMoney = static_cast<float>(m_maxCapacity);
    }
    NotifyChanged();
}

bool Wallet::SpendMoney(int amount) {
    if (m_currentMoney >= amount) {
        m_currentMoney -= amount;
        NotifyChanged();
        return true;
    }
    return false;
}

bool Wallet::Upgrade() {
    if (m_currentLevel >= m_maxLevel) {
        LOG_INFO("Wallet is already at maximum level.");
        return false;
    }
    
    int cost = GetUpgradeCost();
    if (SpendMoney(cost)) {
        m_currentLevel++;
        m_maxCapacity = GetMaxCapacityForLevel(m_currentLevel);
        LOG_INFO("Wallet upgraded to level %d. Max Capacity: %d", m_currentLevel, m_maxCapacity);
        NotifyChanged();
        return true;
    }
    
    LOG_INFO("Not enough money to upgrade wallet.");
    return false;
}

int Wallet::GetMaxCapacityForLevel(int level) const {
    // 基礎容量由科技等級決定，每升一級戰鬥中錢包，容量會按比例放大
    int baseCapacity = 1000 + (m_walletCapacityLevel - 1) * 200;
    return baseCapacity + (level - 1) * (baseCapacity / 2);
}

float Wallet::GetMoneyGenerationRate() const {
    // 賺錢速度由科技等級和當前錢包等級決定
    float baseRate = 20.0f + (m_workerCatEfficiencyLevel - 1) * 5.0f;
    return baseRate * (1.0f + (m_currentLevel - 1) * 0.2f);
}

int Wallet::GetUpgradeCost() const {
    if (IsMaxLevel()) return 0;
    // 升級花費根據當前等級
    return 300 * m_currentLevel + 200 * (m_currentLevel - 1);
}

void Wallet::NotifyChanged() {
    if (m_onWalletChanged) {
        m_onWalletChanged(static_cast<int>(m_currentMoney), m_maxCapacity, m_currentLevel, GetUpgradeCost());
    }
}
