#include "Phase/StageSelection.hpp"

#include "PhaseManager.hpp"

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

    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    m_StageSelectionBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_StageSelectionBanner->GetSize().y / 2.0;
    m_StageSelectionBanner->Place({0.0F, upgradeBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1 * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

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

    const auto catcanX = 320.0F;
    const auto catcanY = -353.0F;
    m_b_CatCan->Place({catcanX, catcanY});
    AddChild(m_b_CatCan);

}

std::shared_ptr<Phase> StageSelection::GetDestinationPhase() {
    if (this->m_DestinationPhase == "")
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("StageSelection", this->m_DestinationPhase);
}

void StageSelection::ToFight() {
    this->m_DestinationPhase = "Fight";
}

void StageSelection::ToPropsStore() {
    this->m_DestinationPhase = "PropsStore";
}

void StageSelection::GoBack() {
    this->m_DestinationPhase = "";
}
