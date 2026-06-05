// *** don't touch below ***
//
// #include "Component/Block/DeployBlock.hpp"
//
// #include "DataBase//UnitData.hpp"
// #include "DataBaseManager.hpp"
//
// DeployBlock::DeployBlock(
//     DeployType deployType,
//     const std::string& bgImagePath,
//     float zIndex,
//
//     int id
//     )
//     : OptionBlock(bgImagePath, zIndex),
//         m_ID(id),
//         m_DeployType(deployType){
//     m_OriginalSize = m_Drawable->GetSize();
//
//     const UnitData* catData = DatabaseManager::GetInstance().GetCatData(m_ID);
//
//     if (catData == nullptr) {
//         LOG_WARN("cannot find cat with ID " + std::to_string(m_ID));
//     }
//
//     std::string deploySort = std::to_string(m_ID) + ": " + catData->nameInternal;
//     LOG_DEBUG(deploySort);
//
//     // Upgrade block --
//     auto bg = std::make_shared<UpgradeBlock>(
//         UpgradeType::CHARACTER,
//         RESOURCE_DIR "/phase/upgrade/cat_background_xp.png"
//         );
//     bg->ID = m_ID;
//
//     // layout
//     bg->ScaleSize({0.39F, 0.39F});
//     bg->Place({selectedPos.x + (bg->GetSize().x + 20.0F) * i, selectedPos.y});
//
//     // cat block image
//     bg->m_CatBlockImage = std::make_shared<BackgroundImage>(
//         RESOURCE_DIR + GetIconPath(unlockedCats[i]),
//         11.0F
//         );
//     glm::vec2 catBlockImageT = {0.0F, 40.0F};
//     bg->m_CatBlockImage->Place(bg->GetCoordinate() + catBlockImageT);
//     bg->m_CatBlockImage->ScaleSize({1.14F, 1.14F});
//     bg->AddChild(bg->m_CatBlockImage);
//
//     // max level
//     bg->SetImage(RESOURCE_DIR "/phase/upgrade/cat_background.png");
//
//     bg->m_Max = std::make_shared<Text>(28, "MAX", 15.0F);
//
//     bg->m_Max->SetColor(Util::Color::FromName(Util::Colors::GREEN));
//     bg->m_Max->Place({bg->GetCoordinate().x + 52.0F, bg->GetCoordinate().y + 15.0F});
//
//     bg->AddChild(bg->m_Max);
//
//     bg->m_Max->SetVisible(false);
//     if (unlockedCats[i].level == 10) {
//         bg->m_Max->SetVisible(true);
//     }
//
//     // cat name
//     std::string name = catData->forms[unlockedCats[i].currentForm - 1].name;
//     std::replace(name.begin(), name.end(), '_', ' ');
//     bg->m_CatName = std::make_shared<Text>(24, name, 15.0F);
//
//     glm::vec2 catNameOffset = {0.0F, 125.0F};
//     bg->m_CatName->Place({bg->GetCoordinate() + catNameOffset});
//     bg->m_CatName->SetColor(Util::Color::FromName(Util::Colors::WHITE));
//
//     bg->AddChild(bg->m_CatName);
//
//     // cat level
//     std::string level = std::to_string(unlockedCats[i].level);
//     bg->m_CatLevel = std::make_shared<Text>(32, level, 15.0F);
//
//     glm::vec2 catLevelPos = {135.0F, -8.0F};
//     bg->m_CatLevel->Place(bg->GetCoordinate() + catLevelPos);
//     bg->m_CatLevel->SetColor(Util::Color::FromName(Util::Colors::YELLOW));
//
//     bg->AddChild(bg->m_CatLevel);
//
//     m_CatSelectionBar.push_back(bg);
//
// }
