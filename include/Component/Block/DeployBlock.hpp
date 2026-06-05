#ifndef DEPLOY_BLOCK_HPP
#define DEPLOY_BLOCK_HPP

#include "Component/Block/OptionBlock.hpp"
#include "Component/Text.hpp"
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
        float zIndex = 10.0F,

        int id = 1
        );
    ~DeployBlock() override = default;



    [[nodiscard]] DeployType GetBlockType() const;

private:
    int m_ID;
    DeployType m_DeployType = DeployType::CHARACTER;

    std::shared_ptr<BackgroundImage> m_CatBlockImage;

    std::shared_ptr<Text> m_CatName;
    std::shared_ptr<Text> m_Max;
    std::shared_ptr<Text> m_CatLevel;
};

#endif //DEPLOY_BLOCK_HPP
