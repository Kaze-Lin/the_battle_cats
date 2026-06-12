#include "Component/Block/DeployBlock.hpp"

#include "DataBase//UnitData.hpp"
#include "DataBaseManager.hpp"
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

    std::string GetIconPath(const CatSaveData &data) {
        std::string path = "/cat_materials/normal/";

        std::string name = DatabaseManager::GetInstance().GetCatData(data.catId)->nameInternal;

        path = path + to_lower(name) + "/cat_block_image/" + std::to_string(data.currentForm) + ".png";

        return path;
    }
}

DeployBlock::DeployBlock(
    DeployType deployType,
    const std::string& bgImagePath,
    glm::vec2 position,
    const CatSaveData& catSaveData,
    float zIndex
    )
    : OptionBlock(bgImagePath, zIndex),
    m_DeployType(deployType){
    m_OriginalSize = m_Drawable->GetSize();

    auto id = catSaveData.catId;

    const UnitData* catData = DatabaseManager::GetInstance().GetCatData(id);

    if (catData == nullptr) {
        LOG_ERROR("cannot find cat with ID " + std::to_string(id));
        return;
    }

    std::string deploySort = std::to_string(id) + ": " + catData->nameInternal;
    LOG_DEBUG(deploySort);

    // layout
    // this->ScaleSize({0.39F, 0.39F});
    this->Place(position);

    // cat block image
    m_CatBlockImage = std::make_shared<BackgroundImage>(
        RESOURCE_DIR + GetIconPath(catSaveData),
        11.0F
        );
    glm::vec2 catBlockImageT = {0.0F, 40.0F};
    m_CatBlockImage->Place(GetCoordinate() + catBlockImageT);
    m_CatBlockImage->ScaleSize({1.14F, 1.14F});
    AddChild(m_CatBlockImage);

    // max level
    // SetImage(RESOURCE_DIR "/phase/upgrade/cat_background.png");

    m_Max = std::make_shared<Text>(28, "MAX", 15.0F);

    m_Max->SetColor(Util::Color::FromName(Util::Colors::GREEN));
    m_Max->Place({GetCoordinate().x + 52.0F, GetCoordinate().y + 15.0F});

    AddChild(m_Max);

    m_Max->SetVisible(false);
    if (catSaveData.level == 10) {
        m_Max->SetVisible(true);
    }

    // cat name
    std::string name = catData->forms[catSaveData.currentForm - 1].name;
    std::replace(name.begin(), name.end(), '_', ' ');
    m_CatName = std::make_shared<Text>(24, name, 15.0F);

    glm::vec2 catNameOffset = {0.0F, 125.0F};
    m_CatName->Place({GetCoordinate() + catNameOffset});
    m_CatName->SetColor(Util::Color::FromName(Util::Colors::WHITE));

    AddChild(m_CatName);

    // cat level
    std::string level = std::to_string(catSaveData.level);
    m_CatLevel = std::make_shared<Text>(32, level, 15.0F);

    glm::vec2 catLevelOffset = {135.0F, -8.0F};
    m_CatLevel->Place(GetCoordinate() + catLevelOffset);
    m_CatLevel->SetColor(Util::Color::FromName(Util::Colors::YELLOW));

    AddChild(m_CatLevel);

    // cat deploy cost
    int deployCost = 0;
    if (catSaveData.currentForm > 0 && catSaveData.currentForm <= 3) {
        deployCost = catData->forms[catSaveData.currentForm - 1].cost;
    }
    std::string deploy_cost = std::to_string(deployCost);
    m_DeployCost = std::make_shared<Text>(24, deploy_cost, 15.0F);

    glm::vec2 deployCostOffset = {50.0F, -113.0F};
    m_DeployCost->Place(GetCoordinate() + deployCostOffset);
    m_DeployCost->SetColor(Util::Color::FromName(Util::Colors::WHITE));

    AddChild(m_DeployCost);

}

DeployType DeployBlock::GetBlockType() const {
    return this->m_DeployType;
}