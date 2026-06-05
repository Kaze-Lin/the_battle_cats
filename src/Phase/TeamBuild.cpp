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

    std::string GetIconPath(CatSaveData &data) {
        std::string path = "/cat_materials/normal/";

        std::string name = DatabaseManager::GetInstance().GetCatData(data.catId)->nameInternal;

        path = path + to_lower(name) + "/cat_block_image/" + std::to_string(data.currentForm) + ".png";

        return path;
    }
}

TeamBuild::TeamBuild(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/team_build/team_build_bg.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_UpgradeBanner =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/upgrade/upgrade_Banner.png",
            -7.0F);
    AddChild(m_UpgradeBanner);

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
        -5.0F,
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

    m_UpgradeBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_UpgradeBanner->GetSize().y / 2.0;
    m_UpgradeBanner->Place({0.0F, upgradeBannerY});

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
                case UpgradeType::CHARACTER:
                    title = "角色";
                    break;
                case UpgradeType::CANNON:
                    title = "貓咪砲";
                    break;
                case UpgradeType::WORKER_CAT:
                    title = "工作狂貓";
                    break;
                case UpgradeType::CASTLE:
                    title = "城堡";
                    break;
                case UpgradeType::SPECIAL_ABILITIES:
                    title = "特殊能力";
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

    // Middle Position
    glm::vec2 selectedPos = {0.0F, -150.0F};

    for (size_t i = 0; i < unlockedCats.size(); i++) {
        int catId = unlockedCats[i].catId;

        const UnitData* catData = DatabaseManager::GetInstance().GetCatData(catId);

        if (catData == nullptr) {
            LOG_WARN("cannot find cat with ID " + std::to_string(catId));
            continue;
        }

        std::string deploySort = std::to_string(catId) + ": " + catData->nameInternal;
        LOG_DEBUG(deploySort);

        // Upgrade block --
        auto bg = std::make_shared<UpgradeBlock>(
            UpgradeType::CHARACTER,
            RESOURCE_DIR "/phase/upgrade/cat_background_xp.png"
            );
        bg->ID = catId;

        // layout
        bg->ScaleSize({0.39F, 0.39F});
        bg->Place({selectedPos.x + (bg->GetSize().x + 20.0F) * i, selectedPos.y});

        // cat block image
        bg->m_CatBlockImage = std::make_shared<BackgroundImage>(
            RESOURCE_DIR + GetIconPath(unlockedCats[i]),
            11.0F
            );
        glm::vec2 catBlockImageT = {0.0F, 40.0F};
        bg->m_CatBlockImage->Place(bg->GetCoordinate() + catBlockImageT);
        bg->m_CatBlockImage->ScaleSize({1.14F, 1.14F});
        bg->AddChild(bg->m_CatBlockImage);

        // max level
        bg->SetImage(RESOURCE_DIR "/phase/upgrade/cat_background.png");

        bg->m_Max = std::make_shared<Text>(28, "MAX", 15.0F);

        bg->m_Max->SetColor(Util::Color::FromName(Util::Colors::GREEN));
        bg->m_Max->Place({bg->GetCoordinate().x + 52.0F, bg->GetCoordinate().y + 15.0F});

        bg->AddChild(bg->m_Max);

        bg->m_Max->SetVisible(false);
        if (unlockedCats[i].level == 10) {
            bg->m_Max->SetVisible(true);
        }

        // cat name
        std::string name = catData->forms[unlockedCats[i].currentForm - 1].name;
        std::replace(name.begin(), name.end(), '_', ' ');
        bg->m_CatName = std::make_shared<Text>(24, name, 15.0F);

        glm::vec2 catNameOffset = {0.0F, 125.0F};
        bg->m_CatName->Place({bg->GetCoordinate() + catNameOffset});
        bg->m_CatName->SetColor(Util::Color::FromName(Util::Colors::WHITE));

        bg->AddChild(bg->m_CatName);

        // cat level
        std::string level = std::to_string(unlockedCats[i].level);
        bg->m_CatLevel = std::make_shared<Text>(32, level, 15.0F);

        glm::vec2 catLevelPos = {135.0F, -8.0F};
        bg->m_CatLevel->Place(bg->GetCoordinate() + catLevelPos);
        bg->m_CatLevel->SetColor(Util::Color::FromName(Util::Colors::YELLOW));

        bg->AddChild(bg->m_CatLevel);

        m_CatSelectionBar.push_back(bg);
    }

    for (auto &item: m_CatSelectionBar) AddChild(item);
}
