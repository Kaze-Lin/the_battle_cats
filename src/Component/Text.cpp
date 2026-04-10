#include "Component/Text.hpp"

Text::Text(
    int size,
    const std::string &text,
    float zIndex
    ): Util::GameObject(nullptr, zIndex) {

    auto backgroundText = std::make_shared<Util::GameObject>(
        std::make_shared<Util::Text>(
            TextThemeDetail::DefaultBackgroundFont,
            size,
            text,
            TextThemeDetail::DefaultBackgroundColor
        ),
        zIndex - 1
        );
    AddChild(backgroundText);

    auto highlightText = std::make_shared<Util::GameObject>(
        std::make_shared<Util::Text>(
            TextThemeDetail::DefaultHighlightFont,
            size,
            text,
            TextThemeDetail::DefaultHighlightColor
        ),
        zIndex
        );
    AddChild(highlightText);

};

void Text::Place(glm::vec2 p) {
    for (auto &it: m_Children)
        it->m_Transform.translation = p;
}
