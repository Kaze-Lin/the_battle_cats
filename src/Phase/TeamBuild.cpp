#include "Phase/TeamBuild.hpp"

#include "Phase/Upgrade.hpp"

#include "PhaseManager.hpp"
#include "DatabaseManager.hpp"
#include "UserManager.hpp"

TeamBuild::TeamBuild(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/team_build/team_build_bg.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_TeamBuildBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/team_build/team_build_Banner.png",
            -7.0F);
    AddChild(m_TeamBuildBanner);

    m_BottomBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/bottom_banner.png",
            -7.0F);
    AddChild(m_BottomBanner);

    m_SubTitleBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/upgrade/top_left_banner.png",
            20.0F);
    m_SubTitleText = std::make_shared<Text>(
        30,
        " ",
        25.0F,
        Util::Color::FromName(Util::Colors::WHITE));
    m_SubTitleBanner->AddChild(m_SubTitleText);
    AddChild(m_SubTitleBanner);

    // button image (with interaction image)
    m_b_Back =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/back_button.png",
            [this](){this->GoBack();},
            20.0F);
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

    m_TeamBuildBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_TeamBuildBanner->GetSize().y / 2.0;
    m_TeamBuildBanner->Place({0.0F, upgradeBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1 * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

    m_SubTitleBanner->ScaleSize({ORIGINAL_SCALING, ORIGINAL_SCALING});
    const auto topLeftBannerSize = m_SubTitleBanner->GetSize();
    const auto topLeftBannerX = -1 * System::GetWindowWidth() / 2.0 + topLeftBannerSize.x / 2.0;
    const auto topLeftBannerY = -6.0F;
    m_SubTitleBanner->Place({topLeftBannerX, topLeftBannerY});

    // button image
    const auto propsStoreButtonX = m_BottomBanner->GetCoordinate().x + 60.0F;
    const auto propsStoreButtonY = m_BottomBanner->GetCoordinate().y + 3.0F;
    m_b_PropsStore->Place({propsStoreButtonX, propsStoreButtonY});
    AddChild(m_b_PropsStore);

    const auto backButtonX = -1.0F * System::GetWindowWidth() / 2.0F + m_b_Back->GetSize().x / 2.0F + 5.0F;
    const auto backButtonY = -1.0F * System::GetWindowHeight() / 2.0F + m_b_Back->GetSize().y / 2.0F + 15.0F;
    m_b_Back->Place({backButtonX, backButtonY});
    AddChild(m_b_Back);

    constexpr auto catCanX = 320.0F;
    constexpr auto catCanY = -353.0F;
    m_b_CatCan->Place({catCanX, catCanY});
    AddChild(m_b_CatCan);

    // ================

    BuildSelectionBar();
}

std::shared_ptr<Phase> TeamBuild::GetDestinationPhase() {
    if (this->m_DestinationPhase.empty())
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("Upgrade", this->m_DestinationPhase);
}

void TeamBuild::GoBack() {
    this->m_DestinationPhase = "";
}

void TeamBuild::ToPropsStore() {
    this->m_DestinationPhase = "PropsStore";
}

void TeamBuild::Update() {

    Phase::Update();

    if (!m_CatSelectionBar.empty()) {
        m_ScrollManager.Update(m_CatSelectionBar);
    }

    if (m_SubTitleText) {
        std::string title = " ";
        if (!m_CatSelectionBar.empty()) {
            size_t middleIndex = m_ScrollManager.GetMiddleIndex(m_CatSelectionBar);
            if (middleIndex < m_CatSelectionBar.size()) {
                auto block = m_CatSelectionBar[middleIndex];
                switch (block->GetBlockType()) {
                case DeployType::CHARACTER:
                    title = "角色";
                    break;
                default:
                    title = " ";
                    break;
                }
            }
        }

        m_SubTitleText->SetText(title);
    }

}


void TeamBuild::BuildSelectionBar() {
    auto unlockedCats = UserManager::GetInstance().GetUnlockedCats();

    // Start Position
    glm::vec2 pos = {0.0F, -150.0F};

    for (size_t i = 0; i < unlockedCats.size(); i++) {
        // team build block --
        auto bg = std::make_shared<DeployBlock>(
            DeployType::CHARACTER,
            RESOURCE_DIR "/phase/team_build/cat_deploy_bg.png",
            pos,
            unlockedCats[i]
            );
        pos = {pos.x + bg->GetSize().x + 20.0F, pos.y};

        m_CatSelectionBar.push_back(bg);
    }

    for (auto &item: m_CatSelectionBar) AddChild(item);
}
