#ifndef UPGRADE_BLOCK_HPP
#define UPGRADE_BLOCK_HPP

#include "Component/Block/OptionBlock.hpp"
#include "Component/Text.hpp"

enum class UpgradeType {
    FOLDER,
    CHARACTER,
    CANNON,
    WORKER_CAT,
    CASTLE,
    SPECIAL_ABILITIES
};

class UpgradeBlock: public OptionBlock {
private:
    UpgradeType m_UpgradeType = UpgradeType::CHARACTER;
public:
    UpgradeBlock(
        UpgradeType blockType,
        const std::string& imagePath,
        float zIndex = 10.0F
        );
    ~UpgradeBlock() override = default;

    int ID = -1;

    std::shared_ptr<BackgroundImage> m_CatBlockImage;
    std::shared_ptr<Text> m_CatName;
    std::shared_ptr<Text> m_Max;
    std::shared_ptr<Text> m_CatLevel;
    std::shared_ptr<Text> m_UpgradeXP;

    [[nodiscard]] UpgradeType GetBlockType() const;
};


#endif //UPGRADE_BLOCK_HPP
