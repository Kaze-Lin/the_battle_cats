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

    m_b_test =
        std::make_shared<Button>(
            RESOURCE_DIR "/phase/fight/Richupgrade_Max.png",
            nullptr,
            -6.0F);
    m_b_test->ScaleSize({ORIGINAL_SCALING + 0.1F, ORIGINAL_SCALING + 0.1F});

    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    // button image (with interaction image)
    const auto pauseX = -1 * System::GetWindowWidth() / 2.0F + m_b_Pause->GetSize().x / 2.0F + 5.0F;
    const auto pauseY = System::GetWindowHeight() / 2.0F - m_b_Pause->GetSize().y / 2.0F - 5.0F;
    m_b_Pause->Place({pauseX, pauseY});
    AddChild(m_b_Pause);

    const auto testX = -1 * System::GetWindowWidth() / 2.0F + m_b_Pause->GetSize().x / 2.0F + 5.0F;
    const auto testY = System::GetWindowHeight() / 2.0F - m_b_Pause->GetSize().y / 2.0F - 5.0F;
    m_b_test->Place({testX, testY});
    AddChild(m_b_test);
}