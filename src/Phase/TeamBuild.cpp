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
        const auto catData = DatabaseManager::GetInstance().GetCatData(data.catId);
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

    constexpr glm::vec2 block_size = {148.0F, 114.0F};
    constexpr glm::vec2 horizontal_offset = {196.0F, 0.0F};
    constexpr glm::vec2 vertical_offset = {0.0F, 127.0F};
    constexpr glm::vec2 pos = {-331.0F, 219.0F};
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
    m_SubTitleText = std::make_shared<TwoLayerText>(
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

    m_ResourceDisplay = std::make_shared<ResourceDisplay>();
    AddChild(m_ResourceDisplay);

    CalculateBlocksRange();

    auto currentUser = UserManager::GetInstance().GetCurrentUser();
    if (currentUser) {
        m_CurrentTeam = currentUser->progress.teamBuild;
    } else {
        m_CurrentTeam.fill(-1);
    }

    for (int i = 0; i < 10; ++i) {
        // Only load a completely transparent placeholder if you don't want a solid BG
        m_TeamImages[i] = std::make_shared<BackgroundImage>(RESOURCE_DIR "/phase/team_build/cat_deploy_bg.png", 5.0F);

        glm::vec2 pos = (m_BlocksRange[i][0] + m_BlocksRange[i][1]) / 2.0F;
        m_TeamImages[i]->Place(pos);
        glm::vec2 temp = {110.0F, 85.0F};
        m_TeamImages[i]->ScaleSize(block_size / temp);
        AddChild(m_TeamImages[i]);
    }
    RefreshTeamImages();
}

void TeamBuild::CalculateBlocksRange() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            glm::vec2 offset = (-1.0F * static_cast<float>(i) * vertical_offset) + (horizontal_offset * static_cast<float>(j));
            m_BlocksRange[(i * 5) + j][0] = pos - (block_size / 2.0F) + offset;
            m_BlocksRange[(i * 5) + j][1] = pos + (block_size / 2.0F) + offset;
        }
    }
}

void TeamBuild::RefreshTeamImages() {
    auto currentUser = UserManager::GetInstance().GetCurrentUser();
    for (int i = 0; i < 10; ++i) {
        if (m_CurrentTeam[i] != -1) {
            CatSaveData catData{m_CurrentTeam[i], 1, 1};
            if (currentUser) {
                for (const auto& cat : currentUser->unlockedCats) {
                    if (cat.catId == m_CurrentTeam[i]) {
                        catData = cat;
                        break;
                    }
                }
            }
            m_TeamImages[i]->SetImage(RESOURCE_DIR + GetDragIconPath(catData));
            m_TeamImages[i]->SetVisible(true);
        } else {
            m_TeamImages[i]->SetVisible(false);
        }
    }
}

int TeamBuild::FindNearestEmptySlot(int target_index) {
    int target_row = target_index / 5;
    int target_col = target_index % 5;
    
    int nearest_index = -1;
    int min_dist = 9999;

    for (int i = 0; i < 10; ++i) {
        if (m_CurrentTeam[i] == -1) {
            int row = i / 5;
            int col = i % 5;
            int dist = std::abs(target_row - row) + std::abs(target_col - col);
            if (dist < min_dist) {
                min_dist = dist;
                nearest_index = i;
            }
        }
    }
    return nearest_index;
}

