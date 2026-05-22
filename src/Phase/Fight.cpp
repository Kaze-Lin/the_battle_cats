#include "Phase/Fight.hpp"
#include "DatabaseManager.hpp"
#include "LevelManager.hpp"
#include "EntityManager.hpp"
#include "UserManager.hpp"
#include "Util/Time.hpp"

Fight::Fight(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/stage_background/Bg000.png",
            -10.0F);
    AddChild(m_BackgroundImage);

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
    m_WalletMoneyText = std::make_shared<Text>(
        TextThemeDetail::DefaultFontSize,
        "Money: 0 / 0",
        50
    );
    AddChild(m_WalletMoneyText);

    // Wallet UI - Upgrade Cost Text
    m_WalletLevelText = std::make_shared<Text>(
        TextThemeDetail::DefaultFontSize,
        "$ 0",
        100
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
    m_LaserEffectText = std::make_shared<Text>(
        TextThemeDetail::DefaultFontSize * 2,
        "============ LASER ============",
        100,
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

                        // 對範圍內的敵人造成傷害並擊退
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

    // 連接 Cannon 的事件來更新 UI
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

    // 初始化 Cannon 以觸發第一次 Callback
    m_Cannon->Initialize(cannonAttack, cannonRange, cannonCharge);

    // 連接 Wallet 的事件來更新 UI
    std::weak_ptr<Text> weakMoneyText = m_WalletMoneyText;
    std::weak_ptr<Text> weakLevelText = m_WalletLevelText;
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
            []() { LOG_INFO("Pause Button Clicked!"); });
    m_b_Pause->SetZIndex(50.0F);
    m_b_Pause->ScaleSize({ORIGINAL_SCALING + 0.1F, ORIGINAL_SCALING + 0.1F});

    // stage name
    m_StageName = std::make_shared<Text>(
        TextThemeDetail::DefaultFontSize,
        "台灣",
        -8
        );
    AddChild(m_StageName);

    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    // button image (with interaction image)
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

    // 設定敵人死亡時掉落金錢
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
               auto catHpText = std::make_shared<Util::GameObject>(
               std::make_shared<Util::Text>(
                    TextThemeDetail::DefaultBackgroundFont,
                    16,
                    std::to_string(catBase->GetCurrentHp()) + "/" + std::to_string(catBase->GetMaxHp()),
                    Util::Color::FromName(Util::Colors::BLACK)
                ),
                10
               );
               catHpText->m_Transform.translation = {catBase->GetPositionX() + 0.0f, 100.0f};
               AddChild(catHpText);

                std::weak_ptr<Util::GameObject> weakCatHpText = catHpText;
                             catBase->SetOnHealthChanged([weakCatHpText](int current, int max) {
                                 if (auto text = weakCatHpText.lock()) {
                                     if (auto textComp = std::dynamic_pointer_cast<Util::Text>(text->GetDrawable())) {
                                         textComp->SetText(std::to_string(current) + "/" + std::to_string(max));
                                     }
                                 }
                             });
            }

            // [敵方塔血量顯示]
            if (enemyBase) {
                auto enemyHpText = std::make_shared<Util::GameObject>(
                std::make_shared<Util::Text>(
                TextThemeDetail::DefaultBackgroundFont,
                16,
                std::to_string(enemyBase->GetCurrentHp()) + "/" + std::to_string(enemyBase->GetMaxHp()),
                Util::Color::FromName(Util::Colors::BLACK)
            ),
            10
           );
                enemyHpText->m_Transform.translation = {enemyBase->GetPositionX() + 20.0f, 50.0f};
               AddChild(enemyHpText);

               std::weak_ptr<Util::GameObject> weakEnemyHpText = enemyHpText;
               enemyBase->SetOnHealthChanged([weakEnemyHpText](int current, int max) {
                   if (auto text = weakEnemyHpText.lock()) {
                       if (auto textComp = std::dynamic_pointer_cast<Util::Text>(text->GetDrawable())) {
                           textComp->SetText(std::to_string(current) + "/" + std::to_string(max));
                       }
                   }
               });
            }

        m_StageName->SetText(stage->stageName);

        auto textSize = m_StageName->GetSize();
        const auto stageNameXNew = pauseX + pauseSize.x / 2 + textSize.x / 2.0F + 15.0F;
        m_StageName->Place({stageNameXNew, stageNameY});

        // TODO: deploy cats
        // Deploy Cats
        std::vector<int> ids = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        std::reverse(ids.begin(), ids.end());
        DeployCatButton(ids);
    }
}

