#include "Phase/StageSelection.hpp"

#include "DatabaseManager.hpp"
#include "LevelManager.hpp"
#include "PhaseManager.hpp"
#include "UserManager.hpp"

StageSelection::StageSelection(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/stage_selection/EOC_map.jpg",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_StageSelectionBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/stage_selection/stage_selection_banner.png",
            -7.0F);
    AddChild(m_StageSelectionBanner);

    m_BottomBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/bottom_banner.png",
            -7.0F);
    AddChild(m_BottomBanner);

    // button image (with interaction image)
    m_b_Start =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/stage_selection/start_battle.png",
            [this](){this->ToFight();});
    m_b_Start->ScaleSize({ORIGINAL_SCALING + 0.25F, ORIGINAL_SCALING + 0.25F});

    m_b_Back =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/back_button.png",
            [this](){this->GoBack();});
    m_b_Back->ScaleSize({YELLOW_BUTTON_SCALE + 0.03F, YELLOW_BUTTON_SCALE + 0.03F});


    m_b_PropsStore =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/props_store_button.png",
            [this](){this->ToPropsStore();});
    m_b_PropsStore->ScaleSize({YELLOW_BUTTON_SCALE + 0.05F, YELLOW_BUTTON_SCALE + 0.05F});

    m_b_CatCan =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/cat_can_button.png",
            nullptr);
    m_b_CatCan->ScaleSize({ORIGINAL_SCALING + 0.15F, ORIGINAL_SCALING + 0.15F});

    m_b_XP =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/xp_button.png",
            [this](){this->ToPropsStore();});
    m_b_XP->ScaleSize({YELLOW_BUTTON_SCALE + 0.07F, YELLOW_BUTTON_SCALE + 0.07F});


    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    m_StageSelectionBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_StageSelectionBanner->GetSize().y / 2.0;
    m_StageSelectionBanner->Place({0.0F, upgradeBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1 * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

    // button image (with interaction image)
    const float startX = System::GetWindowWidth() / 2.0 - m_b_Start->GetSize().x / 2.0 - 20.0F;
    const float startY = m_BottomBanner->GetCoordinate().y + m_BottomBanner->GetSize().y / 2.0 + m_b_Start->GetSize().y / 2.0 + 125.0F;
    m_b_Start->Place({startX, startY});
    AddChild(m_b_Start);

    const auto propsStroeButtonX = m_BottomBanner->GetCoordinate().x + 60.0F;
    const auto propsStroeButtonY = m_BottomBanner->GetCoordinate().y + 3.0F;
    m_b_PropsStore->Place({propsStroeButtonX, propsStroeButtonY});
    AddChild(m_b_PropsStore);

    const auto backButtonX = -1.0F * System::GetWindowWidth() / 2.0F + m_b_Back->GetSize().x / 2.0F + 5.0F;
    const auto backButtonY = -1.0F * System::GetWindowHeight() / 2.0F + m_b_Back->GetSize().y / 2.0F + 15.0F;
    m_b_Back->Place({backButtonX, backButtonY});
    AddChild(m_b_Back);

    const glm::vec2 catcanPos = {320.0F, -353.0F};
    m_b_CatCan->Place(catcanPos);
    AddChild(m_b_CatCan);

    const glm::vec2 XPPos = {260.0F, 347.0F};
    m_b_XP->Place(XPPos);
    AddChild(m_b_XP);

    BuildSelectionBar();
}

void StageSelection::Update() {
    Phase::Update();

    m_ScrollManager.Update(m_StageSelectionBar);

    if (!m_StageSelectionBar.empty()) {
        auto midStage = m_StageSelectionBar[m_ScrollManager.GetMiddleIndex(m_StageSelectionBar)];
        m_CurrentChapter = midStage->m_StageID[0];
        m_CurrentStage = midStage->m_StageID[1];
    }
}

std::shared_ptr<Phase> StageSelection::GetDestinationPhase() {
    if (this->m_DestinationPhase.empty())
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("StageSelection", this->m_DestinationPhase);
}

void StageSelection::ToFight() {
    auto user = UserManager::GetInstance().GetCurrentUser();
    if (user) {
        user->progress.currentStage[0] = m_CurrentChapter;
        user->progress.currentStage[1] = m_CurrentStage;
        LevelManager::GetInstance().LoadStage(m_CurrentChapter, m_CurrentStage);
    } else {
        LevelManager::GetInstance().LoadStage(1, 1);
    }
    this->m_DestinationPhase = "Fight";
}

void StageSelection::ToPropsStore() {
    this->m_DestinationPhase = "PropsStore";
}

void StageSelection::GoBack() {
    this->m_DestinationPhase = "";
}

void StageSelection::BuildSelectionBar() {
    auto user = UserManager::GetInstance().GetCurrentUser();

    // Middle Position
    glm::vec2 selectedPos = {0.0F, 256.0F};\

    std::string str = "stage: " + std::to_string(user->progress.currentStage[1]);
    LOG_DEBUG(str);


    for (int i = 1; i <= user->progress.highestStageCleared + 1; i++) {

        auto bg = std::make_shared<StageBlock>(RESOURCE_DIR "/phase/stage_selection/stage_block_background.png");

        bg->m_StageID = {user->progress.currentStage[0], i};

        std::string debug = "chapter: " + std::to_string(bg->m_StageID[0]) + ", stage: " + std::to_string(bg->m_StageID[1]);
        LOG_DEBUG(debug);

        auto stage = DatabaseManager::GetInstance().GetStageData(bg->m_StageID[0], bg->m_StageID[1]);
        bg->Place({selectedPos.x + (bg->GetSize().x + 20.0F) * (i - 1), selectedPos.y});

        bg->m_StageName = std::make_shared<Text>(
            42,
            stage->stageName,
            15.0F
            );
        bg->m_StageName->Place(bg->GetCoordinate());
        bg->AddChild(bg->m_StageName);

        bg->m_Clear = std::make_shared<Text>(24, "Clear!!", 15.0F);
        glm::vec2 clearOffset = {100.0F, -20.0F};
        bg->m_Clear->Place(bg->GetCoordinate() + clearOffset);
        bg->m_Clear->Rotate(45);
        bg->m_Clear->SetColor(Util::Color::FromName(Util::Colors::PINK));
        bg->AddChild(bg->m_Clear);

        if (i == user->progress.highestStageCleared + 1) {
            bg->m_Clear->SetVisible(false);
        }

        m_StageSelectionBar.push_back(bg);
    }

    for (int i = 0; i < user->progress.currentStage[1] - 1; i++) {
        size_t midIndex = m_ScrollManager.GetMiddleIndex(m_StageSelectionBar);
        if (midIndex + 1 < m_StageSelectionBar.size()) {
            size_t rightIndex = midIndex + 1;
            m_ScrollManager.HorizontalMovement(m_StageSelectionBar, m_StageSelectionBar[rightIndex]->GetCoordinate().x);
        }
    }

    for (auto &item: m_StageSelectionBar) AddChild(item);
}