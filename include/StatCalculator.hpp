#ifndef STATCALCULATOR_HPP
#define STATCALCULATOR_HPP

#include "DataBase/UnitData.hpp"
#include "DataBase/EnemyData.hpp"

class StatCalculator {
public:
    static int CalculateCatHp(const FormData& form, int level) {
        return form.baseHp + (level - 1) * form.hpGrowth;
    }

    static int CalculateCatAttack(const FormData& form, int level) {
        return form.baseAttack + (level - 1) * form.attackGrowth;
    }

    static int CalculateEnemyHp(const EnemyData& enemy, int magnification) {
        return static_cast<int>(enemy.baseHp * (magnification / 100.0f));
    }

    static int CalculateEnemyAttack(const EnemyData& enemy, int magnification) {
        return static_cast<int>(enemy.baseAttack * (magnification / 100.0f));
    }
};

#endif