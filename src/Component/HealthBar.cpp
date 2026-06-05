#include "Component/HealthBar.hpp"
#include "Util/Text.hpp"
#include <iomanip>
#include <sstream>

HealthBar::HealthBar(std::shared_ptr<Unit> target, const glm::vec2& offset)
    : m_Target(target), m_Offset(offset) {
    
    // Create a single-layered (single-color) text using PTSD raw Util::Text
    auto textDrawable = std::make_shared<Util::Text>(
        TextThemeDetail::DefaultBackgroundFont,
        16, // Use 16 as requested to match original font size
        "0/0",
        Util::Color::FromName(Util::Colors::BLACK)
    );

    m_HpTextObj = std::make_shared<Util::GameObject>(textDrawable, 15.0f);
    AddChild(m_HpTextObj);

    if (m_Target) {
        int current = m_Target->GetCurrentHp();
        int max = m_Target->GetMaxHp();
        textDrawable->SetText(std::to_string(current) + "/" + std::to_string(max));

        // Use callback for lower coupling
        std::weak_ptr<Util::GameObject> weakHpTextObj = m_HpTextObj;
        m_Target->SetOnHealthChanged([weakHpTextObj](int c, int m) {
            if (auto textObj = weakHpTextObj.lock()) {
                if (auto textComp = std::dynamic_pointer_cast<Util::Text>(textObj->GetDrawable())) {
                    textComp->SetText(std::to_string(c) + "/" + std::to_string(m));
                }
            }
        });
    }
}

void HealthBar::Update() {
    if (!m_Target || m_Target->IsDead()) {
        SetVisible(false);
        return;
    }

    // Follow the target's visual position
    m_Transform.translation = m_Target->GetTransform().translation + m_Offset;

    // Update internal text object position
    m_HpTextObj->m_Transform.translation = m_Transform.translation;
}

void HealthBar::SetVisible(bool visible) {
    m_Visible = visible;
    if (m_HpTextObj) {
        m_HpTextObj->SetVisible(visible);
    }
}
