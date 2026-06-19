#include "Phase/Fight.hpp"
#include <cmath>
#include "DatabaseManager.hpp"
#include "LevelManager.hpp"
#include "EntityManager.hpp"
#include "UserManager.hpp"
#include "Util/Time.hpp"
#include "PhaseManager.hpp"
#include "Component/HealthBar.hpp"

namespace {
    std::vector<int> GetTeamReverse() {
        auto teamBuild = UserManager::GetInstance().GetCurrentUser()->progress.teamBuild;
        std::vector<int> team;
        for (int i = 0; i < 10; i++) team.push_back(teamBuild[i]);

        return team;
    }
}

CatSlotController::CatSlotController(int cost, float maxCd)
    : m_Cost(cost), m_MaxCd(maxCd), m_CurrentCd(0.0f), m_LastSec(-1) {
    m_CdText = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize,
        " ",
        81.0f,
        Util::Color::FromName(Util::Colors::RED)
    );
    m_CdText->SetVisible(false);

    m_CostText = std::make_shared<TwoLayerText>(
        18,
        "$" + std::to_string(m_Cost),
        85.0f,
        Util::Color::FromName(Util::Colors::BLACK)
    );
}

void CatSlotController::SetButton(std::shared_ptr<Button> btn) {
    m_Button = btn;
}

void CatSlotController::Update(float gameDeltaTime) {
    if (m_CurrentCd > 0.0f) {
        m_CurrentCd -= gameDeltaTime;

        if (m_CurrentCd <= 0.0f) {
            m_CurrentCd = 0.0f;
            m_CdText->SetVisible(false);
            m_LastSec = -1; // Reset cache
        } else {
            // [Performance Optimization]
            // Calculate ceiling seconds. Only update text texture if the integer changes.
            int currentSec = static_cast<int>(std::ceil(m_CurrentCd));
            if (currentSec != m_LastSec) {
                m_LastSec = currentSec;
                m_CdText->SetText(std::to_string(currentSec) + "s");
            }
        }
    }
}

void CatSlotController::StartCooldown() {
    m_CurrentCd = m_MaxCd;
    m_LastSec = -1; // Force immediate text update
    m_CdText->SetVisible(true);
}

bool CatSlotController::IsReady() const {
    return m_CurrentCd <= 0.0f;
}

void CatSlotController::Place(const glm::vec2& p) {
    if (m_Button) m_Button->Place(p);
    if (m_CdText) m_CdText->Place(p);
    if (m_CostText) {
        float offsetY = -20.0f;
        if (m_Button) {
            offsetY = -(m_Button->GetSize().y / 2.0f) + 12.0f;
        }
        m_CostText->Place({p.x, p.y + offsetY});
    }
}

Fight::~Fight() {
    EntityManager::GetInstance().ClearAllEntities();
    EntityManager::GetInstance().SetSceneNode(nullptr);
    LOG_INFO("Fight Phase Destructed: Cleared all entities and scene node.");
}

