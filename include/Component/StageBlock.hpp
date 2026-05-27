#ifndef STAGE_BLOCK_HPP
#define STAGE_BLOCK_HPP

#include "Component/Text.hpp"
#include "Component/OptionBlock.hpp"

class StageBlock: public OptionBlock {
public:
    StageBlock(
        const std::string& bg,
        float zIndex = 10.0F
        );

    std::array<int, 2> m_StageID = {1, 1};

    std::shared_ptr<Text> m_StageName;
    std::shared_ptr<Text> m_Clear;

};

#endif //STAGE_BLOCK_HPP
