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

    DatabaseManager::GetInstance().LoadCatData("Resources/assets/Database/CatData.json");
    DatabaseManager::GetInstance().LoadEnemyData("Resources/assets/Database/EnemyData.json");
    DatabaseManager::GetInstance().LoadStageData("Resources/assets/Database/stageData.json");


    if (!DatabaseManager::GetInstance().LoadCatData(RESOURCE_DIR "/assets/Database/CatData.json")) {
        m_CurrentState = State::END;
        return;
    }
    m_CurrentState = State::UPDATE;
}


