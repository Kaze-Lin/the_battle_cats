#include "Component/Button.hpp"

#include "PhaseManager.hpp"

Button::Button(const std::string& imagePath, std::function<void()> Move)
    : GameObject(std::make_shared<Util::Image>(imagePath), -5), Move(Move) {
    m_OriginalSize = m_Drawable->GetSize();
}

void Button::ScaleSize(glm::vec2 s) {
    m_BaseScale = s;
    m_Transform.scale = s;
}

void Button::Place(glm::vec2 p) {
    m_Transform.translation = p;
}

void Button::Update() {
    if (!m_Visible) return;

    constexpr float animationSpeed = 0.2F;
    const glm::vec2 idleScaleFactor = {1.0F, 1.0F};
    const glm::vec2 hoverScaleFactor = {0.97F, 0.97F};
    const glm::vec2 pressDownScaleFactor = {0.9F, 0.9F};
    const glm::vec2 pressUpScaleFactor = {1.1F, 1.1F};

    bool isHovered = IsHovered();

    switch (m_State) {
    case State::IDLE:
        if (isHovered) m_State = State::HOVER;
        break;
    case State::HOVER:
        if (IsPressDown()) m_State = State::PRESSDOWN;
        else if (!isHovered) m_State = State::IDLE;
        break;
    case State::PRESSDOWN:
        if (!isHovered) m_State = State::IDLE;
        else if (IsPressUP()) m_State = State::PRESSUP;
        break;
    case State::PRESSUP:
        m_State = isHovered? State::HOVER: State::IDLE;
        break;
    default:
        m_State = State::IDLE;
        break;
    }


    glm::vec2 targetScaleFactor;
    switch (m_State) {
    case State::HOVER:
        targetScaleFactor = hoverScaleFactor;
        break;
    case State::PRESSDOWN:
        targetScaleFactor = pressDownScaleFactor;
        break;
    case State::PRESSUP:
        targetScaleFactor = pressUpScaleFactor;
        if (this->Move) this->Move();
        break;
    case State::IDLE:
    default:
        targetScaleFactor = idleScaleFactor;
        break;
    }

    glm::vec2 targetScale = m_BaseScale * targetScaleFactor;

    m_Transform.scale += (targetScale - m_Transform.scale) * animationSpeed;
}


glm::vec2 Button::GetCoordinate() { return  m_Transform.translation; }

glm::vec2 Button::GetSize() { return m_OriginalSize * m_Transform.scale; }

bool Button::IsHovered() {
    const auto mousePos = Util::Input::GetCursorPosition();

    const auto buttonSize = GetSize();
    const auto buttonPos = GetCoordinate();
    const auto halfWidth = buttonSize.x / 2.0F;
    const auto halfHeight = buttonSize.y / 2.0F;

    const auto buttonTop = buttonPos.y + halfHeight;
    const auto buttonBottom = buttonPos.y - halfHeight;
    const auto buttonRight = buttonPos.x + halfWidth;
    const auto buttonLeft = buttonPos.x - halfWidth;

    return (
        m_Visible &&
        mousePos.x >= buttonLeft && mousePos.x <= buttonRight &&
        mousePos.y >= buttonBottom && mousePos.y <= buttonTop
        );
}

bool Button::IsPressDown() {
    return IsHovered() && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
}

bool Button::IsPressUP() {
    return IsHovered() && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
}
