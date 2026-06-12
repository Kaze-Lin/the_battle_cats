#ifndef TWO_LAYER_TEXT_HPP
#define TWO_LAYER_TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

namespace TextThemeDetail{
    const Util::Color DefaultBackgroundColor = Util::Color::FromName(Util::Colors::BLACK);
    const Util::Color DefaultHighlightColor = Util::Color::FromName(Util::Colors::YELLOW);

    const std::string DefaultBackgroundFont = RESOURCE_DIR "/assets/fonts/NotoSansTC-Black.ttf";
    const std::string DefaultHighlightFont = RESOURCE_DIR "/assets/fonts/NotoSansTC-Regular.ttf";

    const int DefaultFontSize = 36;
}

class TwoLayerText: public Util::GameObject {
private:
    int size;
    std::string text;
    float zIndex;

public:
    TwoLayerText(
        int size,
        const std::string &text,
        float zIndex);
    TwoLayerText(
        int size,
        const std::string &text,
        float zIndex,
        Util::Color color);

    ~TwoLayerText() override = default;

    bool GetVisible();

    glm::vec2 GetSize();

    void Place(const glm::vec2 &p) override;
    void SetText(const std::string& text);
    void SetColor(Util::Color color);
    void SetVisible(bool visible) override;
    void Rotate(float degree);
};

#endif //TWO_LAYER_TEXT_HPP