Fight::Fight(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/stage_background/Bg000.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_replay = UserManager::GetInstance().GetCurrentUser()->progress.highestStageCleared >= LevelManager::GetInstance().GetCurrentStage()->stageId;

    // Wallet 初始化
    m_Wallet = std::make_shared<Wallet>();
    int workerCatEfficiency = 1;
    int walletCapacity = 1;
    if (auto user = UserManager::GetInstance().GetCurrentUser()) {
        workerCatEfficiency = user->baseUpgrades.workerCatEfficiency;
        walletCapacity = user->baseUpgrades.walletCapacity;
    }
    m_Wallet->Initialize(workerCatEfficiency, walletCapacity);

    // Wallet UI - Money Text
    m_WalletMoneyText = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize,
        "Money: 0 / 0",
        50
    );
    AddChild(m_WalletMoneyText);

    // Wallet UI - Upgrade Cost Text
    m_WalletLevelText = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize,
        "$ 0",
        80
    );
    AddChild(m_WalletLevelText);

    // Cannon 參數讀取
    int cannonAttack = 1, cannonRange = 1, cannonCharge = 1;
    if (auto user = UserManager::GetInstance().GetCurrentUser()) {
        cannonAttack = user->baseUpgrades.cannonAttack;
        cannonRange = user->baseUpgrades.cannonRange;
        cannonCharge = user->baseUpgrades.cannonCharge;
    }
    m_Cannon = std::make_shared<Cannon>();

    // Laser Effect UI
    m_LaserEffectText = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize * 2,
        "============ LASER ============",
        80,
        Util::Color::FromName(Util::Colors::YELLOW)
    );
    // 預設不加入場景，開砲時再 AddChild

    std::weak_ptr<Wallet> weakWallet = m_Wallet;
    m_b_RickUpgrade =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Richupgrade_1.png",
            [weakWallet]() {
                if (auto w = weakWallet.lock()) {
                    w->Upgrade();
                }
            },
            50.0F);

    m_b_RickUpgrade->ScaleSize({ORIGINAL_SCALING + 0.5F, ORIGINAL_SCALING + 0.5F});

    std::weak_ptr<Cannon> weakCannon = m_Cannon;
    m_b_CannonCharge =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Cannoncharge_0.png",
            [weakCannon, this]() {
                if (auto c = weakCannon.lock()) {
                    if (c->Fire()) {
                        LOG_INFO("Cannon Fired!");
                        m_laserTimer = 1.0f;
                        if (m_LaserEffectText) {
                            // 將雷射特效加到畫面上
                            this->AddChild(m_LaserEffectText);
                        }

                        auto catBase = EntityManager::GetInstance().GetCatBase();
                        if (catBase) {
                            float cannonRange = c->GetRange();
                            float damage = c->GetDamage();
                            float baseX = catBase->GetPositionX();

                            auto targets = EntityManager::GetInstance().GetEntitiesInRange(Faction::Enemy, baseX - cannonRange, baseX + cannonRange);
                            for (auto target : targets) {
                                target->TakeDamage(static_cast<int>(damage));
                                target->ForceKnockback();
                            }
                        }
                    }
                }
            },
            50.0F);
    m_b_CannonCharge->ScaleSize({ORIGINAL_SCALING + 0.5F, ORIGINAL_SCALING + 0.5F});

    std::weak_ptr<Button> weakCannonBtn = m_b_CannonCharge;
    m_Cannon->SetOnChargeChanged([weakCannonBtn](int chargeState) {
        if (auto btn = weakCannonBtn.lock()) {
            std::string imagePath;
            if (chargeState >= 10) {
                imagePath = RESOURCE_DIR "/phase/fight/Cannoncharge_full.png";
            } else {
                imagePath = RESOURCE_DIR "/phase/fight/Cannoncharge_" + std::to_string(chargeState) + ".png";
            }
            btn->SetImage(imagePath);
            LOG_INFO("Cannon image updated to: %s", imagePath.c_str());
        }
    });

    m_Cannon->Initialize(cannonAttack, cannonRange, cannonCharge);

    std::weak_ptr<TwoLayerText> weakMoneyText = m_WalletMoneyText;
    std::weak_ptr<TwoLayerText> weakLevelText = m_WalletLevelText;
    std::weak_ptr<Button> weakUpgradeBtn = m_b_RickUpgrade;
    m_Wallet->SetOnWalletChanged([weakMoneyText, weakLevelText, weakUpgradeBtn](int current, int max, int level, int upgradeCost) {
        if (auto mText = weakMoneyText.lock()) {
            mText->SetText("Money: " + std::to_string(current) + " / " + std::to_string(max));
        }
        if (auto lText = weakLevelText.lock()) {
            if (level >= 8) {
                lText->SetText("MAX");
            } else {
                lText->SetText("$ " + std::to_string(upgradeCost));
            }
        }
        if (auto btn = weakUpgradeBtn.lock()) {
            std::string imagePath;
            if (level >= 8) {
                imagePath = RESOURCE_DIR "/phase/fight/Richupgrade_Max.png";
            } else {
                std::string suffix = (current >= upgradeCost) ? "_enable.png" : ".png";
                imagePath = RESOURCE_DIR "/phase/fight/Richupgrade_" + std::to_string(level) + suffix;
            }
            btn->SetImage(imagePath);
        }
    });

    // button image (with interaction image)
    m_b_Pause =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/pause.png",
            [this]() {
                LOG_INFO("Pause Button Clicked!");
                if (this->m_CurrentState == SubState::PLAYING) {
                this->m_CurrentState = SubState::PAUSED;
                this->SetPauseMenuVisible(true);
        } else {
            this->m_CurrentState = SubState::PLAYING;
            this->SetPauseMenuVisible(false);
        }
            });
    m_b_Pause->SetZIndex(50.0F);
    m_b_Pause->ScaleSize({ORIGINAL_SCALING + 0.1F, ORIGINAL_SCALING + 0.1F});

    m_PauseMenuBg = std::make_shared<BackgroundImage>(RESOURCE_DIR "/stage_background/Bg000.png", 90.0f);

    m_ResumeBtn = std::make_shared<TextButton>(
        32, "Resume", 99.0f,
        [this]() {
            this->m_CurrentState = SubState::PLAYING;
            this->SetPauseMenuVisible(false);
        });

    m_ExitBtn = std::make_shared<TextButton>(
        32, "Exit to Lobby", 99.0f    ,
        [this]() {
            this->m_DestinationPhase = "CatBase";
        });

    AddChild(m_PauseMenuBg);
    AddChild(m_ResumeBtn);
    AddChild(m_ExitBtn);

    m_PauseMenuBg->AlignWithWindow();
    m_ResumeBtn->Place({0.0f, 50.0f});
    m_ExitBtn->Place({0.0f, -50.0f});

    SetPauseMenuVisible(false);

    // stage name
    m_StageName = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize,
        " ",
        -8
        );
    AddChild(m_StageName);

    m_BackgroundImage->AlignWithWindow();

    const auto pauseX = -1 * System::GetWindowWidth() / 2.0F + m_b_Pause->GetSize().x / 2.0F + 5.0F;
    const auto pauseY = System::GetWindowHeight() / 2.0F - m_b_Pause->GetSize().y / 2.0F - 5.0F;
    m_b_Pause->Place({pauseX, pauseY});
    AddChild(m_b_Pause);

    const auto rickUpgradeX = -1 * System::GetWindowWidth() / 2.0F + 75.0F;
    const auto rickUpgradeY = -1 * System::GetWindowHeight() / 2.0F + m_b_RickUpgrade->GetSize().y / 2.0F - 5.0F;
    m_b_RickUpgrade->Place({rickUpgradeX, rickUpgradeY});
    AddChild(m_b_RickUpgrade);

    // Wallet Texts Layout
    m_WalletLevelText->Place({rickUpgradeX, rickUpgradeY - 40.0f});
    m_WalletMoneyText->Place({459.0f,375.0f});

    const auto cannonChargeX = System::GetWindowWidth() / 2.0F - 75.0F;
    const auto cannonChargeY = -1 * System::GetWindowHeight() / 2.0F + m_b_CannonCharge->GetSize().y / 2.0F - 5.0F;
    m_b_CannonCharge->Place({cannonChargeX, cannonChargeY});
    AddChild(m_b_CannonCharge);

    // stage name
    const auto pauseSize = m_b_Pause->GetSize();
    const auto stageNameX = pauseX + pauseSize.x / 2 + 50.0F;
    const auto stageNameY = pauseY;
    m_StageName->Place({stageNameX, stageNameY});
    AddChild(m_StageName);

    EntityManager::GetInstance().SetSceneNode(this);

    EntityManager::GetInstance().SetOnEnemyDied([weakWallet](Unit* enemy) {
        if (auto w = weakWallet.lock()) {
            if (enemy && enemy->GetFaction() == Faction::Enemy) {
                w->AddMoney(enemy->GetDropGold());
                LOG_INFO("Enemy died. Wallet received %.1f money.", enemy->GetDropGold());
            }
        }
    });

    const StageData* stage = LevelManager::GetInstance().GetCurrentStage();
    if (stage != nullptr) {

        EntityManager::GetInstance().SpawnCatBase(1);

        EntityManager::GetInstance().SpawnEnemyBase(stage->enemyBaseHp, stage->basePath,-545.0f);

         auto catBase = EntityManager::GetInstance().GetCatBase();
         auto enemyBase = EntityManager::GetInstance().GetEnemyBase();

            // [貓咪塔血量顯示]
            if (catBase) {
               auto catHealthBar = std::make_shared<HealthBar>(catBase, glm::vec2{10.0f, 135.0f});
               AddChild(catHealthBar);
            }

            // [敵方塔血量顯示]
            if (enemyBase) {
               auto enemyHealthBar = std::make_shared<HealthBar>(enemyBase, glm::vec2{10.0f, 135.0f});
               AddChild(enemyHealthBar);
            }

        m_StageName->SetText(stage->stageName);

        auto textSize = m_StageName->GetSize();
        const auto stageNameXNew = pauseX + pauseSize.x / 2 + textSize.x / 2.0F + 15.0F;
        m_StageName->Place({stageNameXNew, stageNameY});


        DeployCatButton(GetTeamReverse());
    }

}

