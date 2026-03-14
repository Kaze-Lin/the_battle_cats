#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

#include <iostream>

void App::Start() {
    LOG_TRACE("Start");

    m_BackgroundImage = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/Img009_3.webp");
    m_BackgroundImage->AlignWindow();
    m_Root.AddChild(m_BackgroundImage);

    m_b_Start = std::make_shared<Button>(RESOURCE_DIR "/lobby/start_battle.png");
    m_b_Start->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_b_Upgrade = std::make_shared<Button>(RESOURCE_DIR "/lobby/level_up.png");
    m_b_Upgrade->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_b_TeamBuild = std::make_shared<Button>(RESOURCE_DIR "/lobby/team_buildup.png");
    m_b_TeamBuild->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    // --- Layout settings ---
    const float startX = -1.0F * (System::GetWindowWidth() / 2.0F) + m_b_Start->GetSize().x / 2.0F + 70.0F;
    m_b_Start->Place({startX, 160.0F});
    m_Root.AddChild(m_b_Start);

    const auto startPos = m_b_Start->GetCoordinate();
    const auto startSize = m_b_Start->GetSize();
    const float upgradeY = startPos.y - startSize.y / 2.0F - 20.0F - m_b_Upgrade->GetSize().y / 2.0F;
    m_b_Upgrade->Place({startPos.x, upgradeY});
    m_Root.AddChild(m_b_Upgrade);

    const auto upgradePos = m_b_Upgrade->GetCoordinate();
    const auto upgradeSize = m_b_Upgrade->GetSize();
    const float teamBuildY = upgradePos.y - upgradeSize.y / 2.0F - 20.0F - m_b_TeamBuild->GetSize().y / 2.0F;
    m_b_TeamBuild->Place({upgradePos.x, teamBuildY});
    m_Root.AddChild(m_b_TeamBuild);

    m_CurrentState = State::UPDATE;
}


