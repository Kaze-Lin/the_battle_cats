#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"

class Button : public Util::GameObject {
private:
    enum class State {
        IDLE,
        HOVER,
        PRESSDOWN,
        PRESSUP,
        CLICKED
    };
    State m_State = State::IDLE;

    glm::vec2 m_OriginalSize = {};
    glm::vec2 m_BaseScale = {1.0F, 1.0F};

public:
    explicit Button(const std::string &imagePath);

    void ScaleSize(glm::vec2 s);
    void Place(glm::vec2 p);
    void Update();

    glm::vec2 GetCoordinate();
    glm::vec2 GetSize();

    bool IsHovered();
    bool IsPressDown();
    bool IsPressUP();
};

#endif //BUTTON_HPP