#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "DatabaseManager.hpp"
#include "LevelManager.hpp"
#include "EntityManager.hpp"
#include "Unit.hpp"



void App::Start() {
    LOG_TRACE("Start");

    PhaseManager::MapInitialize();

    m_Phase = PhaseManager::GetFirstPhase();
    m_Root.AddChild(m_Phase);

    bool loadFailed =
        !DatabaseManager::GetInstance().LoadCatData(RESOURCE_DIR "/assets/Database/CatData.json") ||
        !DatabaseManager::GetInstance().LoadEnemyData(RESOURCE_DIR "/assets/Database/EnemyData.json") ||
        !DatabaseManager::GetInstance().LoadStageData(RESOURCE_DIR "/assets/Database/stageData.json");

    if (loadFailed) {
        m_CurrentState = State::END;
        return;
    }
    m_CurrentState = State::UPDATE;
}
