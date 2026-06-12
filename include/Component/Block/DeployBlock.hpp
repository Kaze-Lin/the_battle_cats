#ifndef DEPLOY_BLOCK_HPP
#define DEPLOY_BLOCK_HPP

#include "Component/Block/OptionBlock.hpp"
#include "Component/Text.hpp"
#include "pch.hpp"

enum class DeployType {
    FOLDER,
    CHARACTER
};

struct CatSaveData;

class DeployBlock: public OptionBlock {
public:
    DeployBlock(
        DeployType deployType,
        const std::string& bgImagePath,
        glm::vec2 position,
        const CatSaveData& catSaveData,
        float zIndex = 10.0F
        );
    ~DeployBlock() override = default;



    [[nodiscard]] DeployType GetBlockType() const;

private:
    DeployType m_DeployType = DeployType::CHARACTER;

    std::shared_ptr<BackgroundImage> m_CatBlockImage;

    std::shared_ptr<Text> m_CatName;
    std::shared_ptr<Text> m_Max;
    std::shared_ptr<Text> m_CatLevel;
    std::shared_ptr<Text> m_DeployCost;
};

#endif //DEPLOY_BLOCK_HPP
