#ifndef SCROLL_MANAGER_HPP
#define SCROLL_MANAGER_HPP

#include <vector>
#include <memory>
#include "Util/Input.hpp"

template <typename T>
class ScrollManager {
private:
    glm::vec2 m_LastMousePos = {0.0f, 0.0f};
    bool m_IsDragging = false;
    float m_SnapThreshold = 0.01f;
    float m_SnapSpeed = 0.2f;

public:
    void HorizontalMovement(const std::vector<std::shared_ptr<T>>& v, float offset) {
        for (auto& item : v) {
            item->Place({item->GetCoordinate().x - offset, item->GetCoordinate().y});
        }
    }

    size_t GetMiddleIndex(const std::vector<std::shared_ptr<T>>& v) {
        if (v.empty()) return 0;
        size_t closestIndex = 0;
        float minDistance = std::abs(v[0]->GetCoordinate().x);
        
        // Find the element with the x-coordinate closest to 0 (screen center)
        for (size_t i = 1; i < v.size(); i++) {
            float dist = std::abs(v[i]->GetCoordinate().x);
            if (dist < minDistance) {
                minDistance = dist;
                closestIndex = i;
            }
        }
        return closestIndex;
    }

    void Update(const std::vector<std::shared_ptr<T>>& v) {
        if (v.empty()) return;

        size_t midIndex = GetMiddleIndex(v);

        // 1. Retain original keyboard control logic (Original used IsKeyDown)
        if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)) {
            if (midIndex + 1 < v.size()) {
                HorizontalMovement(v, v[midIndex + 1]->GetCoordinate().x);
                return;
            }
        } else if (Util::Input::IsKeyDown(Util::Keycode::LEFT)) {
            if (midIndex > 0) {
                HorizontalMovement(v, v[midIndex - 1]->GetCoordinate().x);
                return;
            }
        }

        // 2. Mouse drag control logic (Must use IsKeyPressed for continuous holding)
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            if (!m_IsDragging) {
                m_IsDragging = true;
                m_LastMousePos = Util::Input::GetCursorPosition();
            } else {
                glm::vec2 currentMousePos = Util::Input::GetCursorPosition();
                float deltaX = currentMousePos.x - m_LastMousePos.x;
                if (std::abs(deltaX) > 0.0f) {
                    HorizontalMovement(v, -deltaX); // Move by the drag difference
                    m_LastMousePos = currentMousePos;
                }
            }
        } else {
            m_IsDragging = false;
            // 3. Smooth snapping back to center when mouse is released
            float offset = v[midIndex]->GetCoordinate().x;
            if (std::abs(offset) > m_SnapThreshold) {
                HorizontalMovement(v, offset * m_SnapSpeed); 
            }
        }
    }
};

#endif // SCROLL_MANAGER_HPP