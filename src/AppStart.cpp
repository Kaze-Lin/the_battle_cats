#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

#include <iostream>

void App::Start() {
    LOG_TRACE("Start");

    // background image (without interaction image)
    m_BackgroundImage = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/Img009_3.png", -10.0F);
    m_Root.AddChild(m_BackgroundImage);

    m_LobbyBanner = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/lobby_banner.png", -7.0F);
    m_Root.AddChild(m_LobbyBanner);

    m_BottomBanner = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/bottom_banner.png", -7.0F);
    m_Root.AddChild(m_BottomBanner);

    m_PlayerLevel = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/player_level.png", -7.0F);
    m_Root.AddChild(m_PlayerLevel);

    m_CatBaseCatFace = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/Cat_base_cat_face.png", -9.0F);
    m_Root.AddChild(m_CatBaseCatFace);

    // button image (with interaction image)
    m_b_Start = std::make_shared<Button>(RESOURCE_DIR "/lobby/start_battle.png");
    m_b_Start->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_b_Upgrade = std::make_shared<Button>(RESOURCE_DIR "/lobby/level_up.png");
    m_b_Upgrade->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_b_TeamBuild = std::make_shared<Button>(RESOURCE_DIR "/lobby/team_buildup.png");
    m_b_TeamBuild->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_b_RestoreIcon = std::make_shared<Button>(RESOURCE_DIR "/lobby/restore_icon.png");
    m_b_RestoreIcon->ScaleSize({WOOD_BAR_BUTTON_SCALE, WOOD_BAR_BUTTON_SCALE});

    m_b_NormalGachaIcon = std::make_shared<Button>(RESOURCE_DIR "/lobby/normal_gacha_icon.png");
    m_b_NormalGachaIcon->ScaleSize({WOOD_BAR_BUTTON_SCALE, WOOD_BAR_BUTTON_SCALE});

    m_SpecialGachaIcon = std::make_shared<Button>(RESOURCE_DIR "/lobby/special_gacha_icon.png");
    m_SpecialGachaIcon->ScaleSize({WOOD_BAR_BUTTON_SCALE, WOOD_BAR_BUTTON_SCALE});

    m_PropsStore = std::make_shared<Button>(RESOURCE_DIR "/lobby/props_store_button.png");
    m_PropsStore->ScaleSize({YELLOW_BUTTON_SCALE, YELLOW_BUTTON_SCALE});

    m_Back = std::make_shared<Button>(RESOURCE_DIR "/lobby/back_button.png");
    m_Back->ScaleSize({YELLOW_BUTTON_SCALE + 0.03F, YELLOW_BUTTON_SCALE + 0.03F});


    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();

    m_LobbyBanner->AlignWithWindowWidth();
    const auto lobbyBannerY = System::GetWindowHeight() / 2.0 - m_LobbyBanner->GetSize().y / 2.0;
    m_LobbyBanner->Place({0.0F, lobbyBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1.0F * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

    m_PlayerLevel->ScaleSize({0.45F, 0.45F});
    const auto playerLevelX = -1.0F *  System::GetWindowWidth() / 2.0F + 140.0F;
    const auto playerLevelY = lobbyBannerY - m_LobbyBanner->GetSize().y / 2.0F - m_PlayerLevel->GetSize().y / 2.0F + 20.0F;
    m_PlayerLevel->Place({playerLevelX, playerLevelY});

    m_CatBaseCatFace->ScaleSize({2.0F, 2.0F});
    const auto catBaseCatFaceX = System::GetWindowWidth() / 2.0F - m_CatBaseCatFace->GetSize().x / 2.0F - 45.0F;
    const auto catBaseCatFaceY = -1.0F * System::GetWindowHeight() / 2.0F + m_CatBaseCatFace->GetSize().y / 2.0F + 100.0F;
    m_CatBaseCatFace->Place({catBaseCatFaceX, catBaseCatFaceY});

    // button image (with interaction image)
    const float startX = -1.0F * (System::GetWindowWidth() / 2.0F) + m_b_Start->GetSize().x / 2.0F + 50.0F;
    m_b_Start->Place({startX, 140.0F});
    m_Root.AddChild(m_b_Start);

    const auto startPos = m_b_Start->GetCoordinate();
    const auto startSize = m_b_Start->GetSize();
    const float upgradeY = startPos.y - startSize.y / 2.0F - 15.0F - m_b_Upgrade->GetSize().y / 2.0F;
    m_b_Upgrade->Place({startPos.x, upgradeY});
    m_Root.AddChild(m_b_Upgrade);

    const auto upgradePos = m_b_Upgrade->GetCoordinate();
    const auto upgradeSize = m_b_Upgrade->GetSize();
    const float teamBuildY = upgradePos.y - upgradeSize.y / 2.0F - 15.0F - m_b_TeamBuild->GetSize().y / 2.0F;
    m_b_TeamBuild->Place({upgradePos.x, teamBuildY});
    m_Root.AddChild(m_b_TeamBuild);

    const auto propsStroeButtonX = m_BottomBanner->GetCoordinate().x + 80.0F;
    const auto propsStroeButtonY = m_BottomBanner->GetCoordinate().y;
    m_PropsStore->Place({propsStroeButtonX, propsStroeButtonY});
    m_Root.AddChild(m_PropsStore);

    const auto backButtonX = -1.0F * System::GetWindowWidth() / 2.0F + m_Back->GetSize().x / 2.0F + 5.0F;
    const auto backButtonY = -1.0F * System::GetWindowHeight() / 2.0F + m_Back->GetSize().y / 2.0F + 15.0F;
    m_Back->Place({backButtonX, backButtonY});
    m_Root.AddChild(m_Back);

    // layout of wood area buttons
    const auto restoreIconX = -5.0F;
    const auto restoreIconY = m_BottomBanner->GetCoordinate().y + m_BottomBanner->GetSize().y / 2.0F + m_b_RestoreIcon->GetSize().y / 2.0F + 40.0F;
    m_b_RestoreIcon->Place({restoreIconX, restoreIconY});
    m_Root.AddChild(m_b_RestoreIcon);

    const auto normalGachaIconX = m_b_RestoreIcon->GetCoordinate().x + m_b_RestoreIcon->GetSize().x / 2.0F + m_b_NormalGachaIcon->GetSize().x / 2.0F + 30.0F;
    const auto normalGachaIconY = restoreIconY;
    m_b_NormalGachaIcon->Place({normalGachaIconX, normalGachaIconY});
    m_Root.AddChild(m_b_NormalGachaIcon);

    const auto specialGachaIconX = m_b_NormalGachaIcon->GetCoordinate().x + m_b_NormalGachaIcon->GetSize().x / 2.0F + m_SpecialGachaIcon->GetSize().x / 2.0F + 20.0F;
    const auto specialGachaIconY = restoreIconY;
    m_SpecialGachaIcon->Place({specialGachaIconX, specialGachaIconY});
    m_Root.AddChild(m_SpecialGachaIcon);



    m_CurrentState = State::UPDATE;
}


