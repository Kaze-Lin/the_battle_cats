#include "Component/Block/OptionBlock.hpp"

OptionBlock::OptionBlock(const std::string& imagePath, float zIndex)
    : BackgroundImage(imagePath, zIndex) {
    m_OriginalSize = m_Drawable->GetSize();
}
