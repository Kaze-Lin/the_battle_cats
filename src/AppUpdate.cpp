#include "App.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Update() {

    m_b_Start->Update();
    m_b_Upgrade->Update();
    m_b_TeamBuild->Update();
    m_b_RestoreIcon->Update();
    m_b_NormalGachaIcon->Update();
    m_SpecialGachaIcon->Update();
    m_PropsStore->Update();
    m_Back->Update();


    m_Root.Update();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (m_Back->IsPressUP() ||
        Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
        }
}
