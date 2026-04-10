#include "Phase/Fight.hpp"

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
            nullptr);
    m_b_Pause->ScaleSize({ORIGINAL_SCALING + 0.1F, ORIGINAL_SCALING + 0.1F});

    m_b_RickUpgrade =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Richupgrade_1.png",
            nullptr,
            -6.0F);
    m_b_RickUpgrade->ScaleSize({ORIGINAL_SCALING + 0.5F, ORIGINAL_SCALING + 0.5F});

    m_b_CannonCharge =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Cannoncharge_0.png",
            nullptr,
            -6.0F);
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
}