#include "Phase/Upgrade.hpp"

#include "PhaseManager.hpp"
#include "DatabaseManager.hpp"
#include "Component/Text.hpp"

namespace Chooser {
    size_t GetMiddleIndex(const std::vector<std::shared_ptr<OptionBlock>> &v)
    {
        for (size_t i = 0; i < v.size(); i++) {
            if (std::abs(v[i]->GetCoordinate().x) < 0.01F) {
                return i;
            }
        }
        return v.size();
    }

    void HorizontalMovement(const std::vector<std::shared_ptr<OptionBlock>> &v, float offset) {
        for (auto &item: v) {
            item->Place({item->GetCoordinate().x - offset, item->GetCoordinate().y});
        }
    }

    std::string GetBlockTitle(const std::vector<std::shared_ptr<OptionBlock>> &v) {
        auto block = v[GetMiddleIndex(v)];

        std::string title = "";

        switch (block->GetBlockType()) {
        case BlockType::CHARACTER:
            title = "角色";
            break;
        case BlockType::CANNON:
            title = "貓咪砲";
            break;
        case BlockType::WORKER_CAT:
            title = "工作狂貓";
            break;
        case BlockType::CASTLE:
            title = "城堡";
            break;
        case BlockType::SPECIAL_ABILITIES:
            title = "特殊能力";
            break;
        default:
            break;
        }

        return title;
    }
}

Upgrade::Upgrade(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/phase/upgrade/background_image.png",
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
            -7.0F);
    m_SubTitleBanner->AddChild(
        std::make_shared<Text>(
            30,
            "角色",
            -5.0F,
            Util::Color::FromName(Util::Colors::WHITE)
            )
        );
    AddChild(m_SubTitleBanner);

    // button image (with interaction image)
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

    m_UpgradeBanner->AlignWithWindowWidth();
    const auto upgradeBannerY = System::GetWindowHeight() / 2.0 - m_UpgradeBanner->GetSize().y / 2.0;
    m_UpgradeBanner->Place({0.0F, upgradeBannerY});

    m_BottomBanner->AlignWithWindowWidth();
    const auto bottomBannerY = -1 * System::GetWindowHeight() / 2.0 + m_BottomBanner->GetSize().y / 2.0;
    m_BottomBanner->Place({0.0F, bottomBannerY});

    m_SubTitleBanner->ScaleSize({ORIGINAL_SCALING, ORIGINAL_SCALING});
    const auto topLeftBannerSize = m_SubTitleBanner->GetSize();
    const auto topLeftBannerX = -1 * System::GetWindowWidth() / 2.0 + topLeftBannerSize.x / 2.0;
    const auto topLeftBannerY = System::GetWindowHeight() / 2.0 - m_UpgradeBanner->GetSize().y - topLeftBannerSize.y / 2.0 - 10.0F;
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

std::shared_ptr<Phase> Upgrade::GetDestinationPhase() {
    if (this->m_DestinationPhase.empty())
        return PhaseManager::GetNextPhase();
    return PhaseManager::GetNextPhase("Upgrade", this->m_DestinationPhase);
}

void Upgrade::GoBack() {
    this->m_DestinationPhase = "";
}

void Upgrade::ToPropsStore() {
    this->m_DestinationPhase = "PropsStore";
}

void Upgrade::Update() {

    Phase::Update();

    // LOG_DEBUG("middle index is = " + std::to_string(GetMiddleIndex(m_UpgradeSelectionBar)));
    // LOG_DEBUG("vector size is = " + std::to_string(m_UpgradeSelectionBar.size()));

    if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
        size_t midIndex = Chooser::GetMiddleIndex(m_UpgradeSelectionBar);

        if (midIndex + 1 < m_UpgradeSelectionBar.size()) {
            size_t rightIndex = midIndex + 1;
            Chooser::HorizontalMovement(m_UpgradeSelectionBar, m_UpgradeSelectionBar[rightIndex]->GetCoordinate().x);
        }
    } else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
        size_t midIndex = Chooser::GetMiddleIndex(m_UpgradeSelectionBar);

        if (midIndex > 0) {
            size_t leftIndex = midIndex - 1;
            Chooser::HorizontalMovement(m_UpgradeSelectionBar, m_UpgradeSelectionBar[leftIndex]->GetCoordinate().x);
        }
    }

    if (auto text = std::dynamic_pointer_cast<Text>(m_SubTitleBanner->GetChildren()[0])) {
        std::string title = Chooser::GetBlockTitle(m_UpgradeSelectionBar);
        text->SetText(title);
    }

}


void Upgrade::BuildSelectionBar() {
    UserProfile* user = UserManager::GetInstance().GetCurrentUser();

    // Middle Position
    glm::vec2 selectedPos = {0.0F, 105.0F};

    for (size_t i = 0; i < user->unlockedCats.size(); i++) {
        int catId = user->unlockedCats[i].catId;

        const UnitData* catData = DatabaseManager::GetInstance().GetCatData(catId);

        if (catData == nullptr) {
            LOG_WARN("cannot find cat with ID " + std::to_string(catId));
            continue;
        }

        std::string deploySort = std::to_string(catId) + ": " + catData->nameInternal;
        LOG_DEBUG(deploySort);

        auto bg = std::make_shared<OptionBlock>(
            BlockType::CHARACTER,
            RESOURCE_DIR "/phase/upgrade/cat_background_xp.png"
            );

        // layout
        bg->ScaleSize({0.39F, 0.39F});
        bg->Place({selectedPos.x + (bg->GetSize().x + 20.0F) * i, selectedPos.y});

        // max level
        // it should be "if (user->unlockedCats[i].level == catData->maxLevel)"
        if (user->unlockedCats[i].level == 10) {
            bg->SetImage(RESOURCE_DIR "/phase/upgrade/cat_background.png");

            auto text = std::make_shared<Text>(28, "MAX", 15.0F);

            text->SetColor(Util::Color::FromName(Util::Colors::GREEN));
            text->Place({bg->GetCoordinate().x + 52.0F, bg->GetCoordinate().y + 15.0F});

            bg->AddChild(text);
        }

        auto catName = std::make_shared<Text>(24, catData->nameInternal, 15.0F);

        catName->Place({bg->GetCoordinate().x, bg->GetCoordinate().y + 128.0F});
        catName->SetColor(Util::Color::FromName(Util::Colors::WHITE));

        bg->AddChild(catName);

        m_UpgradeSelectionBar.push_back(bg);
    }

    for (auto &item: m_UpgradeSelectionBar) AddChild(item);
}
