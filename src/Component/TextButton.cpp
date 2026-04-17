#include "Component/TextButton.hpp"

TextButton::TextButton(int size, const std::string& text, float zIndex, std::function<void()> Move):
    Text(size, text, zIndex), Move(Move) {}

void TextButton::Place(glm::vec2 p) {
    Text::Place(p);
    m_Transform.translation = p;
}

void TextButton::Update() {
    if (!GetVisible()) return;

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

    switch (m_State) {
    case State::HOVER:
        SetColor(Util::Color::FromName(Util::Colors::PURPLE));
        break;
    case State::PRESSDOWN:
        SetColor(Util::Color::FromName(Util::Colors::WHITE));
        break;
    case State::PRESSUP:
        SetColor(Util::Color::FromName(Util::Colors::BLACK));
        if (this->Move) this->Move();
        break;
    case State::IDLE:
    default:
        SetColor(TextThemeDetail::DefaultHighlightColor);
        break;
    }
}

glm::vec2 TextButton::GetSize() {
    return Text::GetSize();
}

glm::vec2 TextButton::GetCoordinate() {
    return Text::GetCoordinate();
}

bool TextButton::IsHovered() {
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
        GetVisible() &&
        mousePos.x >= buttonLeft && mousePos.x <= buttonRight &&
        mousePos.y >= buttonBottom && mousePos.y <= buttonTop
        );
}

bool TextButton::IsPressDown() {
    return IsHovered() && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
}

bool TextButton::IsPressUP() {
    return IsHovered() && Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB);
}


