#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "Component/Text.hpp"
#include "Util/GameObject.hpp"
#include "Component/Button.hpp"
#include "IStateful.hpp"

class TextButton: public Text, public IStateful {
private:
    int fontSize = 16;

    enum class State {
        IDLE,
        HOVER,
        PRESSDOWN,
        PRESSUP,
        CLICKED
    };
    State m_State = State::IDLE;

    std::function<void()> Move = nullptr;

public:
    TextButton(int size, const std::string& text, float zIndex, std::function<void()> Move);
    ~TextButton() override = default;

    glm::vec2 GetCoordinate();
    glm::vec2 GetSize();

    bool IsHovered();
    bool IsPressDown();
    bool IsPressUP();

    void Place(glm::vec2 p);
    void Update() override;
};

#endif //TEXT_BUTTON_HPP
