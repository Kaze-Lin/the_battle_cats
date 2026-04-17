#include "Phase/Fight.hpp"
#include "DatabaseManager.hpp"
#include "LevelManager.hpp"
#include "EntityManager.hpp"
#include "Util/Time.hpp"

Fight::Fight(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/stage_background/Bg000.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    // button image (with interaction image)
    m_b_Pause =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/pause.png",
            []() { LOG_INFO("Pause Button Clicked!"); });
    m_b_Pause->SetZIndex(50.0F);
    m_b_Pause->ScaleSize({ORIGINAL_SCALING + 0.1F, ORIGINAL_SCALING + 0.1F});

    m_b_RickUpgrade =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Richupgrade_1.png",
            []() { LOG_INFO("Wallet Upgrade Clicked!"); },
            50.0F);

    m_b_RickUpgrade->ScaleSize({ORIGINAL_SCALING + 0.5F, ORIGINAL_SCALING + 0.5F});

    m_b_CannonCharge =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Cannoncharge_0.png",
            []() { LOG_INFO("Cat Cannon Clicked!"); },
            50.0F);
    m_b_CannonCharge->ScaleSize({ORIGINAL_SCALING + 0.5F, ORIGINAL_SCALING + 0.5F});

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

    const StageData* stage = LevelManager::GetInstance().GetCurrentStage();
    if (stage != nullptr) {

        EntityManager::GetInstance().SpawnCatBase(1);

        EntityManager::GetInstance().SpawnEnemyBase(stage->enemyBaseHp, stage->basePath,-545.0f);

        m_StageName->SetText(stage->stageName);

        // stage name layout
        const auto pauseSize = m_b_Pause->GetSize();
        auto textSize = m_StageName->GetSize();
        const auto stageNameX = pauseX + pauseSize.x / 2 + textSize.x / 2.0F + 15.0F;
        const auto stageNameY = pauseY;
        m_StageName->Place({stageNameX, stageNameY});
        AddChild(m_StageName);

        // TODO: deploy cats
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

    LevelManager::GetInstance().Update(gameDeltaTime, 1.0f);
    EntityManager::GetInstance().Update(gameDeltaTime);

    if (EntityManager::GetInstance().IsPlayerWin()) {
        LOG_INFO("VICTORY! The Player has destroyed the Enemy Base!");
        m_isGameOver = true;
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

void Fight::DeployCat(std::vector<int> IDs) {
    // ID, cat
    std::unordered_map<int, const UnitData*> cats;

    for (auto i: IDs)
        cats[i] = DatabaseManager::GetInstance().GetCatData(i);

    if (!cats.empty() || (cats.size() > 0 && cats.size() < 10)) {
        for (auto cat: cats) {
            if (cat.second == nullptr)
                LOG_WARN("cannot find cat with ID " + std::to_string(cat.first));
            // m_gen_b_cats.push_back(
            //     std::make_shared<Button>(
            //             cat.second->
            //         )
            //     )
        }
    }
}
