#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Update() {
    const std::shared_ptr<Phase> NewPhase = this->m_Phase->GetDestinationPhase();
    if (NewPhase) {
        this->m_Root.RemoveChild(this->m_Phase);
        this->m_Phase = NewPhase;
        this->m_Root.AddChild(this->m_Phase);
    }

    m_Phase->Update();

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
