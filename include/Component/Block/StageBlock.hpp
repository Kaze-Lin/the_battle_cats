#ifndef STAGE_BLOCK_HPP
#define STAGE_BLOCK_HPP

#include "Component/TwoLayerText.hpp"
#include "Component/Block/OptionBlock.hpp"

class StageBlock: public OptionBlock {
public:
    StageBlock(
        const std::string& bg,
        float zIndex = 10.0F
        );

    std::array<int, 2> m_StageID = {1, 1};

    std::shared_ptr<TwoLayerText> m_StageName;
    std::shared_ptr<TwoLayerText> m_Clear;

};

#endif //STAGE_BLOCK_HPP
