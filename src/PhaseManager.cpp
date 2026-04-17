#include "PhaseManager.hpp"

size_t PhaseManager::m_PathSize;

std::unordered_map<
    std::string,
    std::function<std::shared_ptr<Phase>()>
> PhaseManager::m_Map;


std::vector<std::string> PhaseManager::m_Path = {};

std::string PhaseManager::m_Dest = "";

void PhaseManager::MapInitialize() {

    m_PathSize = 0;

    m_Map["Login"] = [](){return std::make_shared<Login>();};

    m_Map["CatBase"] = [](){ return std::make_shared<CatBase>(); };

    m_Map["PropsStore"] = [](){ return std::make_shared<PropsStore>(); };

    m_Map["StageSelection"] = [](){ return std::make_shared<StageSelection>(); };

    m_Map["Upgrade"] = [](){ return std::make_shared<Upgrade>(); };

    m_Map["Fight"] = [](){ return std::make_shared<Fight>(); };
}


std::shared_ptr<Phase> PhaseManager::GetFirstPhase() {
    return m_Map.find("Login")->second();
}

std::shared_ptr<Phase> PhaseManager::GetNextPhase(const std::string& source, const std::string& destination) {
    auto it = m_Map.find(destination);

    if (it != m_Map.end()) {
        m_Path.push_back(source);
        m_PathSize += 1;
        return it->second();
    } else {
        return nullptr;
    }
}

std::shared_ptr<Phase> PhaseManager::GetNextPhase() {
    if (m_PathSize > 0) {
        std::string dest = m_Path.back();
        m_Path.pop_back();
        m_PathSize -= 1;
        return m_Map.find(dest)->second();
    }
    return nullptr;
}
