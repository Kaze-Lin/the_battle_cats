#include "Component/Text.hpp"

Text::Text(
    int size,
    const std::string &text,
    float zIndex
    ): Util::GameObject(nullptr, zIndex),
    size(size), text(text), zIndex(zIndex) {

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

glm::vec2 Text::GetSize() {
    glm::vec2 maxSize = {0.0F, 0.0F};
    for (const auto &it: GetChildren()) {
        auto s = it->GetDrawable()->GetSize();
        if (glm::all(glm::lessThan(maxSize, s))) {
            maxSize = s;
        }
    }
    return maxSize;
}

void Text::Place(glm::vec2 p) {
    for (auto &it: m_Children)
        it->m_Transform.translation = p;
}

void Text::SetText(const std::string& text) {
    for (auto &it: m_Children) {
        auto item = std::dynamic_pointer_cast<Util::Text>(it->GetDrawable());
        item->SetText(text);
    }
}
