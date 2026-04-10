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

public:
    Text(
        int size,
        const std::string &text,
        float zIndex);

    ~Text() override = default;

    void Place(glm::vec2 p);
};

#endif //TEXT_HPP
