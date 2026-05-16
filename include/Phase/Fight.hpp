#ifndef FIGHT_HPP
#define FIGHT_HPP

#include "Phase/Phase.hpp"

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/Wallet.hpp"
#include "Component/Cannon.hpp"

#include "ImageScaling.hpp"
#include "SystemInfo.hpp"
#include "Component/Text.hpp"

class Fight: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Pause;
    std::shared_ptr<Button> m_b_RickUpgrade;
    std::shared_ptr<Button> m_b_CannonCharge;

    // stage name
    std::shared_ptr<Text> m_StageName;
    
    // wallet and its UI
    std::shared_ptr<Wallet> m_Wallet;
    std::shared_ptr<Text> m_WalletMoneyText;
    std::shared_ptr<Text> m_WalletLevelText;

    // cannon and its UI
    std::shared_ptr<Cannon> m_Cannon;
    std::shared_ptr<Text> m_LaserEffectText;
    float m_laserTimer = 0.0f;

    float m_timeScale = 1.0f;

    // generate cat button
    std::vector<std::shared_ptr<Button>> m_gen_b_cats;

    // IDs are in the team build
    void DeployCatButton(std::vector<int> IDs);

    bool m_isGameOver = false;

public:

    Fight();
    ~Fight() override = default;

    void Update() override;
};

#endif //FIGHT_HPP
