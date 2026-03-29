#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Update() {
    // Phase transition
    const auto NewPhase = this->m_Phase->GetDestinationPhase();
    if (NewPhase) {
        std::string temp = std::to_string(PhaseManager::m_PathSize) + " ";
        for (auto &i: PhaseManager::m_Path) {
            temp += i;
            temp += " ";
        }
        LOG_DEBUG(temp);

        this->m_Root.RemoveChild(this->m_Phase);
        this->m_Phase = NewPhase;
        this->m_Root.AddChild(this->m_Phase);
    }

    m_Phase->Update();

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB)) {
        const glm::vec2 mousePos = Util::Input::GetCursorPosition();
        LOG_DEBUG("Click Coordinate: X={}, Y={}", mousePos.x, mousePos.y);
    }

    m_Root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
        }
}