void Fight::Update() {
    Phase::Update();

    if (m_isGameOver) {
        return;
    }

    if (m_CurrentState == SubState::PAUSED) {
        return;
    }

    float realDeltaTime = Util::Time::GetDeltaTime();

    if (Util::Input::IsKeyDown(Util::Keycode::D)) {
        if (m_timeScale != 6.0f) {
            m_timeScale = 6.0f;
            LOG_INFO("Time Scale changed to: 6.0x Speed!");
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::A)) {
        if (m_timeScale != 1.0f) {
            m_timeScale = 1.0f;
            LOG_INFO("Time Scale changed to: Normal Speed");
        }
    }

    float gameDeltaTime = realDeltaTime * m_timeScale;

    int currentMoney = m_Wallet ? m_Wallet->GetCurrentMoney() : 0;
    for (auto& slot : m_CatSlots) {
        slot->Update(gameDeltaTime);
        if (slot->GetCostText()) {
            if (currentMoney < slot->GetCost()) {
                slot->GetCostText()->SetColor(Util::Color::FromName(Util::Colors::RED));
            } else {
                slot->GetCostText()->SetColor(Util::Color::FromName(Util::Colors::YELLOW));
            }
        }
    }

    // Wallet Passive Money Generation
    if (m_Wallet) {
        m_Wallet->Update(gameDeltaTime);
    }

    // Cannon Charge Generation
    if (m_Cannon) {
        m_Cannon->Update(gameDeltaTime);
    }

    // Laser Effect Timer
    if (m_laserTimer > 0.0f) {
        m_laserTimer -= gameDeltaTime;
        if (m_laserTimer <= 0.0f) {
            m_laserTimer = 0.0f;
            if (m_LaserEffectText) {
                this->RemoveChild(m_LaserEffectText);
            }
        }
    }

    float enemyBaseHpPercent = 100.0f;
    if (auto enemyBase = EntityManager::GetInstance().GetEnemyBase()) {
        if (enemyBase->GetMaxHp() > 0) {
            enemyBaseHpPercent = (static_cast<float>(enemyBase->GetCurrentHp()) / enemyBase->GetMaxHp()) * 100.0f;
        }
    }
    LevelManager::GetInstance().Update(gameDeltaTime, enemyBaseHpPercent);
    EntityManager::GetInstance().Update(gameDeltaTime);

    if (EntityManager::GetInstance().IsPlayerWin() && !m_isGameOver) {
        LOG_INFO("VICTORY! The Player has destroyed the Enemy Base!");
        UserManager::GetInstance().AddXP(LevelManager::GetInstance().GetStageExperience());
        m_isGameOver = true;

        if (auto user = UserManager::GetInstance().GetCurrentUser()) {
            const StageData* currentStage = LevelManager::GetInstance().GetCurrentStage();
            if (currentStage && currentStage->stageId > user->progress.highestStageCleared) {
                const ChapterData* currentChapter = DatabaseManager::GetInstance().GetChapterData(user->progress.currentStage[0]);
                
                if (currentChapter != nullptr) {
                    user->progress.highestStageCleared = currentStage->stageId;

                    if (currentStage->stageId < currentChapter->stages.size()) {
                        user->progress.currentStage[1] = currentStage->stageId + 1;
                    } else {
                        user->progress.currentStage[1] = currentStage->stageId;
                    }
                    LOG_INFO("User progress updated! Highest stage cleared: %d", currentStage->stageId);
                }
            }
        }
        ShowSettlementScreen(true);
    }
    else if (EntityManager::GetInstance().IsEnemyWin() && !m_isGameOver) {
        LOG_INFO("DEFEAT! The Enemy has destroyed the Cat Base!");
        m_isGameOver = true;
        ShowSettlementScreen(false);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::X)) {
        EntityManager::GetInstance().SpawnCat(0, 10000, 2);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
        EntityManager::GetInstance().SpawnEnemy(0, 10000);
    }
}