void Fight::Update() {
    Phase::Update();

    if (m_isGameOver) {
        return;
    }

    float realDeltaTime = Util::Time::GetDeltaTime();

    if (Util::Input::IsKeyDown(Util::Keycode::D)) {
        if (m_timeScale != 5.0f) {
            m_timeScale = 5.0f;
            LOG_INFO("Time Scale changed to: 5.0x Speed!");
        }
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::A)) {
        if (m_timeScale != 1.0f) {
            m_timeScale = 1.0f;
            LOG_INFO("Time Scale changed to: Normal Speed");
        }
    }

    float gameDeltaTime = realDeltaTime * m_timeScale;

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

    LevelManager::GetInstance().Update(gameDeltaTime, 1.0f);
    EntityManager::GetInstance().Update(gameDeltaTime);

    if (EntityManager::GetInstance().IsPlayerWin()) {
        LOG_INFO("VICTORY! The Player has destroyed the Enemy Base!");
        m_isGameOver = true;

        // 更新關卡進度
        if (auto user = UserManager::GetInstance().GetCurrentUser()) {
            const StageData* currentStage = LevelManager::GetInstance().GetCurrentStage();
            if (currentStage && currentStage->stageId > user->progress.highestStageCleared) {
                user->progress.highestStageCleared = currentStage->stageId;
                LOG_INFO("User progress updated! Highest stage cleared: %d", currentStage->stageId);
            }
        }
    }
    else if (EntityManager::GetInstance().IsEnemyWin()) {
        LOG_INFO("DEFEAT! The Enemy has destroyed the Cat Base!");
        m_isGameOver = true;

    }

    if (Util::Input::IsKeyDown(Util::Keycode::X)) {
        EntityManager::GetInstance().SpawnCat(0, 1, 0);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::Z)) {
        EntityManager::GetInstance().SpawnEnemy(0, 100);
    }
}

void Fight::DeployCatButton(std::vector<int> IDs) {
    // ID, cat
    std::unordered_map<int, const UnitData*> cats;
    for (auto i: IDs) {
        cats[i] = DatabaseManager::GetInstance().GetCatData(i);
    }

    if (!cats.empty() || (cats.size() > 0 && cats.size() < 10)) {
        for (auto cat: cats)
        {
            if (cat.second == nullptr) {
                LOG_WARN("cannot find cat with ID " + std::to_string(cat.first));
                continue;
            }

            std::weak_ptr<Wallet> weakWallet = m_Wallet;
            m_gen_b_cats.push_back(
                std::make_shared<Button>(
                        RESOURCE_DIR + cat.second->catGenButton[0], // the '0' should be changed
                        [cat, weakWallet](){
                            if (auto w = weakWallet.lock()) {
                                int cost = cat.second->forms[0].cost; // the '0' should be changed
                                if (w->SpendMoney(cost)) {
                                    EntityManager::GetInstance().SpawnCat(
                                        cat.first,
                                        cat.second->maxLevel,
                                        cat.second->forms[0].formIndex); // the '0' should be changed
                                } else {
                                    LOG_INFO("Not enough money to spawn %s! Cost: %d", cat.second->nameInternal.c_str(), cost);
                                }
                            }
                        },
                        99
                    )
                );
        }
        //layout
        const auto X = -245.0F;
        const auto Y = -250.0F;
        m_gen_b_cats[0]->Place({X, Y});
        AddChild(m_gen_b_cats[0]);

        auto catButtonX = X;
        auto catButtonY = Y;

        for (int i = 1; i < m_gen_b_cats.size(); i++) {
            auto photoSize = m_gen_b_cats[i]->GetSize();
            if (i <= 4 || (i > 5 && i <= 10)) {
                catButtonX += m_gen_b_cats[i - 1]->GetSize().x / 2.0F + photoSize.x / 2.0F + 15.0F;
            } else if (i == 5) {
                catButtonX = X;
                catButtonY = catButtonY - m_gen_b_cats[i - 5]->GetSize().y / 2.0F - photoSize.y / 2.0F - 15.0F;
            }
            m_gen_b_cats[i]->Place({catButtonX, catButtonY});
            AddChild(m_gen_b_cats[i]);
        }
    }
}
