#include "Phase/TeamBuild.hpp"

#include "Phase/Upgrade.hpp"

#include "PhaseManager.hpp"
#include "DatabaseManager.hpp"
#include "UserManager.hpp"

namespace {
    std::string to_lower(std::string s) {
        std::transform(
            s.begin(),
            s.end(),
            s.begin(),
            [](unsigned char c) {
                return std::tolower(c);
            }
        );
        return s;
    }

    std::string GetDragIconPath(const CatSaveData& data) {
        std::string path = "/cat_materials/normal/";
        const auto* catData = DatabaseManager::GetInstance().GetCatData(data.catId);
        if (catData != nullptr) {
            std::string name = catData->nameInternal;
            path = path + to_lower(name) + "/generate_cat_button/" + std::to_string(data.currentForm) + ".png";
        }
        return path;
    }

    bool IsInBlock(
        const glm::vec2& left_bottom_pos,
        const glm::vec2& right_top_pos,
        const glm::vec2& pos) {
        return glm::all(glm::greaterThanEqual(pos, left_bottom_pos)) &&
               glm::all(glm::lessThanEqual(pos, right_top_pos));
    }
}

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
    const auto mousePos = Util::Input::GetCursorPosition();

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        if (m_IsDraggingBlock && m_DragGhost) {

            constexpr glm::vec2 block_size = {148.0F, 114.0F};
            constexpr glm::vec2 horizontal_offset = {196.0F, 0.0F};
            constexpr glm::vec2 vertical_offset = {0.0F, 127.0F};
            constexpr glm::vec2 pos = {-331.0F, 219.0F};

            // every block has 2 range elements, first one is left bottom, second one is right top
            std::array<std::array<glm::vec2, 2>, 10> blocks_range;

            // set the range for each block
            for (float i = 0; i < 2; i++) {
                for (float j = 0; j < 5; j++) {
                    glm::vec2 offset = (-1.0F * i * vertical_offset) + (horizontal_offset * j);
                    blocks_range[(i * 5) + j][0] = pos - (block_size / 2.0F) + offset;
                    blocks_range[(i * 5) + j][1] = pos + (block_size / 2.0F) + offset;
                }
            }

            auto it = std::find_if(
                blocks_range.begin(),
                blocks_range.end(),
                [&](const auto& block) {
                    return IsInBlock(
                        block[0],
                        block[1],
                        mousePos);
                });

            // is in range
            if (it != blocks_range.end()) {
                int index = std::distance(blocks_range.begin(), it);
                //　
            } else {

            }

            RemoveChild(m_DragGhost);
            m_DragGhost = nullptr;
            
        } else if (m_PressedBlock) {
            // 原地單擊邏輯
            LOG_DEBUG("單擊了貓咪方塊");
        }
        
        // 狀態重置
        m_PressedBlock = nullptr;
        m_IsDraggingBlock = false;
    }

    // 2. 按下左鍵：記錄潛在的拖曳目標與起始點
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        for (const auto& block : m_CatSelectionBar) {
            if (IsBlockHovered(block)) {
                m_PressedBlock = block;
                m_PressStartPos = mousePos;
                m_PressStartTime = Util::Time::GetElapsedTimeMs();
                break;
            }
        }
    }

    // 3. 壓住滑鼠並移動：判定意圖 (拖曳 vs 左右滑動)
    if (m_PressedBlock && !m_IsDraggingBlock && Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        glm::vec2 delta = mousePos - m_PressStartPos;
        constexpr float threshold = 15.0F; // 容錯閾值，超過 15 像素才算有滑動
        auto elapsedTime = Util::Time::GetElapsedTimeMs() - m_PressStartTime;

        if (std::abs(delta.x) > threshold && elapsedTime < 200) {
            // 【左右拉】判定為意圖滑動選單
            m_PressedBlock = nullptr; // 清除點擊目標，放棄拖曳貓咪的意圖
        } else if (elapsedTime >= 200 || delta.y > threshold) {
            // 【向上拉或長按】判定為意圖拖曳貓咪
            m_IsDraggingBlock = true;
            
            m_DragGhost = std::make_shared<BackgroundImage>(
                RESOURCE_DIR + GetDragIconPath(m_PressedBlock->GetCatSaveData()),
                50.0F // 給一個很高的 zIndex 確保蓋在所有畫面最上層
            );
            m_DragGhost->ScaleSize({1.14F, 1.14F}); // 保持跟原本大小一樣
            AddChild(m_DragGhost);
        }
    }

    // 4. 更新拖曳虛影的座標 (保持在游標位置)
    if (m_IsDraggingBlock && m_DragGhost) {
        m_DragGhost->Place(mousePos);
    }

    // 5. ScrollManager 邏輯
    if (!m_IsDraggingBlock && !m_CatSelectionBar.empty()) {
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

bool TeamBuild::IsBlockHovered(const std::shared_ptr<DeployBlock>& block) {
    if (!block) return false;

    const auto mousePos = Util::Input::GetCursorPosition();
    const auto size = block->GetSize();
    const auto pos = block->GetCoordinate();
    const auto halfWidth = size.x / 2.0F;
    const auto halfHeight = size.y / 2.0F;

    return (mousePos.x >= pos.x - halfWidth && mousePos.x <= pos.x + halfWidth &&
            mousePos.y >= pos.y - halfHeight && mousePos.y <= pos.y + halfHeight);
}
