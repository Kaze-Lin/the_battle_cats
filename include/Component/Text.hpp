#ifndef TEXT_HPP
#define TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

namespace TextTheme{
    const Util::Color DefaultHighlightColor = Util::Color::FromName(Util::Colors::BLACK);

    const std::string DefaultHighlightFont = RESOURCE_DIR "/assets/fonts/roboto_medium_numbers.ttf";

    const int DefaultFontSize = 36;
}

class Text: public Util::GameObject {
private:
    int size;
    std::string text;
    float zIndex;

public:
    Text(
        int size,
        const std::string &text,
        float zIndex);
    Text(
        int size,
        const std::string &text,
        float zIndex,
        Util::Color color);

    ~Text() override = default;

    bool GetVisible();

    glm::vec2 GetSize();

    void Place(const glm::vec2 &p) override;
    void SetText(const std::string& text);
    void SetColor(Util::Color color);
    void SetVisible(bool visible) override;
    void Rotate(float degree);
};

#endif //TEXT_HPP