void Fight::DeployCatButton(std::vector<int> IDs) {
    const float startX = -245.0F;
    const float startY = -250.0F;

    for (int i = 0; i < 10; ++i) {
        if (i >= IDs.size() || IDs[i] == -1) {
            continue; // Skip rendering for empty slots
        }

        int catId = IDs[i];
        const auto* catData = DatabaseManager::GetInstance().GetCatData(catId);
        
        if (catData == nullptr) {
            LOG_WARN("cannot find cat with ID " + std::to_string(catId));
            continue;
        }

        auto currentUser = UserManager::GetInstance().GetCurrentUser();
        int currentForm = 1;
        int level = 1;
        if (currentUser) {
            for (const auto& cat : currentUser->unlockedCats) {
                if (cat.catId == catId) {
                    currentForm = cat.currentForm;
                    level = cat.level;
                    break;
                }
            }
        }
        int formIndexForData = currentForm - 1;

        int cost = catData->forms[formIndexForData].cost;
        float maxCd = catData->forms[formIndexForData].rechargeTime / 30.0f;

        auto slot = std::make_shared<CatSlotController>(cost, maxCd);
        std::weak_ptr<CatSlotController> weakSlot = slot;
        std::weak_ptr<Wallet> weakWallet = m_Wallet;

        auto btn = std::make_shared<Button>(
            RESOURCE_DIR + catData->catGenButton[formIndexForData],
            [catId, weakWallet, weakSlot, cost, level, formIndexForData]() {
                if (auto s = weakSlot.lock()) {
                    if (!s->IsReady()) {
                        LOG_INFO("Cat is cooling down!");
                        return;
                    }
                    if (auto w = weakWallet.lock()) {
                        if (w->SpendMoney(cost)) {
                            EntityManager::GetInstance().SpawnCat(
                                catId,
                                level,
                                formIndexForData);

                            s->StartCooldown();
                        } else {
                            LOG_INFO("Not enough money!");
                        }
                    }
                }
            },
            80.0f
        );

        slot->SetButton(btn);
        m_CatSlots.push_back(slot);

        AddChild(slot->GetButton());
        AddChild(slot->GetCdText());
        AddChild(slot->GetCostText());
        float btnWidth = btn->GetSize().x;
        float btnHeight = btn->GetSize().y;
        
        float catButtonX = startX;
        float catButtonY = startY;

        if (i > 0 && i < 5) {
            catButtonX = startX + (btnWidth + 15.0F) * i;
        } else if (i >= 5) {
            catButtonX = startX + (btnWidth + 15.0F) * (i - 5);
            catButtonY = startY - btnHeight - 15.0F;
        }

        slot->Place({catButtonX, catButtonY});
    }
}

