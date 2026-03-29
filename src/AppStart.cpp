#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "DatabaseManager.hpp"

void App::Start() {
    LOG_TRACE("Start");

    PhaseManager::MapInitialize();

    m_Phase = PhaseManager::GetFirstPhase();
    m_Root.AddChild(m_Phase);




    if (!DatabaseManager::GetInstance().LoadCatData(RESOURCE_DIR "/assets/Database/CatData.json")) {
        m_CurrentState = State::END;
        return;
    }
    m_CurrentState = State::UPDATE;
}


