#include <algorithm>

#include "EntityManager.hpp"
#include "DatabaseManager.hpp"
#include "StatCalculator.hpp"
#include "LevelManager.hpp"

void EntityManager::SpawnCat(int catId, int level, int form) {
    const UnitData* catData = DatabaseManager::GetInstance().GetCatData(catId);
    if (catData == nullptr) {
        LOG_WARN("cannot find cat data");
        return;
    }

    if (form < 0 || form >= catData->forms.size()) {
        form = 0;
    }
    const FormData& formData = catData->forms[form];

    UnitStats stats;
    stats.name = formData.name;

    if (form < catData->catPath.size()) {
        stats.photoPath = RESOURCE_DIR + catData->catPath[form];
    } else {
        stats.photoPath = "";
    }

    stats.maxHp = StatCalculator::CalculateCatHp(formData, level);
    stats.attackPower = StatCalculator::CalculateCatAttack(formData, level);
    stats.range = formData.range;
    stats.isAreaAttack = formData.isAreaAttack;
    stats.moveSpeed = formData.moveSpeed;
    stats.knockbackLimit = formData.knockback;

    const float FPS = 30.0f;
    stats.precastTime = formData.animation.precast / FPS;
    stats.postcastTime = formData.animation.postcast / FPS;
    stats.cooldownTime = formData.timeBetweenAttacks / FPS;

    float startX = 515.0f; // it depended

    auto cat = std::make_shared<Unit>(Faction::Player, stats, startX);
    m_playerUnits.push_back(cat);

    if (m_SceneNode) m_SceneNode->AddChild(cat);

    LOG_INFO("[EntityManager] deploy cat: " + stats.name);
}

void EntityManager::SpawnEnemy(int enemyId, int magnification) {
    const EnemyData* enemyData = DatabaseManager::GetInstance().GetEnemyData(enemyId);
    if (enemyData == nullptr) {
        LOG_WARN("cannot find enemy data");
        return;
    }

    UnitStats stats;
    stats.name = enemyData->name;
    stats.photoPath = RESOURCE_DIR + enemyData->photoPath;;
    stats.maxHp = StatCalculator::CalculateEnemyHp(*enemyData, magnification);
    stats.attackPower = StatCalculator::CalculateEnemyAttack(*enemyData, magnification);
    stats.range = enemyData->range;
    stats.isAreaAttack = enemyData->isAreaAttack;
    stats.moveSpeed = enemyData->moveSpeed;
    stats.knockbackLimit = enemyData->knockback;

    // 將幀數轉換為秒數
    const float FPS = 100.0f;
    stats.precastTime = enemyData->animation.precast / FPS;
    stats.postcastTime = enemyData->animation.postcast / FPS;
    stats.cooldownTime = enemyData->timeBetweenAttacks / FPS;

    // 這裡我們去向 LevelManager 詢問這關的主塔有多遠 (background_length)
    float startX = static_cast<float>(LevelManager::GetInstance().GetBackgroundLength());

    startX = -540.0f;

    auto enemy = std::make_shared<Unit>(Faction::Enemy, stats, startX);
    m_enemyUnits.push_back(enemy);

    if (m_SceneNode) m_SceneNode->AddChild(enemy);

    LOG_INFO("[EntityManager] deploy enemy: " + stats.name);
}

void EntityManager::SpawnCatBase(int level) {
    UnitStats stats;
    stats.name = "Cat_Base";
    stats.photoPath = RESOURCE_DIR "/cat_base/0.png" ;
    stats.maxHp = 1000 + (level * 1000);
    stats.attackPower = 0; stats.moveSpeed = 0.0f; stats.knockbackLimit = 1;
    stats.range = 0.0f; stats.isAreaAttack = false; stats.precastTime = 0.0f; stats.postcastTime = 0.0f; stats.cooldownTime = 999.0f;

    auto base = std::make_shared<Unit>(Faction::Player, stats, 521.0f);
    m_catBase = base;
    m_playerUnits.push_back(base);
    if (m_SceneNode) m_SceneNode->AddChild(base);
}

