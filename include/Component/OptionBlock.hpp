#ifndef OPTION_BLOCK_HPP
#define OPTION_BLOCK_HPP

#include "Component/BackgroundImage.hpp"

enum class BlockType {
    FOLDER,
    CHARACTER,
    CANNON,
    WORKER_CAT,
    CASTLE,
    SPECIAL_ABILITIES
};

class OptionBlock: public BackgroundImage {
private:
    BlockType m_BlockType = BlockType::CHARACTER;
public:
    OptionBlock(
        BlockType blockType,
        const std::string& imagePath,
        float zIndex = 10.0F
        );

    // TODO: rewrite Place function (it should has to change position by use offset)
    // void Place(glm::vec2 p) override;

    BlockType GetBlockType() const;
};

#endif //OPTION_BLOCK_HPP
