#include "Component/OptionBlock.hpp"

OptionBlock::OptionBlock(BlockType blockType, const std::string& imagePath, float zIndex)
    : BackgroundImage(imagePath, zIndex) {
    m_OriginalSize = m_Drawable->GetSize();
    this->m_BlockType = blockType;
}

BlockType OptionBlock::GetBlockType() const {
    return this->m_BlockType;
}
