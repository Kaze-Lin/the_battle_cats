#ifndef SCROLL_CONTAINER_HPP
#define SCROLL_CONTAINER_HPP

#include "Util/GameObject.hpp"

/**
 * @brief An invisible container that propagates its position to all children.
 * 
 * This achieves O(1) movement from the perspective of the manager,
 * while maintaining the correct global coordinates for children in the PTSD engine.
 */
class ScrollContainer : public Util::GameObject {
public:
    ScrollContainer() : Util::GameObject(nullptr, 0.0f) {}

    void Place(const glm::vec2& p) override {
        glm::vec2 delta = p - GetCoordinate();
        Util::GameObject::Place(p);

        for (auto& child : GetChildren()) {
            child->Place(child->GetCoordinate() + delta);
        }
    }
};

#endif // SCROLL_CONTAINER_HPP
