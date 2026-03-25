#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "DatabaseManager.hpp"

void App::Start() {
    LOG_TRACE("Start");

    catBase = std::make_shared<CatBase>();
    m_Root.AddChildren(catBase->m_catBase);




    if (!DatabaseManager::GetInstance().LoadCatData(RESOURCE_DIR "/assets/Database/CatData.json")) {
        m_CurrentState = State::END;
        return;
    }

    m_CurrentState = State::UPDATE;
}


