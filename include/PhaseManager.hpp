#ifndef PHASE_MANAGER_HPP
#define PHASE_MANAGER_HPP

#include <pch.hpp>
#include "Phase/Phase.hpp"
#include "Phase/CatBase.hpp"
#include "Phase/StageSelection.hpp"
#include "Phase/Upgrade.hpp"
#include "Phase/PropsStore.hpp"
#include "Phase/Fight.hpp"


class PhaseManager {
private:

    static std::unordered_map<
        std::string,
        std::function<std::shared_ptr<Phase>()>
    > m_Map;

public:
    static size_t m_PathSize;

    static std::vector<std::string> m_Path;

    static std::string m_Dest;

    static void MapInitialize();

    static std::shared_ptr<Phase> GetFirstPhase();

    // to move and store current Phase
    static std::shared_ptr<Phase> GetNextPhase(const std::string& source, const std::string& destination);

    // go back (m_Path.pop_back())
    static std::shared_ptr<Phase> GetNextPhase();
};



#endif //PHASE_MANAGER_HPP