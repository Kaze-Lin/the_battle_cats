#include "LevelManager.hpp"
#include "DatabaseManager.hpp"
#include "EntityManager.hpp" 

bool LevelManager::LoadStage(int chapterId, int stageId) {
    const StageData* stageTemplate = DatabaseManager::GetInstance().GetStageData(chapterId, stageId);
    
    if (stageTemplate == nullptr) {
        LOG_INFO("[levelManager] loading failed!" + std::to_string(chapterId) + "." + std::to_string(stageId));
        m_isStageLoaded = false;
        return false;
    }

    // 複製一份劇本到 m_currentStage，因為我們需要修改裡面的動態計時器
    m_currentStage = *stageTemplate;
    m_stageTime = 0.0f;
    m_isStageLoaded = true;

    // 重置所有出怪規則的動態變數
    for (auto& rule : m_currentStage.spawnRules) {
        rule.currentCount = 0;
        rule.timer = 0.0f;
        rule.isUnlocked = false;
    }

    LOG_INFO("[levelManager] loading successfully" + std::to_string(chapterId) + "." + std::to_string(stageId));
    return true;
}

void LevelManager::Update(float deltaTime, float enemyBaseHpPercent) {
    if (!m_isStageLoaded) return;

    m_stageTime += deltaTime;

    int currentEnemyCount = EntityManager::GetInstance().GetEnemyCount();

    // 巡視每一條出怪規則
    for (auto& rule : m_currentStage.spawnRules) {
        
        // 1. 總出怪數量上限檢查 (-1 代表無限出怪)
        if (rule.maxCount != -1 && rule.currentCount >= rule.maxCount) {
            continue; 
        }

        // 2. 血量觸發解鎖檢查
        if (!rule.isUnlocked) {
            if (enemyBaseHpPercent <= rule.baseHpTrigger) {
                rule.isUnlocked = true; // 血量達標，解鎖規則！
                
                // 如果這隻怪是 Boss，立刻發動震退
                if (rule.isBoss) {
                    TriggerBossShockwave();
                }
            } else {
                continue; // 尚未解鎖，跳過這條規則的時間檢查
            }
        }

        // 3. 開場時間檢查 (m_stageTime 必須大於 startTime)
        if (m_stageTime >= rule.startTime) {
            
            // 推進這條規則的間隔計時器
            rule.timer += deltaTime;

            // 4. 出怪時機檢查 (第一隻怪，或是間隔時間到了)
            if (rule.currentCount == 0 || rule.timer >= rule.interval) {
                
                // 5. 同畫面敵人數量上限檢查 (enemyLimit)
                if (currentEnemyCount < m_currentStage.enemyLimit) {
                    
                    // 💥 條件全部滿足，出怪！
                    EntityManager::GetInstance().SpawnEnemy(rule.enemyId, rule.magnification);
                    
                    rule.currentCount++;
                    rule.timer = 0.0f; // 重置計時器，等待下一隻
                    
                    // 預先將當前數量 +1，避免同一幀內其他規則出怪導致總數超過上限
                    currentEnemyCount++; 
                }
                // (如果場上敵人滿了，程式會什麼都不做，timer 繼續累積，等到有空位才進來)
            }
        }
    }
}

void LevelManager::TriggerBossShockwave() {
    // 取得所有友軍 (貓咪)
    auto playerUnits = EntityManager::GetInstance().GetEntitiesByFaction(Faction::Player);
    
    for (auto* unit : playerUnits) {
        // 強制切換為擊退狀態
        // unit->ChangeState(std::make_unique<KnockbackState>(unit));
        LOG_INFO("[levelManager] cats being knockback!");
    }
}