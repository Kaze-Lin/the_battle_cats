#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <vector>
#include <memory>
#include "Unit.hpp"


class EntityManager {
private:
    std::vector<std::shared_ptr<Unit>> m_playerUnits;
    std::vector<std::shared_ptr<Unit>> m_enemyUnits;

    EntityManager() = default;
    ~EntityManager() = default;

    Util::GameObject* m_SceneNode = nullptr;

public:
    void SetSceneNode(Util::GameObject* node) { m_SceneNode = node; }
    void SpawnCatBase(int level);
    void SpawnEnemyBase(int hp, const std::string& imagePath, float positionX);

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    static EntityManager& GetInstance() {
        static EntityManager instance;
        return instance;
    }

    void SpawnCat(int catId, int level, int form);
    void SpawnEnemy(int enemyId, int magnification);

    void Update(float deltaTime);
    void Render();

    int GetEnemyCount() const { return m_enemyUnits.size(); }

    // 取得特定陣營的所有實體 (供範圍攻擊或技能使用)
    std::vector<Unit*> GetEntitiesByFaction(Faction faction);

    // 取得在特定 X 座標範圍內的敵人 (供單體/範圍攻擊判定)
    std::vector<Unit*> GetEntitiesInRange(Faction targetFaction, float startX, float endX);
};

#endif