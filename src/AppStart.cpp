#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    catBase = std::make_shared<CatBase>();
    m_Root.AddChildren(catBase->m_catBase);






    m_CurrentState = State::UPDATE;
}


