#ifndef RESOURCEDISPLAY_HPP
#define RESOURCEDISPLAY_HPP

#include "Util/GameObject.hpp"
#include "Component/Text.hpp"
#include "UserManager.hpp"
#include "SystemInfo.hpp"

class ResourceDisplay : public Util::GameObject, public IStateful {
private:
    std::shared_ptr<Text> m_xpText;
    std::shared_ptr<Text> m_catfoodText;
    int m_lastXP = -1;
    int m_lastCatFood = -1;

public:
    ResourceDisplay() : Util::GameObject(nullptr, 10.0f) { // high z-index to overlay
        m_xpText = std::make_shared<Text>(50, "0", 10.0f);
        m_catfoodText = std::make_shared<Text>(50, "0", 10.0f);

        // Place at top-right (XP) near the XP button (260.0F, 347.0F)
        m_xpText->Place({470.0f, 347.0f});
        
        // Place at bottom-right (CatFood) near the CatCan button (320.0F, -353.0F)
        m_catfoodText->Place({490.0f, -353.0f});

        AddChild(m_xpText);
        AddChild(m_catfoodText);
    }

    void Update() override {
        int currentXP = UserManager::GetInstance().GetXP();
        int currentCatFood = UserManager::GetInstance().GetCatFood();

        if (currentXP != m_lastXP) {
            m_xpText->SetText(std::to_string(currentXP));
            m_lastXP = currentXP;
        }

        if (currentCatFood != m_lastCatFood) {
            m_catfoodText->SetText(std::to_string(currentCatFood));
            m_lastCatFood = currentCatFood;
        }
    }
};

#endif // RESOURCEDISPLAY_HPP