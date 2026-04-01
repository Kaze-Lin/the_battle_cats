#include "Phase/PropsStore.hpp"

#include "PhaseManager.hpp"
#include "Phase/Upgrade.hpp"

PropsStore::PropsStore(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/upgrade/background_image.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_PropsStoreBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/props_store/props_Banner.png",
            -7.0F);
    AddChild(m_PropsStoreBanner);

    m_BottomBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/bottom_banner.png",
            -7.0F);
    AddChild(m_BottomBanner);

    // button image (with interaction image)
    m_b_Back =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/back_button.png",
            [this](){this->GoBack();});
    m_b_Back->ScaleSize({YELLOW_BUTTON_SCALE + 0.03F, YELLOW_BUTTON_SCALE + 0.03F});

    m_b_CatCan =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/cat_can_button.png",
            nullptr);
    m_b_CatCan->ScaleSize({ORIGINAL_SCALING + 0.15F, ORIGINAL_SCALING + 0.15F});


    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    m_PropsStoreBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_PropsStoreBanner->GetSize().y / 2.0;
    m_PropsStoreBanner->Place({0.0F, upgradeBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1 * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

    const auto backButtonX = -1.0F * System::GetWindowWidth() / 2.0F + m_b_Back->GetSize().x / 2.0F + 5.0F;
    const auto backButtonY = -1.0F * System::GetWindowHeight() / 2.0F + m_b_Back->GetSize().y / 2.0F + 15.0F;
    m_b_Back->Place({backButtonX, backButtonY});
    AddChild(m_b_Back);

    const auto catcanX = 320.0F;
    const auto catcanY = -353.0F;
    m_b_CatCan->Place({catcanX, catcanY});
    AddChild(m_b_CatCan);

}

std::shared_ptr<Phase> PropsStore::GetDestinationPhase() {
    if (this->m_DestinationPhase == "")
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("PropsStore", this->m_DestinationPhase);
}

void PropsStore::GoBack() {
    this->m_DestinationPhase = "";
}

