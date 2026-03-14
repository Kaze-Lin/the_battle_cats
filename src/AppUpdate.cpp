#include "App.hpp"


#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"



void App::Update() {

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