void EntityManager::SpawnEnemyBase(int hp, const std::string& imagePath, float positionX) {
    UnitStats stats;
    stats.name = "Enemy_Base";
    stats.photoPath = RESOURCE_DIR + imagePath;
    stats.maxHp = hp;
    stats.attackPower = 0; stats.moveSpeed = 0.0f; stats.knockbackLimit = 1;
    stats.range = 0.0f; stats.isAreaAttack = false; stats.precastTime = 0.0f; stats.postcastTime = 0.0f; stats.cooldownTime = 999.0f;

    auto base = std::make_shared<Unit>(Faction::Enemy, stats, -545.0f);
    m_enemyBase = base;
    m_enemyUnits.push_back(base);
    if (m_SceneNode) m_SceneNode->AddChild(base);
}

void EntityManager::Update(float deltaTime) {

    for (auto& cat : m_playerUnits) {
        cat->Update(deltaTime);
    }

    for (auto& enemy : m_enemyUnits) {
        enemy->Update(deltaTime);
    }

    for (auto it = m_playerUnits.begin(); it != m_playerUnits.end(); ) {
        if ((*it)->IsDeadAndAnimationFinished()) {
            if (m_SceneNode) {
                m_SceneNode->RemoveChild(*it);
            }
            LOG_INFO("A Cat unit is removed from the scene.");
            it = m_playerUnits.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = m_enemyUnits.begin(); it != m_enemyUnits.end(); ) {
        if ((*it)->IsDeadAndAnimationFinished()) {
            if (m_SceneNode) {
                m_SceneNode->RemoveChild(*it);
            }
            LOG_INFO("An Enemy unit is removed from the scene.");
            it = m_enemyUnits.erase(it);
        } else {
            ++it;
        }
    }
}

void EntityManager::Render() {
    for (auto& cat : m_playerUnits) {
        cat->Draw();
    }

    // 畫出所有敵人跟敵方塔
    for (auto& enemy : m_enemyUnits) {
        enemy->Draw();
    }
}

std::vector<Unit*> EntityManager::GetEntitiesByFaction(Faction faction) {
    std::vector<Unit*> result;

    if (faction == Faction::Player) {
        for (auto& unit : m_playerUnits) {
            result.push_back(unit.get()); // get() 可以從 unique_ptr 拿出原始指標
        }
    } else {
        for (auto& unit : m_enemyUnits) {
            result.push_back(unit.get());
        }
    }
    return result;
}

std::vector<Unit*> EntityManager::GetEntitiesInRange(Faction targetFaction, float startX, float endX) {
    std::vector<Unit*> result;

    // 確保範圍大小正確 (min 永遠小於 max)
    float minX = std::min(startX, endX);
    float maxX = std::max(startX, endX);

    // 決定要搜尋的目標陣營
    const auto& targetList = (targetFaction == Faction::Player) ? m_playerUnits : m_enemyUnits;

    for (const auto& unit : targetList) {
        float pos = unit->GetPositionX();

        // 條件：在範圍內，且「不能是死亡狀態」(避免鞭屍)
        if (pos >= minX && pos <= maxX && !unit->IsDead()) {
            result.push_back(unit.get());
        }
    }

    return result;
}
void EntityManager::ClearFactionUnitsExceptBase(Faction faction) {
    if (faction == Faction::Enemy) {
        for (auto it = m_enemyUnits.begin(); it != m_enemyUnits.end(); ) {
            if (*it != m_enemyBase) {
                if (m_SceneNode) {
                    m_SceneNode->RemoveChild(*it); // 從畫面上拔除
                }
                it = m_enemyUnits.erase(it);       // 從陣列中解構
            } else {
                ++it;
            }
        }
        LOG_INFO("All enemy units have been eliminated from the field!");
    }
    else if (faction == Faction::Player) {
        // 清理貓咪
        for (auto it = m_playerUnits.begin(); it != m_playerUnits.end(); ) {
            if (*it != m_catBase) {
                if (m_SceneNode) {
                    m_SceneNode->RemoveChild(*it);
                }
                it = m_playerUnits.erase(it);
            } else {
                ++it;
            }
        }
        LOG_INFO("All player units have been eliminated from the field!");
    }
}