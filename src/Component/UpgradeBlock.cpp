#include "Component/UpgradeBlock.hpp"

UpgradeBlock::UpgradeBlock(UpgradeType blockType, const std::string& imagePath, float zIndex)
    : OptionBlock(imagePath, zIndex) {
    m_OriginalSize = m_Drawable->GetSize();
    this->m_UpgradeType = blockType;
}

UpgradeType UpgradeBlock::GetBlockType() const {
    return this->m_UpgradeType;
}
