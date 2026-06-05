#include "Component/Block/StageBlock.hpp"

StageBlock::StageBlock(const std::string& bg, float zIndex)
    : OptionBlock(bg, zIndex) {
    m_OriginalSize = m_Drawable->GetSize();
}
