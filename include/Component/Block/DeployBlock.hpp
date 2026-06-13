#ifndef DEPLOY_BLOCK_HPP
#define DEPLOY_BLOCK_HPP

#include "Component/Block/OptionBlock.hpp"
#include "Component/TwoLayerText.hpp"
#include "DataBase/UserData.hpp"
#include "pch.hpp"

enum class DeployType {
    FOLDER,
    CHARACTER
};

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
    [[nodiscard]] const CatSaveData& GetCatSaveData() const;

private:
    DeployType m_DeployType = DeployType::CHARACTER;
    CatSaveData m_CatSaveData;

    std::shared_ptr<BackgroundImage> m_CatBlockImage;

    std::shared_ptr<TwoLayerText> m_CatName;
    std::shared_ptr<TwoLayerText> m_Max;
    std::shared_ptr<TwoLayerText> m_CatLevel;
    std::shared_ptr<TwoLayerText> m_DeployCost;
};

#endif //DEPLOY_BLOCK_HPP
