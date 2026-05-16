#ifndef CANNON_HPP
#define CANNON_HPP

#include <functional>
#include <string>
#include "Util/Logger.hpp"

class Cannon {
private:
    float m_chargeTimer;
    float m_maxChargeTime;
    
    int m_attackLevel;
    int m_rangeLevel;
    int m_chargeLevel;
    
    int m_currentChargeState; // 0 to 10 (10 means full)
    
    std::function<void(int)> m_onChargeChanged; // (chargeState)

    void NotifyChanged();

public:
    Cannon();
    ~Cannon() = default;

    void Initialize(int attackLevel, int rangeLevel, int chargeLevel);
    void Update(float deltaTime);
    
    bool Fire(); // returns true if fired successfully
    
    float GetDamage() const;
    float GetRange() const;
    bool IsFull() const { return m_currentChargeState >= 10; }
    
    void SetOnChargeChanged(std::function<void(int)> callback) {
        m_onChargeChanged = callback;
        NotifyChanged();
    }
};

#endif // CANNON_HPP
