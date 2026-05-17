#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/Text.hpp"

BackgroundImage::BackgroundImage(const std::string& imagePath, float zIndex)
    : GameObject(std::make_shared<Util::Image>(imagePath), zIndex) {
    m_OriginalSize = m_Drawable->GetSize();
}

void BackgroundImage::AlignWithWindow() {
    auto backgroundImageSize = m_Drawable->GetSize();
    float scaleX = System::GetWindowWidth() / backgroundImageSize.x;
    float scaleY = System::GetWindowHeight() / backgroundImageSize.y;

    m_Transform.scale = {scaleX, scaleY};
}

void BackgroundImage::AlignWithWindowWidth() {
    auto backgroundImageSize = m_Drawable->GetSize();
    float scaleX = System::GetWindowWidth() / backgroundImageSize.x;

    m_Transform.scale = {scaleX, scaleX};
    m_BaseScale = m_Transform.scale;
}

void BackgroundImage::ScaleSize(glm::vec2 s) {
    m_BaseScale = s;
    m_Transform.scale = s;
}

void BackgroundImage::Place(const glm::vec2 &p) {
    glm::vec2 delta = p - GetCoordinate();
    GameObject::Place(p);

    if (GetChildren().empty()) return;

    for (auto &item: GetChildren()) {
        item->Place(item->GetCoordinate() + delta);
    }
}

void BackgroundImage::SetImage(const std::string& ImagePath){
    m_Drawable = std::make_shared<Util::Image>(ImagePath);
}

glm::vec2 BackgroundImage::GetSize() {
    return m_OriginalSize * m_BaseScale;
}

glm::vec2 BackgroundImage::GetCoordinate() const {
    return m_Transform.translation;
}
