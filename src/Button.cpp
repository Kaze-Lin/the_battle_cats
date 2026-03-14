#include "Button.hpp"

Button::Button(const std::string& imagePath)
    : GameObject(std::make_shared<Util::Image>(imagePath), -8) {
    b_OriginalSize = m_Drawable->GetSize();
}

void Button::ScaleSize(glm::vec2 s) {
    m_Transform.scale = s;
}

void Button::Place(glm::vec2 p) {
    m_Transform.translation = p;
}

glm::vec2 Button::GetCoordinate() { return  m_Transform.translation; }

glm::vec2 Button::GetSize() { return b_OriginalSize * m_Transform.scale; }
