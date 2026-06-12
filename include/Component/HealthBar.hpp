#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

#include "Util/GameObject.hpp"
#include "IStateful.hpp"
#include "Unit.hpp"
#include "Component/TwoLayerText.hpp"

/**
 * @brief A UI component for displaying a health bar and text relative to a Unit.
 */
class HealthBar : public Util::GameObject, public IStateful {
private:
    std::shared_ptr<Unit> m_Target;
    std::shared_ptr<Util::GameObject> m_HpTextObj;
    glm::vec2 m_Offset;

public:
    HealthBar(std::shared_ptr<Unit> target, const glm::vec2& offset = {0.0f, 150.0f});
    ~HealthBar() override = default;

    void Update() override;
    void SetVisible(bool visible) override;
};

#endif // HEALTHBAR_HPP