std::shared_ptr<Phase> TeamBuild::GetDestinationPhase() {
    if (this->m_DestinationPhase == "Non") {
        return nullptr;
    }

    auto currentUser = UserManager::GetInstance().GetCurrentUser();
    if (currentUser) {
        currentUser->progress.teamBuild = m_CurrentTeam;
        UserManager::GetInstance().SaveDatabase();
    }

    if (this->m_DestinationPhase.empty())
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("TeamBuild", this->m_DestinationPhase);
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

            auto it = std::find_if(
                m_BlocksRange.begin(),
                m_BlocksRange.end(),
                [&](const auto& block) {
                    return IsInBlock(
                        block[0],
                        block[1],
                        mousePos);
                });

            // is in range
            if (it != m_BlocksRange.end()) {
                int target_index = std::distance(m_BlocksRange.begin(), it);
                
                if (m_PressedBlock != nullptr) {
                    // Dragged from bottom selection bar
                    bool is_full = true;
                    for (int i = 0; i < 10; ++i) {
                        if (m_CurrentTeam[i] == -1) { is_full = false; break; }
                    }
                    
                    if (!is_full) {
                        int cat_id = m_PressedBlock->GetCatSaveData().catId;
                        if (m_CurrentTeam[target_index] != -1) {
                            int empty_slot = FindNearestEmptySlot(target_index);
                            if (empty_slot != -1) {
                                m_CurrentTeam[empty_slot] = m_CurrentTeam[target_index];
                            }
                        }
                        m_CurrentTeam[target_index] = cat_id;
                    }
                } else if (m_PressedGridIndex != -1) {
                    // Dragged from top grid
                    int cat_id = m_CurrentTeam[m_PressedGridIndex];
                    m_CurrentTeam[m_PressedGridIndex] = -1; // Temporarily vacate the origin slot
                    
                    if (m_CurrentTeam[target_index] != -1) {
                        int empty_slot = FindNearestEmptySlot(target_index);
                        if (empty_slot != -1) {
                            m_CurrentTeam[empty_slot] = m_CurrentTeam[target_index];
                        }
                    }
                    m_CurrentTeam[target_index] = cat_id;
                }
            } else {
                // Out of range drop
                if (m_PressedGridIndex != -1 && mousePos.y < 0.0F) {
                    // Dragged from top grid and dropped in bottom area -> remove from team
                    m_CurrentTeam[m_PressedGridIndex] = -1;
                }
            }

            RemoveChild(m_DragGhost);
            m_DragGhost = nullptr;
            RefreshTeamImages();

        } else if (m_PressedBlock) {
            // 原地單擊邏輯
            LOG_DEBUG("單擊了貓咪方塊");
        }

        // 狀態重置
        m_PressedBlock = nullptr;
        m_PressedGridIndex = -1;
        m_IsDraggingBlock = false;
    }

    // 2. 按下左鍵：記錄潛在的拖曳目標與起始點
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        bool clicked_top_grid = false;
        for (int i = 0; i < 10; ++i) {
            if (m_CurrentTeam[i] != -1 && IsInBlock(m_BlocksRange[i][0], m_BlocksRange[i][1], mousePos)) {
                m_PressedGridIndex = i;
                m_PressStartPos = mousePos;
                m_PressStartTime = Util::Time::GetElapsedTimeMs();
                clicked_top_grid = true;
                break;
            }
        }
        
        if (!clicked_top_grid) {
            bool is_full = true;
            for (int i = 0; i < 10; ++i) {
                if (m_CurrentTeam[i] == -1) { is_full = false; break; }
            }
            if (!is_full) {
                for (const auto& block : m_CatSelectionBar) {
                    if (IsBlockHovered(block)) {
                        int cat_id = block->GetCatSaveData().catId;
                        bool already_in_team = false;
                        for (int i = 0; i < 10; ++i) {
                            if (m_CurrentTeam[i] == cat_id) {
                                already_in_team = true;
                                break;
                            }
                        }
                        
                        if (!already_in_team) {
                            m_PressedBlock = block;
                            m_PressStartPos = mousePos;
                            m_PressStartTime = Util::Time::GetElapsedTimeMs();
                        }
                        break;
                    }
                }
            }
        }
    }

    // 3. 壓住滑鼠並移動：判定意圖 (拖曳 vs 左右滑動)
    bool has_pressed_item = (m_PressedBlock != nullptr) || (m_PressedGridIndex != -1);
    if (has_pressed_item && !m_IsDraggingBlock && Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        glm::vec2 delta = mousePos - m_PressStartPos;
        constexpr float threshold = 15.0F; // 容錯閾值，超過 15 像素才算有滑動
        auto elapsedTime = Util::Time::GetElapsedTimeMs() - m_PressStartTime;

        if (m_PressedBlock != nullptr && std::abs(delta.x) > threshold && elapsedTime < 200) {
            // 【左右拉】判定為意圖滑動選單
            m_PressedBlock = nullptr; // 清除點擊目標，放棄拖曳貓咪的意圖
        } else if (elapsedTime >= 200 || std::abs(delta.y) > threshold || std::abs(delta.x) > threshold) {
            m_IsDraggingBlock = true;

            std::string icon_path = "";
            if (m_PressedBlock != nullptr) {
                icon_path = GetDragIconPath(m_PressedBlock->GetCatSaveData());
            } else if (m_PressedGridIndex != -1) {
                CatSaveData catData{m_CurrentTeam[m_PressedGridIndex], 1, 1};
                auto currentUser = UserManager::GetInstance().GetCurrentUser();
                if (currentUser) {
                    for (const auto& cat : currentUser->unlockedCats) {
                        if (cat.catId == m_CurrentTeam[m_PressedGridIndex]) {
                            catData = cat;
                            break;
                        }
                    }
                }
                icon_path = GetDragIconPath(catData);
                m_TeamImages[m_PressedGridIndex]->SetVisible(false); // Hide original
            }
            
            m_DragGhost = std::make_shared<BackgroundImage>(
                RESOURCE_DIR + icon_path,
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

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        auto currentUser = UserManager::GetInstance().GetCurrentUser();
        if (currentUser) {
            currentUser->progress.teamBuild = m_CurrentTeam;
        }
    }
}


void TeamBuild::BuildSelectionBar() {
    auto unlockedCats = UserManager::GetInstance().GetUnlockedCats();

    // Start Position
    glm::vec2 pos = {0.0F, -150.0F};

    for (auto& catInfo: unlockedCats) {
        // team build block --
        if (DatabaseManager::GetInstance().GetCatData(catInfo.catId) != nullptr) {
            auto bg = std::make_shared<DeployBlock>(
                DeployType::CHARACTER,
                RESOURCE_DIR "/phase/team_build/cat_deploy_bg.png",
                pos,
                catInfo
                );
            pos = {pos.x + bg->GetSize().x + 20.0F, pos.y};

            m_CatSelectionBar.push_back(bg);
        }
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
