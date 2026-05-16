#ifndef WALLET_HPP
#define WALLET_HPP

#include <functional>
#include <algorithm>
#include "Util/Logger.hpp"

class Wallet {
private:
    float m_currentMoney;
    int m_maxCapacity;
    int m_currentLevel;
    const int m_maxLevel = 8;
    
    int m_workerCatEfficiencyLevel;
    int m_walletCapacityLevel;
    
    std::function<void(int, int, int, int)> m_onWalletChanged; // (currentMoney, maxCapacity, currentLevel, upgradeCost)
    
    int GetMaxCapacityForLevel(int level) const;
    float GetMoneyGenerationRate() const;
    int GetUpgradeCost() const;

    void NotifyChanged();

public:
    Wallet();
    ~Wallet() = default;

    void Initialize(int workerCatEfficiencyLevel, int walletCapacityLevel);
    
    void Update(float deltaTime);
    
    void AddMoney(float amount);
    bool SpendMoney(int amount);
    bool Upgrade();
    
    int GetCurrentMoney() const { return static_cast<int>(m_currentMoney); }
    int GetMaxCapacity() const { return m_maxCapacity; }
    int GetCurrentLevel() const { return m_currentLevel; }
    int GetNextUpgradeCost() const { return GetUpgradeCost(); }
    bool IsMaxLevel() const { return m_currentLevel >= m_maxLevel; }
    
    void SetOnWalletChanged(std::function<void(int, int, int, int)> callback) {
        m_onWalletChanged = callback;
        NotifyChanged();
    }
};

#endif // WALLET_HPP
