#ifndef TEXT_HPP
#define TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

namespace TextThemeDetail{
    const Util::Color DefaultBackgroundColor = Util::Color::FromName(Util::Colors::BLACK);
    const Util::Color DefaultHighlightColor = Util::Color::FromName(Util::Colors::YELLOW);

    const std::string DefaultBackgroundFont = RESOURCE_DIR "/assets/fonts/NotoSansTC-Black.ttf";
    const std::string DefaultHighlightFont = RESOURCE_DIR "/assets/fonts/NotoSansTC-Regular.ttf";

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

    ~Text() override = default;

    bool GetVisible();

    glm::vec2 GetSize();
    glm::vec2 GetCoordinate();

    void Place(glm::vec2 p);
    void SetText(const std::string& text);
    void SetColor(Util::Color color);
};

#endif //TEXT_HPP
