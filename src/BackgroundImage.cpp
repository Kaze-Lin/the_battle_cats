#include "BackgroundImage.hpp"

BackgroundImage::BackgroundImage(const std::string& imagePath)
    : GameObject(std::make_shared<Util::Image>(imagePath), -10) {
    m_OriginalSize = m_Drawable->GetSize();
}

void BackgroundImage::AlignWindow() {
    auto backgroundImageSize = m_Drawable->GetSize();
    float scaleX = System::GetWindowWidth() / backgroundImageSize.x;
    float scaleY = System::GetWindowHeight() / backgroundImageSize.y;

    m_Transform.scale = {scaleX, scaleY};
}

glm::vec2 BackgroundImage::GetSize() {
    return m_OriginalSize * m_Transform.scale;
}