void Fight::SetPauseMenuVisible(bool visible) {
    if (m_PauseMenuBg) m_PauseMenuBg->SetVisible(visible);
    if (m_ResumeBtn) m_ResumeBtn->SetVisible(visible);
    if (m_ExitBtn) m_ExitBtn->SetVisible(visible);
}

void Fight::ShowSettlementScreen(bool isVictory) {
    EntityManager::GetInstance().ClearAllEntities();

    for (auto it = m_Children.begin(); it != m_Children.end(); ) {
        if (*it != m_BackgroundImage) {
            it = m_Children.erase(it);
        } else {
            ++it;
        }
    }
    std::string textStr = isVictory ? "VICTORY!" : "DEFEAT!";
    bool checkUnlockcat = LevelManager::GetInstance().GetCurrentStage()->unlockCat != -1;
    if (isVictory and checkUnlockcat and !m_replay){
            UserManager::GetInstance().addCat(LevelManager::GetInstance().GetCurrentStage()->unlockCat);
            //LOG_INFO("Get Unlock Cat %s",LevelManager::GetInstance().GetCurrentStage()->unlockCat);
        }

    auto color = isVictory ? Util::Color::FromName(Util::Colors::YELLOW) : Util::Color::FromName(Util::Colors::RED);
    m_SettlementText = std::make_shared<TwoLayerText>(
        TextThemeDetail::DefaultFontSize * 3,
        textStr,
        100,
        color
    );
    m_SettlementText->Place({0.0f, 100.0f});
    AddChild(m_SettlementText);

    m_SettlementExitBtn = std::make_shared<TextButton>(
        32, "Exit to Lobby", 100.0f,
        [this]() {
            this->m_DestinationPhase = "CatBase";
        }
    );
    m_SettlementExitBtn->Place({0.0f, -50.0f});
    AddChild(m_SettlementExitBtn);
}

std::shared_ptr<Phase> Fight::GetDestinationPhase() {
    return PhaseManager::GetNextPhase("Fight", this->m_DestinationPhase);
}