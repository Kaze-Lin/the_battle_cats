#include <algorithm>

#include "EntityManager.hpp"
#include "DatabaseManager.hpp"
#include "StatCalculator.hpp"
#include "LevelManager.hpp"


// ==========================================
// 召喚貓咪
// ==========================================
void EntityManager::SpawnCat(int catId, int level, int form) {
    // 1. 從資料庫取得該貓咪的圖鑑資料
    const UnitData* catData = DatabaseManager::GetInstance().GetCatData(catId);
    if (catData == nullptr) {
        std::cerr << "[EntityManager] 召喚失敗：找不到 ID 為 " << catId << " 的貓咪！\n";
        return;
    }

    if (form < 0 || form >= catData->forms.size()) {
        form = 0; // 預設退回一階
    }

    // 取出特定型態的資料
    const FormData& formData = catData->forms[form];

    // 2. 打包成通用的 UnitStats
    UnitStats stats;
    stats.name = formData.name;
    stats.maxHp = StatCalculator::CalculateCatHp(formData, level);
    stats.attackPower = StatCalculator::CalculateCatAttack(formData, level);
    stats.range = formData.range;
    stats.isAreaAttack = formData.isAreaAttack;
    stats.moveSpeed = formData.moveSpeed;
    stats.knockbackLimit = formData.knockback;

    // 將幀數轉換為秒數 (以 30 FPS 為基準)
    const float FPS = 30.0f;
    stats.precastTime = formData.animation.precast / FPS;
    stats.postcastTime = formData.animation.postcast / FPS;
    stats.cooldownTime = formData.timeBetweenAttacks / FPS;

    // 3. 決定出生座標 (貓咪固定從左側自己的主塔生出來，假設座標是 0.0f 或 100.0f)
    float startX = 100.0f;

    // 4. 生成實體並推入陣列！
    m_playerUnits.push_back(std::make_unique<Unit>(Faction::Player, stats, startX));

    std::cout << "[EntityManager] 召喚貓咪：" << stats.name << " (等級 " << level << ")，血量：" << stats.maxHp << "\n";
}

// ==========================================
// 召喚敵人
// ==========================================
void EntityManager::SpawnEnemy(int enemyId, int magnification) {
    // 1. 從資料庫取得敵人的圖鑑資料
    const EnemyData* enemyData = DatabaseManager::GetInstance().GetEnemyData(enemyId);
    if (enemyData == nullptr) {
        std::cerr << "[EntityManager] 召喚失敗：找不到 ID 為 " << enemyId << " 的敵人！\n";
        return;
    }

    // 2. 打包成通用的 UnitStats
    UnitStats stats;
    stats.name = enemyData->name;
    stats.maxHp = StatCalculator::CalculateEnemyHp(*enemyData, magnification);
    stats.attackPower = StatCalculator::CalculateEnemyAttack(*enemyData, magnification);
    stats.range = enemyData->range;
    stats.isAreaAttack = enemyData->isAreaAttack;
    stats.moveSpeed = enemyData->moveSpeed;
    stats.knockbackLimit = enemyData->knockback;

    // 💥 新增：根據 enemyId 給予對應的圖片路徑
    // 假設您的圖片命名為 enemy_0.png, enemy_1.png... 並放在 assets/images/ 裡面
    stats.imagePath = "Resources/enemy_materials" + std::to_string(enemyId) + ".png";

    // 將幀數轉換為秒數
    const float FPS = 30.0f;
    stats.precastTime = enemyData->animation.precast / FPS;
    stats.postcastTime = enemyData->animation.postcast / FPS;
    stats.cooldownTime = enemyData->timeBetweenAttacks / FPS;

    // 3. 決定出生座標 (敵人從右側的敵方主塔生出來)
    // 這裡我們去向 LevelManager 詢問這關的主塔有多遠 (background_length)
    float startX = static_cast<float>(LevelManager::GetInstance().GetBackgroundLength());

    // 防呆：如果關卡沒載入，給一個預設距離
    if (startX <= 0.0f) startX = 3000.0f;

    // 4. 生成實體並推入陣列！
    m_enemyUnits.push_back(std::make_unique<Unit>(Faction::Enemy, stats, startX));

    std::cout << "[EntityManager] 召喚敵人：" << stats.name << " (倍率 " << magnification << "%)，血量：" << stats.maxHp << "\n";
}

void EntityManager::Update(float deltaTime) {

    // 1. 更新所有貓咪
    for (auto& cat : m_playerUnits) {
        cat->Update(deltaTime);
    }

    // 2. 更新所有敵人
    for (auto& enemy : m_enemyUnits) {
        enemy->Update(deltaTime);
    }

    // 3. 清理戰場 (移除死亡的實體)
    // 使用 C++ 標準庫的 erase-remove_if 技巧，把 "已經死掉且死亡動畫播完" 的單位刪除
    m_playerUnits.erase(
        std::remove_if(m_playerUnits.begin(), m_playerUnits.end(),
            [](const std::unique_ptr<Unit>& u) { return u->IsDeadAndAnimationFinished(); }
        ),
        m_playerUnits.end()
    );

    m_enemyUnits.erase(
        std::remove_if(m_enemyUnits.begin(), m_enemyUnits.end(),
            [](const std::unique_ptr<Unit>& u) { return u->IsDeadAndAnimationFinished(); }
        ),
        m_enemyUnits.end()
    );
}

void EntityManager::Render() {
    for (auto& cat : m_playerUnits) {
        cat->Render();
    }
    for (auto& enemy : m_enemyUnits) {
        enemy->Render();
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

// ==========================================
// 取得特定範圍內的敵人 (用於戰鬥碰撞、範圍攻擊)
// ==========================================
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