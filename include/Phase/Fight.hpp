#ifndef FIGHT_HPP
#define FIGHT_HPP

#include "Phase/Phase.hpp"

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/Wallet.hpp"
#include "Component/Cannon.hpp"

#include "ImageScaling.hpp"
#include "SystemInfo.hpp"
#include "Component/TwoLayerText.hpp"
#include "Component/TextButton.hpp"
#include "Component/ResourceDisplay.hpp"

#include "AudioManager.hpp"

class CatSlotController {
private:
    std::shared_ptr<Util::BGM> m_BGM;
    std::shared_ptr<Util::BGM> m_PressButton;

    std::shared_ptr<Button> m_Button;
    std::shared_ptr<Text> m_CdText;
    std::shared_ptr<TwoLayerText> m_CostText;
    float m_CurrentCd = 0.0f;
    float m_MaxCd = 0.0f;
    int m_Cost = 0;

    int m_LastSec = -1;

public:
    CatSlotController(int cost, float maxCd);

    void SetButton(std::shared_ptr<Button> btn);
    void Update(float gameDeltaTime);
    void StartCooldown();
    bool IsReady() const;

    std::shared_ptr<Button> GetButton() const { return m_Button; }
    std::shared_ptr<Text> GetCdText() const { return m_CdText; }
    std::shared_ptr<TwoLayerText> GetCostText() const { return m_CostText; }
    int GetCost() const { return m_Cost; }

    void Place(const glm::vec2& p);
};

class Fight: public Phase {
private:
    enum class SubState {
        PLAYING,
        PAUSED
    };
    SubState m_CurrentState = SubState::PLAYING;

    std::shared_ptr<BackgroundImage> m_PauseMenuBg;
    std::shared_ptr<TextButton> m_ResumeBtn;
    std::shared_ptr<TextButton> m_ExitBtn;

    void SetPauseMenuVisible(bool visible);

    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Pause;
    std::shared_ptr<Button> m_b_RickUpgrade;
    std::shared_ptr<Button> m_b_CannonCharge;

    // stage name
    std::shared_ptr<TwoLayerText> m_StageName;

    // wallet and its UI
    std::shared_ptr<Wallet> m_Wallet;
    std::shared_ptr<TwoLayerText> m_WalletMoneyText;
    std::shared_ptr<TwoLayerText> m_WalletLevelText;

    // cannon and its UI
    std::shared_ptr<Cannon> m_Cannon;
    std::shared_ptr<TwoLayerText> m_LaserEffectText;
    float m_laserTimer = 0.0f;

    float m_timeScale = 1.0f;

    // generate cat button
    std::vector<std::shared_ptr<CatSlotController>> m_CatSlots;

    // IDs are in the team build
    void DeployCatButton(std::vector<int> IDs);


    bool m_isGameOver = false;

    bool m_replay = false;

    // 結算畫面元件
    std::shared_ptr<TwoLayerText> m_SettlementText;
    std::shared_ptr<TextButton> m_SettlementExitBtn;
    void ShowSettlementScreen(bool isVictory);

public:
    std::shared_ptr<Phase> GetDestinationPhase() override;

    Fight();
    ~Fight() override;

    void Update() override;
};
#endif //FIGHT_HPP
