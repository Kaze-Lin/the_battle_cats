#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "DataBase/StageData.hpp"
#include <iostream>

class LevelManager {
private:
    StageData m_currentStage;
    float m_stageTime;
    bool m_isStageLoaded;

    // 私有建構子 (單例模式)
    LevelManager() : m_stageTime(0.0f), m_isStageLoaded(false) {}
    ~LevelManager() = default;

    void TriggerBossShockwave();

public:
    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;

    static LevelManager& GetInstance() {
        static LevelManager instance;
        return instance;
    }

    bool LoadStage(int chapterId, int stageId);

    void Update(float deltaTime, float enemyBaseHpPercent);

    int GetBackgroundLength() const {
        return m_isStageLoaded ? m_currentStage.backgroundLength : 0;
    }

    int GetStageExperience() const {
        return m_isStageLoaded ? m_currentStage.experience : 0;
    }
};

#endif